# bofとは
プログラムがメモリ上の確保された領域を超えてデータを書き込み、領域外のメモリを上書きしてしまう脆弱性のこと。
## Windows
### Step1
防御機能なしのBuffer Overflow
コンパイルコマンド
```poweshell
cl.exe /Zi /Od /GS- step1.c /link /NXCOMPAT:NO /DYNAMICBASE:NO /SAFESEH:NO /OUT:step1.exe
```
## Linux