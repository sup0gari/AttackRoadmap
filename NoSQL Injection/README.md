# NoSQL Injectionとは
NoSQLデータベースを利用したWebアプリケーションにおける脆弱性。

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
