/***************************************************************************
                          get_ini.cpp  -  description
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "get_ini.h"

void skip_left_space(char **str)
{
	while ((*(*str) == ' ') || (*(*str) == '\t'))
		(*str)++;
}

void skip_right_space(char **str)
{
	int i;
	i = strlen(*str);
  	while(i >=0 && (*(*str + i-1) ==' ' || *(*str + i-1) =='\t'))
  		i--;
  	*(*str+i) =0;
}

//--- remove left and right space and tabs ----
void skip_lr_space(char **str)
{
	skip_left_space(str);
	skip_right_space(str);
}

int isapp(const char *linedata, const char *appname)
{
	char line[LINELEN];
	char *str = line;
	char *right;

	memcpy(line, linedata, LINELEN);

	skip_lr_space(&str);
	if (*str != '[')
		return -1;
	else
	{
		str++;
		right = str;
		while (*right != '\0')
		{
			if (*right == ']')
				break;
			right++;
		}
		if (*right != '\0')
			*right = '\0';
	}
	skip_lr_space(&str);
	if(strcasecmp(str, appname) == 0)
		return(0);
	return(-1);
}

int gotoapp(FILE * file, const char *appname)
{
	char line[LINELEN];
	char *str = NULL;

	while ((str = fgets(line, LINELEN, file)) != NULL)
	{
		if (isapp(str, appname) == 0)
			return 0;
	}

	return -1;
}

/*
	返回值: -1: 当前行 line 是一节的标题行
			0:  当前行 line 是keyname指明的配置行
			1:  其它情况

*/

int notkeyline(char *line, const char *keyname, char **val)
{
	char *str = line;
	char *right;

	skip_left_space(&str);

	if (*str == '[')
		return -1;

	right = str;
	while (*right != '\0')
	{
		if ('=' == *right)
			break;
		right++;
	}

	if (*right != '\0')
	{
		*right = '\0';
	}
	else
		return 1;

	skip_lr_space(&str);
	if (strcasecmp(str, keyname) == 0 )
	{
		str = right + 1;
		skip_lr_space(&str);
		*val = str;
		return 0;
	}

	return 1;
}


int getkey(FILE * file, const char *keyname, char *buf, int len)
{
	char line[LINELEN];
	char *str = NULL;
	int  result = -1;

	while (NULL != (str = fgets(line, LINELEN, file))) {
		int len = strlen(line);
		while (line[len - 1] == '\n' || line[len - 1] == '\r')
			len--;
		line[len] = '\0';
		if ((result = notkeyline(line, keyname, &str)) == 0) {
			strncpy(buf, str, len);
			return 0;
		} else if (result < 0) {
			return -1;
		}
	}
	return -1;
}


void addappkey(FILE *file, const char *appname, const char *key,
	       const char *val)
{
	char buf[LINELEN] ="[";


	strncat(buf, appname, LINELEN-1);
	strcat(buf, "]");
	fprintf(file, "%s\n", buf);
	snprintf(buf, LINELEN, "%s=%s", key, val);
	fprintf(file, "%s\n", buf);
}

//===================== output ini opertation function ========================

/*----- get a inifile's key value, returned by string -----*/
int get_ini_string(const char *filename,const char *appname,
		   const char *keyname,const char *defval,
		   char *buf, int len)
{
	FILE *file = fopen(filename, "r+");
	
	*buf = 0;

	if (file == NULL || buf == NULL) return -1;
	if (gotoapp(file, appname) < 0)
  {
		strncpy(buf, defval, len);
		return(-1);
	}
  else
  {
		if (getkey(file, keyname, buf, len) < 0)
		{
			if(defval != NULL)
				strncpy(buf, defval, len);
			else
				buf[0] = 0;
			return(-1);
		}
	}

	fclose(file);

	return 0;
}


/*----- get a inifile's key value, returned by int -----*/
int get_ini_int(const char *filename,const char *appname,
		   const char *keyname,const int defval,
		   int *val)
{
	char buf[LINELEN];

	get_ini_string(filename,appname,keyname, NULL,buf,LINELEN);
	if(buf != NULL)
		*val = atoi(buf);
	else
		*val = defval;

	return 0;
}

/*----- get a inifile's key value, returned by int -----*/
int get_ini_bool(const char *filename,const char *appname,
		   const char *keyname,const int defval,
		   int *val)
{
	char buf[LINELEN];

	get_ini_string(filename,appname,keyname,"", buf, LINELEN);

 	if(strcasecmp(buf,"yes")==0 || strcasecmp(buf,"y")==0)
 	{
 		*val=1;
 	}
 	else if(strcasecmp(buf,"no") == 0 || strcasecmp(buf,"n")==0)
 	{
 		*val=0;
 	}
 	else
 	{
 		*val = defval;
 		return(-1);
 	}

	return(0);
}


/*------- set a inifile's key value by string -------*/
int set_ini_string( const char *filename,const char *appname,
		    const char *keyname,const char *val)
{
	int app,ret;
	FILE *file,*newfile;
	char *str,tmpname[513];
	char line[LINELEN];
	char linecpy[LINELEN];

	if ((file = fopen(filename, "r+")) == NULL)
  {
		if((file = fopen(filename, "w+"))  == NULL)
			return -1;

		addappkey(file, appname, keyname, val);
		fclose(file);
	}
	else
	{
		sprintf(tmpname, "%s~", filename);
		app = 0;
		newfile = fopen(tmpname, "w+");
		while (fgets(line, LINELEN, file) != NULL)
		{
			if (app <= 0)
			{
        if (isapp(line, appname) == 0)
          app =1;
				 fprintf(newfile, "%s", line);
			}
			else
			{
				memcpy(linecpy, line, LINELEN);
				if ((ret = notkeyline(line, keyname, &str)) == 0)
				{
					/* the key present in the assigned paragraph.
						so we change the value.
					*/
					fprintf(newfile, "%s=%s\n", keyname, val);
					app = -1;
				}
				else if (ret == 1)
				{
					fprintf(newfile, "%s", linecpy);
				}
				else
				{
					/* we find another paragraph, so we concluded that 
						the key is not present in the assigned paragraph,
						therefore we add the key & value and go on.
						! IT SEEMS we might just break here.
					*/
					fprintf(newfile, "%s=%s\n", keyname, val);
					fprintf(newfile, "%s", linecpy);
					app = -1;
				}
			}
		}
		
		if (app == 0)
		{
			addappkey(newfile, appname, keyname, val);
		} else if (app == 1) {
			/* When we got here, the assigned paragraph is found, the key is not found, 
				and the file comes to end, so we concluded that the key not present in the paragraph, 
				in with case we just add the key & value.
			*/
			
			fprintf(newfile, "%s=%s\n", keyname, val);				
		}
		
		fclose(newfile);
		fclose(file);
		rename(tmpname, filename);
	}

	return 0;
}
/*----------- set a inifile's key value by int -----------*/
int set_ini_int(const char *filename,const char *appname,
		    const char *keyname,const int val)
{
	char buf[LINELEN];
	snprintf(buf,LINELEN,"%d",val);
	return(set_ini_string( filename,appname,keyname,buf));
}

/*---------- set a inifile's key value by bool ---------*/
int set_ini_bool(const char *filename,const char *appname,
		    const char *keyname,const int val)
{
	char buf[LINELEN];

	if(val)
		snprintf(buf,LINELEN,"yes");
	else
		snprintf(buf,LINELEN,"no");

	return(set_ini_string( filename,appname,keyname,buf));
}
