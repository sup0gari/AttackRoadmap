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
```