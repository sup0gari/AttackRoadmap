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
1. 実行ファイル読み込み
2. win関数のアドレス取得
3. buffer, rbpのメモリを埋めて、一番上にwin関数のアドレスを置く。
