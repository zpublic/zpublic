/* Copyright Abandoned 1996, 1999, 2001 MySQL AB, 2013 Oracle and/or
   its affiliates.
   This file is public domain and comes with NO WARRANTY of any kind */

/* Version numbers for protocol & mysqld */

#ifndef _mysql_version_h
#define _mysql_version_h
#ifdef _CUSTOMCONFIG_
#include <custom_conf.h>
#else
#define PROTOCOL_VERSION            10
#define MYSQL_SERVER_VERSION       "5.7.4-m14"
#define MYSQL_VERSION_ID            50704
#define MYSQL_PORT                  3306
#define MYSQL_PORT_DEFAULT          0
#define MYSQL_UNIX_ADDR            "/tmp/mysql.sock"
#define MYSQL_CONFIG_NAME          "my"
#define MYSQL_COMPILATION_COMMENT  "MySQL Connector/C (GPL)"
#define LIBMYSQL_VERSION           "6.1.3"
#define LIBMYSQL_VERSION_ID         60103

/* mysqld compile time options */
#endif /* _CUSTOMCONFIG_ */

#ifndef LICENSE
#define LICENSE                     GPL
#endif /* LICENSE */

#endif /* _mysql_version_h */
