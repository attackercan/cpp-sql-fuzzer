apt-get install mysql-server git g++ libmysqlclient-dev -y

service mysql start
mysql -e 'create database test; use test;
CREATE TABLE `good` (
  `id` int(100) NOT NULL AUTO_INCREMENT,
  `vector` varchar(1000) NOT NULL,
  `unicode_vector` varchar(1000) NOT NULL,
  `mysql_message` varchar(1000) NOT NULL,
  `libinj_token` varchar(10),
  `libinj_isSQLi` int(1),
  PRIMARY KEY (`id`)
) AUTO_INCREMENT=0 DEFAULT CHARSET=latin1;

CREATE TABLE `mysqlErrors` (
  `id` int(100) NOT NULL AUTO_INCREMENT,
  `vector` varchar(1000) NOT NULL,
  `unicode_vector` varchar(1000) NOT NULL,
  `mysql_message` varchar(1000) NOT NULL,
  `libinj_token` varchar(10),
  `libinj_isSQLi` int(1),
  PRIMARY KEY (`id`)
) AUTO_INCREMENT=0 DEFAULT CHARSET=latin1;
'

git clone https://github.com/attackercan/cpp-sql-fuzzer
cd cpp-sql-fuzzer

g++ main.cpp -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql -o mysql_fuzz.out
