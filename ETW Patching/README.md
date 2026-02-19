# ETWとは
Windowsカーネルやアプリケーションが「今何が起きたか」を報告するための超高速なログのようなもの。
1. Provider  
アプリケーションがログを書く
2. Consumer  
イベントビューアーやEDRがログを受け取る。
3. ETWエンジン  
1, 2の紐づけ。

## Step1
Native APIの多くは関数の戻り値を`RAX`に入れて戻すため、それを0にする。
`EtwEventWriteTransfer`の先頭を下記アセンブリで上書きすることによってETWをスキップする。
```asm
xor eax,eax
ret
```
