#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <mysql.h>
#include <algorithm>
#include <sstream>

using namespace std;

#define SERVER "localhost"
#define USER "root"
#define PASSWORD ""
#define DATABASE "test"

char* fillArray(char Alphabet[])
{

    for (int i = 32; i <= 96; i++) {
        char a = i;

        Alphabet[i - 32] = a;
    }

    int i = 123; // {
    char a = i;
    Alphabet[65] = a;

    i = 124; // |
    a = i;
    Alphabet[66] = a;

    i = 125; // }
    a = i;
    Alphabet[67] = a;

    i = 126; // ~
    a = i;
    Alphabet[68] = a;
}

bool replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;

    // replace all substrings ?
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }

    return true;
}

std::string parse_libinjection(std::string text)
{
    #define BUFSIZE 128
    char cmd[500];
    string libInjInput = text;

    // TODO: better bash escaping
    replace(libInjInput, "'", "'\\''");

    strcpy(cmd, "./detect '");
    strcat(cmd, libInjInput.c_str());
    strcat(cmd, "'");
    char buf[BUFSIZE];
    FILE* fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening file!\n");
        return "";
    }

    fgets(buf, BUFSIZE, fp);
    return buf;
}

string dec2hex(unsigned long long i)
{
    stringstream ss;
    ss << hex << uppercase << i;
    return ss.str();
}

void StringExplode(string str, string separator, vector<string>* results)
{
    int found;
    found = str.find_first_of(separator);
    while (found != string::npos) {
        if (found > 0) {
            results->push_back(str.substr(0, found));
        }
        str = str.substr(found + 1);
        found = str.find_first_of(separator);
    }
    if (str.length() > 0) {
        results->push_back(str);
    }
}

int main(int argc, char* argv[])
{
    string sql_input;
    if (argc > 1)
    {
    	sql_input = string(argv[1]);
    } else
    {
    	cout << "Please input fuzzing string with [XXX] input, e.g. ./mysql_fuzz.out 'SELECT[XXX]1 FROM tbl1'\r\n";
	return 0;
    }


    MYSQL* connect;
    connect = mysql_init(NULL);

    if (!connect) {
        cout << "MySQL Initialization Failed";
        return 1;
    }
    else {
        cout << "DB Init OK, start fuzzing";
    }

    connect = mysql_real_connect(connect, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0);

    MYSQL_RES* res_set;
    MYSQL_ROW row;

    char Alphabet[68];
    fillArray(Alphabet);

    int good = 0;

    for (int i = 0; i <= sizeof(Alphabet); i++) {
        for (int j = 0; j <= sizeof(Alphabet); j++) {
            for (int k = 0; k <= sizeof(Alphabet); k++) {
                // for(int l = 0; l <= sizeof(Alphabet); l++) {

                string FinalFuzzWord = "";
                FinalFuzzWord += Alphabet[i];
                FinalFuzzWord += Alphabet[j];
                FinalFuzzWord += Alphabet[k];
                // FinalFuzzWord += Alphabet[l];

                char tmpc1 = Alphabet[i];
                int tmpi = tmpc1;
                char tmpc2 = Alphabet[j];
                int tmpj = tmpc2;
                char tmpc3 = Alphabet[k];
                int tmpk = tmpc3;
                // char tmpc4 = Alphabet[l]; int tmpl = tmpc4;

                string FinalFuzzWordUnicode = "";
                FinalFuzzWordUnicode += "%" + dec2hex(tmpi);
                FinalFuzzWordUnicode += "%" + dec2hex(tmpj);
                FinalFuzzWordUnicode += "%" + dec2hex(tmpk);
                // FinalFuzzWordUnicode += "%" + dec2hex(tmpl);

                string sql = sql_input;
                string sql_unicode = sql;

                replace(sql, "[XXX]", FinalFuzzWord);
                replace(sql_unicode, "[XXX]", FinalFuzzWordUnicode);

                mysql_query(connect, sql.c_str());

                // Check for Syntax Errors first
                if (mysql_query(connect, sql.c_str())) {
                    string mysqlError = mysql_error(connect);

                    size_t isSyntaxError = mysqlError.find("You have an error in your SQL syntax");
                    if (isSyntaxError != 0) {

                        // cout << mysqlError;
                        // TODO: Write query for debug into different table?    E.g.:
                        // FUNCTION does not exist, TABLE not found, etc
                    }
                }

                bool querySuccessfull = false;

                res_set = mysql_store_result(connect);
                if (res_set != NULL) {
                    querySuccessfull = true;
                }

                if (querySuccessfull == true) {
                    char* escaped_sql = new char[strlen(sql.c_str()) * 3 + 1];
                    char* escaped_sql_unicode = new char[strlen(sql_unicode.c_str()) * 3 + 1];

                    mysql_real_escape_string(connect, escaped_sql, sql.c_str(),
                        sql.length());
                    mysql_real_escape_string(connect, escaped_sql_unicode,
                        sql_unicode.c_str(), sql_unicode.length());

                    string escaped_sql_string = string(escaped_sql);
                    string escaped_sql_unicode_string = string(escaped_sql_unicode);

                    vector<string> R;
                    string thisstring = parse_libinjection(escaped_sql_string);
                    StringExplode(thisstring, ":", &R);

                    string sql_add = "INSERT INTO good (id, vector, unicode_vector, "
                                     "libinj_token, libinj_isSQLi) VALUES (NULL, '"
                        + escaped_sql_string + "', '" + escaped_sql_unicode_string + "', '" + R[0] + "', '" + R[1] + "');";
                    mysql_query(connect, sql_add.c_str());

                    good++;
                }
                //}
            }
        }
    }

    cout << "\nGOOD: ";
    cout << good;
    cout << "\n";

    mysql_close(connect);

    return 0;
}