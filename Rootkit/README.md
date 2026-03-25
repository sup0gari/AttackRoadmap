# Rootkitとは
自分の存在や活動をOSから完全に隠し、特権を維持し続けるためのマルウェアのこと。

## Kernel Mode Rootkit
カーネルドライバーとして動作し、プロセス、ファイル、通信の隠蔽やアンチウィルスの無効化を行うルートキットのこと。

### Step1
DKOMによる任意のプロセスの隠蔽を行う。
ここでは10秒ごとにファイル書き込みを行う`count.exe`を作成し、それを隠蔽する。
1. `count.exe`のビルド
```powershell
cl.exe /EHsc /Zi /Od /MT count.cpp /link /OUT:count.exe
```
2. カーネルドライバーのビルド
```powershell
cl /c /Zi /nologo /W3 /WX- /Od /Oi /D _AMD64_ /D _KERNEL_MODE /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\km" main.c
link /NODEFAULTLIB /INCREMENTAL:NO /SUBSYSTEM:NATIVE /DRIVER /ENTRY:DriverEntry /OUT:Step3.sys main.obj "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\km\x64\ntoskrnl.lib"
```
3. 動作確認
```powershell
tasklist | findstr count
Get-Process count
Get-Process -id <PID>
```
