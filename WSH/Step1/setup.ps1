$shell = New-Object -ComObject WScript.Shell
$lnk  = $shell.createshortcut("C:\Users\<User>\Desktop\<anything>.lnk")
$lnk.TargetPath = "C:\Windows\System32\cscript.exe"
$lnk.Arguments = "C:\temp\loader.vbs" # 外部読み込みでも可
$lnk.IconLocation = "shell32.dll, 40"
$lnk.save()

Write-Host "Shortcut Created!" -BackgroundColor Green