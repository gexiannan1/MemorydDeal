/***************************************************************************
                          get_ini.h  -  description
                             -------------------
    begin                : 三 12月 10 2003
    copyright            : (C) 2003 by lixx
    email                : lixx@stocom.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GET_INI_H
#define GET_INI_H

#define LINELEN	256		//INI文件每行最大字节数

int get_ini_string(const char *filename,const char *appname, const char *keyname, const char *defval, char *buf, int len);
int get_ini_int(const char *filename,const char *appname, const char *keyname,const int defval, int *val);
int get_ini_bool(const char *filename,const char *appname, const char *keyname,const int defval, int *val);

int set_ini_string( const char *filename,const char *appname, const char *keyname,const char *val);
int set_ini_int(const char *filename,const char *appname, const char *keyname,const int val);
int set_ini_bool(const char *filename,const char *appname, const char *keyname,const int val);

#endif
