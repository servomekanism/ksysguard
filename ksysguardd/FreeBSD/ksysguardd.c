/*
    KSysGuard, the KDE Task Manager and Performance Monitor
   
	Copyright (c) 1999 Chris Schlaeger <cs@kde.org>
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

	$Id$
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>

#include "Dispatcher.h"
#include "Command.h"
#include "ProcessList.h"
#include "Memory.h"
#include "CPU.h"

#define CMDBUFSIZE 128

int QuitApp = 0;

static void
readCommand(char* cmdBuf)
{
	int i;
	int c;

	for (i = 0; i < CMDBUFSIZE - 1 && (c = getchar()) != '\n'; i++)
		cmdBuf[i] = (char) c;
	cmdBuf[i] = '\0';
	if (feof(stdin)) {
		printf("\nEOF processed, g'bye\n");
		memset(cmdBuf, 0, CMDBUFSIZE);
	}
}

/*
================================ public part ==================================
*/

void
exQuit(const char* cmd)
{
	QuitApp = 1;
}
	
int
main(int argc, const char* argv[])
{
	char cmdBuf[CMDBUFSIZE];

	initCommand();
	initDispatcher();
	registerCommand("quit", exQuit);
	initProcessList();
	initMemory();
	initCPU();

	while (!dispatcherReady())
		;

	printf("ksysguardd %s  (c) 1999, 2000 Chris Schlaeger <cs@kde.org>\n"
		   "This program may be distributed under the GPL.\n"
		   "ksysguardd> ", VERSION);
	fflush(stdout);
	do {
		readCommand(cmdBuf);
		if (!cmdBuf[0]) break;
		executeCommand(cmdBuf);
		printf("ksysguardd> ");
		fflush(stdout);
	} while (!QuitApp);

	exitCPU();
	exitMemory();
	exitProcessList();
	exitDispatcher();
	exitCommand();

	return (0);
}

