//
// Created by Albert , Christian on 10/8/20.
//
#include <mysql.h>
#include <string.h>
#include <stdlib.h>

int __check_hostname_needs_processing(char* host) {
    return strcmp(host, "rdbms.strato.de") == 0;
}


MYSQL *STDCALL __wrap_mysql_real_connect(MYSQL *mysql, const char *host, const char *user,
                                         const char *passwd, const char *db,
                                         uint port, const char *unix_socket, unsigned long client_flag) {
    if (__check_hostname_needs_processing(host)) {
        char *newhost = calloc(strlen(host) + strlen(user) + strlen(".mysqlmap.") + 1, sizeof(char));
        char *nhpos = newhost;
        strncpy(nhpos, user, strlen(user));
        nhpos += strlen(user);
        strncpy(nhpos, ".mysqlmap.", strlen(".mysqlmap."));
        nhpos += strlen(".mysqlmap.");
        strncpy(nhpos, host, strlen(host));
        MYSQL *stdcall_res = mysql_real_connect(mysql, newhost, *user, *passwd, *db,
                                                port, *unix_socket, client_flag);
        free(newhost);
        return stdcall_res;
    } else {
        return mysql_real_connect(mysql, host, *user, *passwd, *db,
                                  port, *unix_socket, client_flag);
    }
}

