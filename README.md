# CPP-SQL-FUZZER

Compile:  
`g++ main.cpp -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql -o fuzz.out`

Example of resulting DB:
```
mysql> describe good;
+----------------+---------------+------+-----+---------+----------------+
| Field          | Type          | Null | Key | Default | Extra          |
+----------------+---------------+------+-----+---------+----------------+
| id             | int(100)      | NO   | PRI | NULL    | auto_increment |
| vector         | varchar(1000) | NO   |     | NULL    |                |
| unicode_vector | varchar(1000) | NO   |     | NULL    |                |
| libinj_token   | varchar(10)   | NO   |     | NULL    |                |
| libinj_isSQLi  | int(1)        | NO   |     | NULL    |                |
+----------------+---------------+------+-----+---------+----------------+
```

Here, `detect` binary is a compiled `src/example1.c` from [libinjection repo](https://github.com/client9/libinjection) with only modification:
```
printf("%s:%d", fingerprint,issqli);
```
Please feel free to re-compile libinjection updates manually.
