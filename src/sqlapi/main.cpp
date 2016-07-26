#include <cstdlib>
#include <stdio.h>
#include <iostream>

#include <SQLAPI.h>

bool checkSQL(SAString text) {

    SAConnection con; // create connection object
    SACommand cmd;

    try {
        con.Connect(
            "test", // database name
            "root", // user name
            "", // password
            SA_MySQL_Client);
        cmd.setConnection(&con);

        cmd.setCommandText(text);
        cmd.Execute();
        //bool is_result = cmd.isResultSet();
        if(cmd.isResultSet()) return true;
    }
    catch (SAException) {
        return false;
    }
}


int main(int argc, char* argv[])
{
    bool result = checkSQL("select 1");
    printf("%d", result);

    return 0;
}
