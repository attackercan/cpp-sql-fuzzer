###Install libraries (if not already)
```bash
apt-get install libmysqlclient-dev
```

###Compile MySQL.h version:  
```bash
g++ main.cpp -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql -o mysql_fuzz.out 
```
***

###Compile [SQLApi.h](http://www.sqlapi.com/Download/index.html) version:    
Install SQLApi by copying SQLAPI/lib/* to /usr/local/lib/*  
```bash
LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH
```
And finally compile:  
```bash
g++ main.cpp -I./include -lsqlapi -ldl -o sqlapi_fuzz.out 
```
***
###Example of output DB:
```sql
CREATE TABLE `good` (
  `id` int(100) NOT NULL AUTO_INCREMENT,
  `vector` varchar(1000) NOT NULL,
  `unicode_vector` varchar(1000) NOT NULL,
  `mysql_message` varchar(1000) NOT NULL,
  `libinj_token` varchar(10) NOT NULL,
  `libinj_isSQLi` int(1) NOT NULL,
  PRIMARY KEY (`id`)
) AUTO_INCREMENT=0 DEFAULT CHARSET=latin1;

CREATE TABLE `mysqlErrors` (
  `id` int(100) NOT NULL AUTO_INCREMENT,
  `vector` varchar(1000) NOT NULL,
  `unicode_vector` varchar(1000) NOT NULL,
  `mysql_message` varchar(1000) NOT NULL,
  `libinj_token` varchar(10) NOT NULL,
  `libinj_isSQLi` int(1) NOT NULL,
  PRIMARY KEY (`id`)
) AUTO_INCREMENT=0 DEFAULT CHARSET=latin1;
```

Here, `detect` binary is a compiled `src/example1.c` from [libinjection repo](https://github.com/client9/libinjection) with only modification:
```c++
printf("%s:%d", fingerprint, issqli);
```
PS: Please feel free to re-compile libinjection with updates manually.
