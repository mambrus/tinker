/***************************************************************************
 *   Copyright (C) 2006 by Michael Ambrus                                  *
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

/*!
@file
@ingroup kernel_reimpl_ansi

@brief Contains storage for error strings

Exit, error and termination handling.

Contains storage for errno, TinKer exit-code and TinKer error strings

For in-depth discussions about re-implemented error handling, see \ref
kernel_reimpl_ansi

@see PACKAGES
*/

#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <tk_tuning.h>
#if TK_HOSTED
#include <stdlib.h>
#endif

void perror(const char *message);	//Should go int stdio.h
char *strerror(int errnum);	//Should go int string.h

#define MAX_STR_LEN 64
//#define EC_LIGHT 1  //FIXME make this configurable. Perhaps the whole error handling (i.e. use GNU optionally)

#define EC_STRING_LIGHT( ec ) \
	#ec

#define EC_STRING_VERBOSE( ec ) \
	#ec " - " ESTR_##ec

#if defined(EC_LIGHT)
#define EC_STRING( x ) EC_STRING_LIGHT( x )
#else
#define EC_STRING( x ) EC_STRING_VERBOSE( x )
#endif

#if defined(TK_USE_EMRGCY_CONSOLE)
int TK_USE_EMRGCY_CONSOLE(const char *buffer, int buff_len);
#endif

static const char *errno_srings[] = {
	EC_STRING(TK_OK),
	EC_STRING(TK_ERROR),
	EC_STRING(ERR_TIMEOUT),
	EC_STRING(ERR_OBJDEL),
	EC_STRING(ERR_OBJID),
	EC_STRING(ERR_OBJTYPE),
	EC_STRING(ERR_OBJFULL),
	EC_STRING(ERR_BUFSIZ),
	EC_STRING(ERR_NOQCB),
	EC_STRING(ERR_FULL),
	EC_STRING(ERR_NOSCB),
	EC_STRING(ERR_NOSEM),
	EC_STRING(ERR_TATSDEL),
	EC_STRING(ERR_NOMEM),
	EC_STRING(ERR_BLOCKLIMIT),
	EC_STRING(ERR_UNDEF_PTIMER),
	EC_STRING(ERR_TIME_NEG),
	EC_STRING(ERR_TIMER_DELETED),
	EC_STRING(ERR_NO_MORE_TIMERS),
	EC_STRING(ERR_UNDEF_HEAPID),
	EC_STRING(EPERM),
	EC_STRING(ENOENT),
	EC_STRING(ESRCH),
	EC_STRING(EINTR),
	EC_STRING(EIO),
	EC_STRING(ENXIO),
	EC_STRING(E2BIG),
	EC_STRING(ENOEXEC),
	EC_STRING(EBADF),
	EC_STRING(ECHILD),
	EC_STRING(EDEADLK),
	EC_STRING(ENOMEM),
	EC_STRING(EACCES),
	EC_STRING(EFAULT),
	EC_STRING(ENOTBLK),
	EC_STRING(EBUSY),
	EC_STRING(EEXIST),
	EC_STRING(EXDEV),
	EC_STRING(ENODEV),
	EC_STRING(ENOTDIR),
	EC_STRING(EISDIR),
	EC_STRING(EINVAL),
	EC_STRING(EMFILE),
	EC_STRING(ENFILE),
	EC_STRING(ENOTTY),
	EC_STRING(ETXTBSY),
	EC_STRING(EFBIG),
	EC_STRING(ENOSPC),
	EC_STRING(ESPIPE),
	EC_STRING(EROFS),
	EC_STRING(EMLINK),
	EC_STRING(EPIPE),
	EC_STRING(EDOM),
	EC_STRING(ERANGE),
	EC_STRING(EAGAIN),
	EC_STRING(EWOULDBLOCK),
	EC_STRING(EINPROGRESS),
	EC_STRING(EALREADY),
	EC_STRING(ENOTSOCK),
	EC_STRING(EMSGSIZE),
	EC_STRING(EPROTOTYPE),
	EC_STRING(ENOPROTOOPT),
	EC_STRING(EPROTONOSUPPORT),
	EC_STRING(ESOCKTNOSUPPORT),
	EC_STRING(EOPNOTSUPP),
	EC_STRING(EPFNOSUPPORT),
	EC_STRING(EAFNOSUPPORT),
	EC_STRING(EADDRINUSE),
	EC_STRING(EADDRNOTAVAIL),
	EC_STRING(ENETDOWN),
	EC_STRING(ENETUNREACH),
	EC_STRING(ENETRESET),
	EC_STRING(ECONNABORTED),
	EC_STRING(ECONNRESET),
	EC_STRING(ENOBUFS),
	EC_STRING(EISCONN),
	EC_STRING(ENOTCONN),
	EC_STRING(EDESTADDRREQ),
	EC_STRING(ESHUTDOWN),
	EC_STRING(ETOOMANYREFS),
	EC_STRING(ETIMEDOUT),
	EC_STRING(ECONNREFUSED),
	EC_STRING(ELOOP),
	EC_STRING(ENAMETOOLONG),
	EC_STRING(EHOSTDOWN),
	EC_STRING(EHOSTUNREACH),
	EC_STRING(ENOTEMPTY),
	EC_STRING(EPROCLIM),
	EC_STRING(EUSERS),
	EC_STRING(EDQUOT),
	EC_STRING(ESTALE),
	EC_STRING(EREMOTE),
	EC_STRING(EBADRPC),
	EC_STRING(ERPCMISMATCH),
	EC_STRING(EPROGUNAVAIL),
	EC_STRING(EPROGMISMATCH),
	EC_STRING(EPROCUNAVAIL),
	EC_STRING(ENOLCK),
	EC_STRING(EFTYPE),
	EC_STRING(EAUTH),
	EC_STRING(ENEEDAUTH),
	EC_STRING(ENOSYS),
	EC_STRING(ENOTSUP),
	EC_STRING(EILSEQ),
	EC_STRING(EBACKGROUND),
	EC_STRING(EDIED),
	EC_STRING(ED),
	EC_STRING(EGREGIOUS),
	EC_STRING(EIEIO),
	EC_STRING(EGRATUITOUS),
	EC_STRING(EBADMSG),
	EC_STRING(EIDRM),
	EC_STRING(EMULTIHOP),
	EC_STRING(ENODATA),
	EC_STRING(ENOLINK),
	EC_STRING(ENOMSG),
	EC_STRING(ENOSR),
	EC_STRING(ENOSTR),
	EC_STRING(EOVERFLOW),
	EC_STRING(EPROTO),
	EC_STRING(ETIME),
	EC_STRING(ECANCELED),
	EC_STRING(ERESTART),
	EC_STRING(ECHRNG),
	EC_STRING(EL2NSYNC),
	EC_STRING(EL3HLT),
	EC_STRING(EL3RST),
	EC_STRING(ELNRNG),
	EC_STRING(EUNATCH),
	EC_STRING(ENOCSI),
	EC_STRING(EL2HLT),
	EC_STRING(EBADE),
	EC_STRING(EBADR),
	EC_STRING(EXFULL),
	EC_STRING(ENOANO),
	EC_STRING(EBADRQC),
	EC_STRING(EBADSLT),
	EC_STRING(EDEADLOCK),
	EC_STRING(EBFONT),
	EC_STRING(ENONET),
	EC_STRING(ENOPKG),
	EC_STRING(EADV),
	EC_STRING(ESRMNT),
	EC_STRING(ECOMM),
	EC_STRING(EDOTDOT),
	EC_STRING(ENOTUNIQ),
	EC_STRING(EBADFD),
	EC_STRING(EREMCHG),
	EC_STRING(ELIBACC),
	EC_STRING(ELIBBAD),
	EC_STRING(ELIBSCN),
	EC_STRING(ELIBMAX),
	EC_STRING(ELIBEXEC),
	EC_STRING(ESTRPIPE),
	EC_STRING(EUCLEAN),
	EC_STRING(ENOTNAM),
	EC_STRING(ENAVAIL),
	EC_STRING(EISNAM),
	EC_STRING(EREMOTEIO),
	EC_STRING(ENOMEDIUM),
	EC_STRING(EMEDIUMTYPE)
};

/*!
Trap-code strings
*/
#if (TK_HOWTO_PRINTK != TK_FNK_VOIDED)
static const char *trapcode_srings[] = {	// Bit number
	"No Trap Error",	// none
	"Total amount of threads would exceed limit",	// 0
	"Assertion failed",	// 1
	"Chosen priority too high",	// 2
	"Unknown",		// 3
	"Stack out of bounds check faliure",	// 4
	"Stack integrity faliure detected",	// 5
	"To many threads at this prio",	// 6
	"Kernel running amok detected",	// 7
	"Thread-name to long",	// 8
	"No memory left for allocation",	// 9
	"Invalid ETKB detected",	// 10
	"Unknown",		// 11
	"Unknown",		// 12
	"Unknown",		// 13
	"HW driver detected a fatal error",	// 14
	"Undefined termination reason (or reason not known to TinKer): "	// 15
};
#endif				//(TK_HOWTO_PRINTK != TK_FNK_VOIDED)

/*!
Helper function
A simple conversion from a number to string
*/
void _ntos(char *outst, int number, int maxlen)
{
	int i, div, d, j, a = 0;

	for (i = 0; i < maxlen; i++)
		outst[i] = 0;

	for (d = 10000000, i = 0, j = 0; d; i++, d /= 10) {
		div = number / d;
		if (a || div) {
			a = 1;
			outst[j] = '0' + div;
			number = number - (div * d);
			j++;
		}
	}
}

void perror(const char *message)
{
#if defined( __C166__ )
	char *ptr;

	unsigned long msgpti = (unsigned long)message;

	msgpti &= 0x0000FFFF;
	msgpti |= 0x00C00000;
	ptr = (char *)msgpti;

	printk((">>> TinKer Error: [%s] <<<\n", ptr));
#else
	printk((">>> TinKer Error: [%s] <<<\n", message));
#endif
}

char *strerror(int errnum)
{
	char *ptr;

	ptr = (char *)(errno_srings[errnum]);
	return ptr;
}

/*!
Generic trap handler - please note that certain targets that have support for
HW trapping might not have a working stack when reaching this far, in which
case this function has to be replaced. In either case, no calling other
functions, stack or heap operations are allowed from here on.
*/
void tk_trap(int ec)
{
	int i, m;

#if defined (TK_USE_EMRGCY_CONSOLE) && (TK_USE_EMRGCY_CONSOLE != __tk_no)

	static const char trp_txt[] = "tk: TRAP";
#define MAX_LEN 80

#if (TK_USE_EMRGCY_CONSOLE == __tk_yes)
	char astr[MAX_LEN];
	console_write(trp_txt, sizeof(trp_txt));
	console_write("\n\r", 2);
	if (ec == 0) {
		console_write(trapcode_srings[0], strlen(trapcode_srings[0]));
		console_write("\n\r", 2);
	} else if (!(ec & TC_ISA_ERR)) {
		console_write(trapcode_srings[16], strlen(trapcode_srings[16]));
		_ntos(astr, ec, MAX_LEN);
		console_write(astr, strlen(astr));
		console_write("\n\r", 2);
	} else {
		for (i = 0, m = 1; i < 15; i++, m = m << 1) {
			if (ec & m) {
				console_write("Bit #", 5);
				_ntos(astr, i, MAX_LEN);
				console_write(astr, strlen(astr));
				console_write(": ", 2);
				console_write(trapcode_srings[i + 1],
					      strlen(trapcode_srings[i + 1]));
				console_write("\n\r", 2);
			}
		}
	}
#else
	char astr[MAX_LEN];
	TK_USE_EMRGCY_CONSOLE(trp_txt, sizeof(trp_txt));
	TK_USE_EMRGCY_CONSOLE("\n\r", 2);
	if (ec == 0) {
		TK_USE_EMRGCY_CONSOLE(trapcode_srings[0],
				      strlen(trapcode_srings[0]));
		TK_USE_EMRGCY_CONSOLE("\n\r", 2);
	} else if (!(ec & TC_ISA_ERR)) {
		TK_USE_EMRGCY_CONSOLE(trapcode_srings[16],
				      strlen(trapcode_srings[16]));
		_ntos(astr, ec, MAX_LEN);
		TK_USE_EMRGCY_CONSOLE(astr, strlen(astr));
		TK_USE_EMRGCY_CONSOLE("\n\r", 2);
	} else {
		for (i = 0, m = 1; i < 15; i++, m = m << 1) {
			if (ec & m) {
				TK_USE_EMRGCY_CONSOLE("Bit #", 5);
				_ntos(astr, i, MAX_LEN);
				TK_USE_EMRGCY_CONSOLE(astr, strlen(astr));
				TK_USE_EMRGCY_CONSOLE(": ", 2);
				TK_USE_EMRGCY_CONSOLE(trapcode_srings[i + 1],
						      strlen(trapcode_srings
							     [i + 1]));
				TK_USE_EMRGCY_CONSOLE("\n\r", 2);
			}
		}
	}
#endif
#else
	printk(("tk: TRAP CODE [0x%04x]: \n", ec));
	if (ec == 0) {
		printk(("%s: \n", trapcode_srings[0]));
	} else if (!(ec & TC_ISA_ERR)) {
		printk(("%s: \n", trapcode_srings[16]));
		printk(("   => Application exist with exit-code: %d \n", ec));
	} else {
		for (i = 0, m = 1; i < 15; i++, m = m << 1) {
			if (ec & m) {
				printk((" - Bit #%d: %s\n", i + 1,
					trapcode_srings[i + 1]));
			}
		}
	}

#endif

#if defined(TK_COMP_FILESYS) && TK_COMP_FILESYS
	printk(("Trying to shut system down...\n"));
	fs_fini();
	fprintf(stderr, "System terminated!\n");
#endif

#if defined(__GNUC__) && TK_HOSTED
	exit(ec);
#else
	while (1) ;
#endif
}

/*!
if we have no-where else to go, we'll end up here
*/
#if TK_HOSTED
void _tk_dead(int ec)
{
	void _exit(int status);
	_exit(ec);
}
#else
void _tk_dead(int ec)
{
	while (1) ;
}
#endif

/*!
Last thing that is called when a thread gives up life either freely or when an
error of some sort happened (either kernel internal or user program specific).

In case of en error, this function also acts as a critical error-handler
entr point (critical = execution is deemed to stop).
*/
void tk_exit(int ec)
{
	static int lc = 0;
	if (lc++ > 0)
		_tk_dead(ec);
	if (ec == 0) {
		printk(("tk: Program terminated normally\n"));
	} else {
		printk(("tk: Warning - Program terminated with exit code [0x%04x] and errno [%d]\n", ec, errno));
		printk(("tk: errno   - %s\n", strerror(errno)));
	}
	while (1) {
		TRAP(ec);
	}
}

/*!
@ingroup kernel_glue

Works as the assert macro except that you have to use the __file_ and __line_
explicitly. Typically the assert macro will be defined to call this function
on targets that do not have assert implemented by TinKer.

*/
void _tk_assertfail(char *assertstr, char *filestr, int line)
{
#if defined(TK_USE_EMRGCY_CONSOLE)
	static const char *asrt_txt = "tk: Error - Assertion failed: ";
	static const char *file_txt = "file: ";
	static const char *line_txt = "line: ";
#define MAX_LEN 80
#if (TK_USE_EMRGCY_CONSOLE == __tk_yes)
	char astr[MAX_LEN];
	console_write(asrt_txt, strnlen(asrt_txt, MAX_LEN));

	console_write(assertstr, strlen(assertstr));
	console_write("\n\r", 2);

	console_write(file_txt, strlen(file_txt));
	console_write(filestr, strlen(filestr));
	console_write("\n\r", 2);

	console_write(line_txt, strlen(line_txt));
	_ntos(astr, line, MAX_LEN);
	console_write(astr, strlen(astr));
	console_write("\n\r", 2);
#else
	char astr[MAX_LEN];
	TK_USE_EMRGCY_CONSOLE(asrt_txt, strnlen(asrt_txt, MAX_LEN));

	TK_USE_EMRGCY_CONSOLE(assertstr, strlen(assertstr));
	TK_USE_EMRGCY_CONSOLE("\n\r", 2);

	TK_USE_EMRGCY_CONSOLE(file_txt, strlen(file_txt));
	TK_USE_EMRGCY_CONSOLE(filestr, strlen(filestr));
	TK_USE_EMRGCY_CONSOLE("\n\r", 2);

	TK_USE_EMRGCY_CONSOLE(line_txt, strlen(line_txt));
	_ntos(astr, line, MAX_LEN);
	TK_USE_EMRGCY_CONSOLE(astr, strlen(astr));
	TK_USE_EMRGCY_CONSOLE("\n\r", 2);

#endif
#else
	printk(("tk: Error - Assertion failed: %s,\nfile: %s,\nline: %d\n",
		assertstr, filestr, line));
#endif
	tk_exit(TC_ERR_ASSERT);
}
