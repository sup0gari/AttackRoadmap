# BufferOverflowとは
プログラムがデータを一時的に保存するメモリ領域（バッファ）の容量を超えたデータを送り込み、隣接するメモリ領域を上書きすることで、プログラムの誤動作や強制終了、さらには悪意のあるコードの実行を可能にする脆弱性。

# Linux
## Step1
Canary, ASLR無効, NXビット無効, PIE無効のシンプルなBOF
```bash
# コンパイル
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space # ASLR無効
gcc -fno-stack-protector -z execstack -no-pie -o vuln vuln.c
```
1. 実行ファイルを読み込む。
2. win関数のアドレスを取得する。
3. buffer, rbpのメモリを埋めて、一番上にwin関数のアドレスを置く。

## Step2
Canary, ASLR無効, NXビット有効, PIE無効のRet2libcを用いたBOF
```bash
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space # ASLR無効
gcc -fno-stack-protector -z noexecstack -no-pie -o vuln vuln.c
```
1. libcのバージョンとアドレスを特定する。  
`ldd <対象バイナリ>` 
2. libcからsystem関数と/bin/shのアドレスを特定する。  
`readelf -s <lib.c.so> | grep system`  
`strings -a -t x <lib.c.so> | grep "/bin/sh"`
3. libcから`pop rdi; ret`のアドレスを特定する。  
`ROPgadget --binary <lib.c.so> | grep "pop rdi ; ret"`
4.  buffer, rbpのメモリを埋めて、実行順にアドレスを置いていく。
