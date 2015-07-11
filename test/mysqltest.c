#include <mysql.h>
#include <my_global.h>

int main(int argc, char **argv)
{
  MYSQL *conn;
  conn = mysql_init(NULL); 
  if (conn == NULL) 
  {
      printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      exit(1);
  } 
  if (mysql_real_connect(conn,NULL, "root", "danjina", NULL, 0, NULL, 0) == NULL) 
  {
      printf("%d:Error %u: %s\n",__LINE__, mysql_errno(conn), mysql_error(conn));
      exit(1);
  } 
/*  if (mysql_query(conn, "create database testdata2"))
  {
      printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      exit(1);
  }*/
  mysql_close(conn);
  return 0;
}