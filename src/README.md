1. Run install.sh  
2. Watch output:  
```sql
SELECT * FROM mysqlErrors ORDER BY id DESC LIMIT 0,10; SELECT * FROM good ORDER BY id DESC LIMIT 0,10;
```  
3. Calculate libinjection tokens (only if you'd like):
3.1. Compile and run `detect` binary: `src/example1.c` from [libinjection repo](https://github.com/client9/libinjection) with only modification:
```c++
printf("%s:%d", fingerprint, issqli);
```

> ###Compile [SQLApi.h](http://www.sqlapi.com/Download/index.html) version:    
> Install SQLApi by copying SQLAPI/lib/* to /usr/local/lib/*  
> LD_LIBRARY_PATH=/usr/local/lib
> export LD_LIBRARY_PATH
> And finally compile:  
> g++ main.cpp -I./include -lsqlapi -ldl -o sqlapi_fuzz.out 
