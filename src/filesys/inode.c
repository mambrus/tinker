/***************************************************************************
 *   Copyright (C) 2007 by Michael Ambrus                                  * 
 *   michael.ambrus@maquet.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "filesys.h"

#include <filesys/inode.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#ifndef PATH_MAX
#include <sys/syslimits.h>
#endif



tk_inode_t 	*__Rnod;		//!< The root node
long long	__icntr=0;		//!< A counter used as node ID (unique)

/*! Returns the name part of a filename  */
char *igetname(const char *s){
	int i;
	for (i=strlen(s);s[i]!='/';i--);
	return (char *)&s[i+1];
}
/*! Creates a string in buff containing the path part of a file name */
void igetpath(char *buff, const char *s){	
	strncpy(buff,s,PATH_MAX);
	igetname(buff)[0]='\0';
}

/*! Return the i-node associated with a name - OR NULL if no valid node is found */
tk_inode_t *isearch(const char*s){
	char		fname[PATH_MAX];	
	char		*cd,*nd;
	tk_inode_t 	*ci 		= __Rnod;
	
	strncpy(fname,s,PATH_MAX);

	for (
		cd="",nd=strtok(fname,"/");
		nd;
		cd=nd,nd=strtok(NULL,"/")
	){
		if (ci) {
			//assert(strncmp(ci->name,nd,NAME_MAX) == 0);
			ci=ci->down;
		}
		for(
			;
			ci && strncmp(ci->name,nd,NAME_MAX);
			ci=ci->next
		);		

	}

	return ci;	
}

/*!
http://www.opengroup.org/onlinepubs/009695399/
*/
int mknod(const char *filename, mode_t mode, dev_t dev){
	char path[PATH_MAX];
	char *name;
	tk_inode_t *belong;
	tk_inode_t *newNode;
	tk_inode_t *seekNode;
	int namelen;

	if (!(strnlen(filename,PATH_MAX) < PATH_MAX)){
		errno=ENAMETOOLONG;	//Breaking the naming rules - or something else is crashing...
		return -1;
	}

	igetpath(path,filename);
	name=igetname(filename);

	if (!((namelen=strnlen(name,NAME_MAX)) < NAME_MAX)){
		errno=ENAMETOOLONG;	//Breaking the naming rules - or something else is crashing...
		return -1;
	}

	if (belong=isearch(filename)){
		/*File with that name exists allready*/
		errno=EEXIST;
		return -1;
	}
	if (!(belong=isearch(path))){
		/*Owner path does not exist*/
		errno=ENOENT;
		return -1;
	}
	if (!(belong->mode | ISA_IFDIR)){
		errno=ENOTDIR;	/*Trying to create a node whos owner is not a directory*/
		return -1;
	}
	
	//Create the node
	assure(newNode = (tk_inode_t*)calloc(1,sizeof(tk_inode_t)));
	newNode->id=__icntr++;
	newNode->name=(char*)calloc(1,namelen);
	strncpy(newNode->name,name,namelen);
	newNode->mode=mode;
	newNode->iohandle=(tk_iohandle_t*)dev;

	//Attach the link upwards
	newNode->belong=belong;
	
	//Attach the link last in the list at the level just below (i.e. in this directory).	
	if (belong->down!=belong) {
		assert(belong->down);
		// Search the end of the list at the same level
		for (seekNode=belong->down;seekNode->next;seekNode=seekNode->next);
		//Attch it last
		seekNode->next=newNode;
	} else {
		//First entry in this directory
		belong->down=newNode;
        }
	//Shorthut newnode to self
	newNode->down=newNode;
	return 0;
}
