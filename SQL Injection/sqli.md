# SQL Injectionとは
Webサイトの入力フォームなどを通じて、SQLに不正なコードを注入し、データベースの情報を盗み見たり、改ざん・削除したりするサイバー攻撃。

## sqlicheck3r.py
簡易的なSQLインジェクションの脆弱性チェックツール(GETパラメータ)。
```bash
┌──(kali㉿kali)-[/tmp]
└─$ python3 sqlicheck3r.py   
[*] Input URL: http://example.com/index.php?id=1
[*] Target URL: http://10.129.229.137/room.php
[*] Target Parameter: id=1
[*] Checking SQLi vulnerability...
[!] Result: The page is VULNERABLE to SQLi.
[*] Scan completed.
```