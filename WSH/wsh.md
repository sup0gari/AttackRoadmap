# WSHとは
Windowsの標準機能である WSH（Windows Script Host）を悪用し、VBScriptやJScriptなどのスクリプトファイルに仕込まれたマルウェアを実行させる。

## Step1 ショートカットの悪用
一見無害に見えるショートカットファイルを作成。実際はクリックした際に`cscript.exe`に対して引数として`loader.vbs`を渡す。`loader.vbs`は攻撃者のサーバーにアクセスし、`.sct`ファイルを実行。  
検知自体はcscript.exeからconhost.exe, regsvr32.exeからpowershell.exeとなる。