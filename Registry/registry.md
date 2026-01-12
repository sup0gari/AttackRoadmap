# レジストリとは
Windows OSの動作に必要なシステム設定、インストールされたソフトウェア、ハードウェア構成、ユーザー情報などを一元的に記録・管理する階層構造を持つデータベース。

## Step1 RunとRunOnce
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
