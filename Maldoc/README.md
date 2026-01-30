# Maldocとは
Malicious Documentの略。悪意のあるマクロを埋め込んだWordファイルやExcelファイルを利用して任意のコマンドを実行する攻撃手法。  
攻撃者は求人メールや信頼できる送信元を装ってターゲットにファイルを送り込んでマクロを読み込ませる。

## Step1 基本的なコマンド実行
Excelファイルを開いた瞬間にPowershellを呼び出し、攻撃者に対して任意のコマンドを実行する。
Workbook_Open関数とWScriptを使用し、Powershellを呼び出すマクロを作成する。

## Step2 難読化
内容自体はStep1と同じだが、現代のハッカーは静的スキャンを回避するために直接的な単語をソースコードに含めない。
1. 文字列の分割と隠蔽
Chr関数やBase64エンコード、文字列の分割により静的スキャンを回避する。
2. 親プロセスの偽装
ExcelからPowershellの起動はEDRにより検知されやすいため、ShellExecuteを使用して親プロセスをExplorerに偽装する。
3. Win32 APIの使用
Win32 APIを使用して外部からペイロードを読み込み、ファイルレスに近い状態で実行する。ExcelからAPIを呼び出すとコマンドライン引数のログには何も残らないため、検知がしにくいが、動作が重い。

## Step3 テンプレートインジェクション
.docx（または.xlsx）をZIP解凍し、ファイル内の参照先を書き換えることによって、ファイルを開いた際に外部URL（またはSMB経由）から悪意のあるマクロ付きテンプレートをロードさせる攻撃手法。

## Step4 永続化
1. XLSTART
XLSTARTに悪意のあるマクロを仕込んだ.xlsbファイルを保存し、ターゲットがExcelを開いたタイミングで書かれたマクロが実行する。
2. GlobalDotName
レジストリを書き換えてターゲットがWordを開いた際に、攻撃者が用意した悪意のあるテンプレートを標準テンプレートであるかのように読み込ませる攻撃手法。

## Step5 MotWの解除
MotWとはMark of the Webの略でインターネット経由でダウンロードしたファイルにZone.IdentifierというADSを付与し、OfficeはこのADSをチェックしてMotWがあれば保護ビューで開き、マクロの実行を無効化する。ISOに任意のマクロを含むファイルを入れたものを配布し、取り出された中身についてはMotWがつかないため、攻撃者がこれを悪用することがある。

## Step6 VBA Stomping
.docm, .dotmの内部には人間が読めるソースコードとP-Codeという二種類が存在する。ソースコード部分は無害なものに書き換え、悪意ある実行はP-Code部分だけに画すという手法をVBA Stompingと呼ぶ。
Versionが一致していないと不可なため、注意が必要。
```bash
echo "Sub Nothing()" > dummy.vba
echo "End Sub" >> dummy.vba
mono EvilClippy.exe -s dummy.vba style.dotm
```
