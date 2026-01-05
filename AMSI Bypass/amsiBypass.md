# AMSIとは
Antimalware Scan Interfaceの略。  
Windowsに搭載された機能で、アプリケーションやスクリプトが実行される際に、インストールされているウイルス対策ソフトに連携して詳細なスキャンを可能にする仕組み。

## Step1 amsiInitFailed
amsiInitFailedとはAMSIの初期化状態を示す変数である。この値が`False`だと問題なくスキャンが走る。  
この変数を`true`にしてAMSIの初期化を失敗したように認識させてバイパスする。  
通常はセキュリティ関連のPowershell内部の変数は書き換えられないが、`.NET Reflection`を利用すると可能。※その時のPowershellセッションのみ可能。
