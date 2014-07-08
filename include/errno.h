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
#ifndef ERRNO_H_TK
#define ERRNO_H_TK

#if defined(errno)
#undef errno
#endif

#include <tinker/config.h>

#if defined(__cplusplus) && (TK_CPLUSPLUS==0)
extern "C" {
#endif

	int *_tk_errno();

#if defined(__cplusplus) && (TK_CPLUSPLUS==0)
}
#endif
#define errno (*_tk_errno())

#define DEF_EC(ec) \
 ec ANSI_#ec

#define DEF_ES(ec,estr)

#define ECODES_ONLY

#include <tk_mem.h>

enum ANSI_ERROR_CODES {
	ANSI_EPERM = ERR_KMEM_SENTINEL,
	ANSI_ENOENT,
	ANSI_ESRCH,
	ANSI_EINTR,
	ANSI_EIO,
	ANSI_ENXIO,
	ANSI_E2BIG,
	ANSI_ENOEXEC,
	ANSI_EBADF,
	ANSI_ECHILD,
	ANSI_EDEADLK,
	ANSI_ENOMEM,
	ANSI_EACCES,
	ANSI_EFAULT,
	ANSI_ENOTBLK,
	ANSI_EBUSY,
	ANSI_EEXIST,
	ANSI_EXDEV,
	ANSI_ENODEV,
	ANSI_ENOTDIR,
	ANSI_EISDIR,
	ANSI_EINVAL,
	ANSI_EMFILE,
	ANSI_ENFILE,
	ANSI_ENOTTY,
	ANSI_ETXTBSY,
	ANSI_EFBIG,
	ANSI_ENOSPC,
	ANSI_ESPIPE,
	ANSI_EROFS,
	ANSI_EMLINK,
	ANSI_EPIPE,
	ANSI_EDOM,
	ANSI_ERANGE,
	ANSI_EAGAIN,
	ANSI_EWOULDBLOCK,
	ANSI_EINPROGRESS,
	ANSI_EALREADY,
	ANSI_ENOTSOCK,
	ANSI_EMSGSIZE,
	ANSI_EPROTOTYPE,
	ANSI_ENOPROTOOPT,
	ANSI_EPROTONOSUPPORT,
	ANSI_ESOCKTNOSUPPORT,
	ANSI_EOPNOTSUPP,
	ANSI_EPFNOSUPPORT,
	ANSI_EAFNOSUPPORT,
	ANSI_EADDRINUSE,
	ANSI_EADDRNOTAVAIL,
	ANSI_ENETDOWN,
	ANSI_ENETUNREACH,
	ANSI_ENETRESET,
	ANSI_ECONNABORTED,
	ANSI_ECONNRESET,
	ANSI_ENOBUFS,
	ANSI_EISCONN,
	ANSI_ENOTCONN,
	ANSI_EDESTADDRREQ,
	ANSI_ESHUTDOWN,
	ANSI_ETOOMANYREFS,
	ANSI_ETIMEDOUT,
	ANSI_ECONNREFUSED,
	ANSI_ELOOP,
	ANSI_ENAMETOOLONG,
	ANSI_EHOSTDOWN,
	ANSI_EHOSTUNREACH,
	ANSI_ENOTEMPTY,
	ANSI_EPROCLIM,
	ANSI_EUSERS,
	ANSI_EDQUOT,
	ANSI_ESTALE,
	ANSI_EREMOTE,
	ANSI_EBADRPC,
	ANSI_ERPCMISMATCH,
	ANSI_EPROGUNAVAIL,
	ANSI_EPROGMISMATCH,
	ANSI_EPROCUNAVAIL,
	ANSI_ENOLCK,
	ANSI_EFTYPE,
	ANSI_EAUTH,
	ANSI_ENEEDAUTH,
	ANSI_ENOSYS,
	ANSI_ENOTSUP,
	ANSI_EILSEQ,
	ANSI_EBACKGROUND,
	ANSI_EDIED,
	ANSI_ED,
	ANSI_EGREGIOUS,
	ANSI_EIEIO,
	ANSI_EGRATUITOUS,
	ANSI_EBADMSG,
	ANSI_EIDRM,
	ANSI_EMULTIHOP,
	ANSI_ENODATA,
	ANSI_ENOLINK,
	ANSI_ENOMSG,
	ANSI_ENOSR,
	ANSI_ENOSTR,
	ANSI_EOVERFLOW,
	ANSI_EPROTO,
	ANSI_ETIME,
	ANSI_ECANCELED,
	ANSI_ERESTART,
	ANSI_ECHRNG,
	ANSI_EL2NSYNC,
	ANSI_EL3HLT,
	ANSI_EL3RST,
	ANSI_ELNRNG,
	ANSI_EUNATCH,
	ANSI_ENOCSI,
	ANSI_EL2HLT,
	ANSI_EBADE,
	ANSI_EBADR,
	ANSI_EXFULL,
	ANSI_ENOANO,
	ANSI_EBADRQC,
	ANSI_EBADSLT,
	ANSI_EDEADLOCK,
	ANSI_EBFONT,
	ANSI_ENONET,
	ANSI_ENOPKG,
	ANSI_EADV,
	ANSI_ESRMNT,
	ANSI_ECOMM,
	ANSI_EDOTDOT,
	ANSI_ENOTUNIQ,
	ANSI_EBADFD,
	ANSI_EREMCHG,
	ANSI_ELIBACC,
	ANSI_ELIBBAD,
	ANSI_ELIBSCN,
	ANSI_ELIBMAX,
	ANSI_ELIBEXEC,
	ANSI_ESTRPIPE,
	ANSI_EUCLEAN,
	ANSI_ENOTNAM,
	ANSI_ENAVAIL,
	ANSI_EISNAM,
	ANSI_EREMOTEIO,
	ANSI_ENOMEDIUM,
	ANSI_EMEDIUMTYPE
};
#define EPERM ANSI_EPERM
#define ESTR_ANSI_EPERM \
"Operation not permitted; only the owner of the file (or other resource) or priv processes allowed. "

#define ENOENT ANSI_ENOENT
#define ESTR_ANSI_ENOENT \
"No such file or directory. "

#define ESRCH ANSI_ESRCH
#define ESTR_ANSI_ESRCH \
"No process matches the specified process ID.  "

#define EINTR ANSI_EINTR
#define ESTR_ANSI_EINTR \
"Interrupted function call; "

#define EIO ANSI_EIO
#define ESTR_ANSI_EIO \
"Input/output error; usually used for physical read or write errors.  "

#define ENXIO ANSI_ENXIO
#define ESTR_ANSI_ENXIO \
"No such device or address. The system tried to use the device represented by a file you specified, and it couldn't find the device. This can mean that the device file was installed incorrectly, or that the physical device is missing or not correctly attached to the computer.  "

#define E2BIG ANSI_E2BIG
#define ESTR_ANSI_E2BIG \
"Argument list too long; used when the arguments passed to a new program being executed with one of the exec functions (see Executing a File) occupy too much memory space. This condition never arises in the GNU system."

#define ENOEXEC ANSI_ENOEXEC
#define ESTR_ANSI_ENOEXEC \
"Invalid executable file format. This condition is detected by the exec functions; see Executing a File.  "

#define EBADF ANSI_EBADF
#define ESTR_ANSI_EBADF \
"Bad file descriptor; for example, I/O on a descriptor that has been closed or reading from a descriptor open only for writing (or vice versa).  "

#define ECHILD ANSI_ECHILD
#define ESTR_ANSI_ECHILD \
"There are no child processes. This error happens on operations that are supposed to manipulate child processes, when there aren't any processes to manipulate.  "

#define EDEADLK ANSI_EDEADLK
#define ESTR_ANSI_EDEADLK \
"Deadlock avoided; allocating a system resource would have resulted in a deadlock situation. The system does not guarantee that it will notice all such situations. This error means you got lucky and the system noticed; it might just hang. See File Locks, for an example.  "

#define ENOMEM ANSI_ENOMEM
#define ESTR_ANSI_ENOMEM \
"No memory available. The system cannot allocate more virtual memory because its capacity is full.  "

#define EACCES ANSI_EACCES
#define ESTR_ANSI_EACCES \
"Permission denied; the file permissions do not allow the attempted operation.  "

#define EFAULT ANSI_EFAULT
#define ESTR_ANSI_EFAULT \
"Bad address; an invalid pointer was detected. In the GNU system, this error never happens; you get a signal instead.  "

#define ENOTBLK ANSI_ENOTBLK
#define ESTR_ANSI_ENOTBLK \
"A file that isn't a block special file was given in a situation that requires one. For example, trying to mount an ordinary file as a file system in Unix gives this error.  "

#define EBUSY ANSI_EBUSY
#define ESTR_ANSI_EBUSY \
"Resource busy; a system resource that can't be shared is already in use. For example, if you try to delete a file that is the root of a currently mounted filesystem, you get this error.  "

#define EEXIST ANSI_EEXIST
#define ESTR_ANSI_EEXIST \
"File exists; an existing file was specified in a context where it only makes sense to specify a new file.  "

#define EXDEV ANSI_EXDEV
#define ESTR_ANSI_EXDEV \
"An attempt to make an improper link across file systems was detected. This happens not only when you use link (see Hard Links) but also when you rename a file with rename (see Renaming Files).  "

#define ENODEV ANSI_ENODEV
#define ESTR_ANSI_ENODEV \
"The wrong type of device was given to a function that expects a particular sort of device.  "

#define ENOTDIR ANSI_ENOTDIR
#define ESTR_ANSI_ENOTDIR \
"A file that isn't a directory was specified when a directory is required.  "

#define EISDIR ANSI_EISDIR
#define ESTR_ANSI_EISDIR \
"File is a directory; you cannot open a directory for writing, or create or remove hard links to it.  "

#define EINVAL ANSI_EINVAL
#define ESTR_ANSI_EINVAL \
"Invalid argument. This is used to indicate various kinds of problems with passing the wrong argument to a library function.  "

#define EMFILE ANSI_EMFILE
#define ESTR_ANSI_EMFILE \
"The current process has too many files open and can't open any more."

#define ENFILE ANSI_ENFILE
#define ESTR_ANSI_ENFILE \
"There are too many distinct file openings in the entire system. Note that any number of linked channels count as just one file opening; see Linked Channels. This error never occurs in the GNU system.  "

#define ENOTTY ANSI_ENOTTY
#define ESTR_ANSI_ENOTTY \
"Inappropriate I/O control operation, such as trying to set terminal modes on an ordinary file.  "

#define ETXTBSY ANSI_ETXTBSY
#define ESTR_ANSI_ETXTBSY \
"An attempt to execute a file that is currently open for writing, or write to a file that is currently being executed."

#define EFBIG ANSI_EFBIG
#define ESTR_ANSI_EFBIG \
"File too big; the size of a file would be larger than allowed by the system.  "

#define ENOSPC ANSI_ENOSPC
#define ESTR_ANSI_ENOSPC \
"No space left on device; write operation on a file failed because the disk is full.  "

#define ESPIPE ANSI_ESPIPE
#define ESTR_ANSI_ESPIPE \
"Invalid seek operation (such as on a pipe).  "

#define EROFS ANSI_EROFS
#define ESTR_ANSI_EROFS \
"An attempt was made to modify something on a read-only file system.  "

#define EMLINK ANSI_EMLINK
#define ESTR_ANSI_EMLINK \
"Too many links; the link count of a single file would become too large. rename can cause this error if the file being renamed already has as many links as it can take (see Renaming Files).  "

#define EPIPE ANSI_EPIPE
#define ESTR_ANSI_EPIPE \
"Broken pipe; there is no process reading from the other end of a pipe. Every library function that returns this error code also generates a SIGPIPE signal; this signal terminates the program if not handled or blocked. Thus, your program will never actually see EPIPE unless it has handled or blocked SIGPIPE.  "

#define EDOM ANSI_EDOM
#define ESTR_ANSI_EDOM \
"Domain error; used by mathematical functions when an argument value does not fall into the domain over which the function is defined.  "

#define ERANGE ANSI_ERANGE
#define ESTR_ANSI_ERANGE \
"Range error; used by mathematical functions when the result value is not representable because of overflow or underflow.  "

#define EAGAIN ANSI_EAGAIN
#define ESTR_ANSI_EAGAIN \
"Resource temporarily unavailable; the call might work if you try again later."
#define EWOULDBLOCK ANSI_EWOULDBLOCK
#define ESTR_ANSI_EWOULDBLOCK \
"Resource temporarily unavailable; the call might work if you try again later."

#define EINPROGRESS ANSI_EINPROGRESS
#define ESTR_ANSI_EINPROGRESS \
"An operation that cannot complete immediately was initiated on an object that has non-blocking mode selected. Some functions that must always block (such as connect; see Connecting) never return EAGAIN. Instead, they return EINPROGRESS to indicate that the operation has begun and will take some time. Attempts to manipulate the object before the call completes return EALREADY. You can use the select function to find out when the pending operation has completed; see Waiting for I/O.  "

#define EALREADY ANSI_EALREADY
#define ESTR_ANSI_EALREADY \
"An operation is already in progress on an object that has non-blocking mode selected.  "

#define ENOTSOCK ANSI_ENOTSOCK
#define ESTR_ANSI_ENOTSOCK \
"A file that isn't a socket was specified when a socket is required.  "

#define EMSGSIZE ANSI_EMSGSIZE
#define ESTR_ANSI_EMSGSIZE \
"The size of a message sent on a socket was larger than the supported maximum size.  "

#define EPROTOTYPE ANSI_EPROTOTYPE
#define ESTR_ANSI_EPROTOTYPE \
"The socket type does not support the requested communications protocol.  "

#define ENOPROTOOPT ANSI_ENOPROTOOPT
#define ESTR_ANSI_ENOPROTOOPT \
"You specified a socket option that doesn't make sense for the particular protocol being used by the socket. See Socket Options.  "

#define EPROTONOSUPPORT ANSI_EPROTONOSUPPORT
#define ESTR_ANSI_EPROTONOSUPPORT \
"The socket domain does not support the requested communications protocol (perhaps because the requested protocol is completely invalid). See Creating a Socket.  "

#define ESOCKTNOSUPPORT ANSI_ESOCKTNOSUPPORT
#define ESTR_ANSI_ESOCKTNOSUPPORT \
"The socket type is not supported.  "

#define EOPNOTSUPP ANSI_EOPNOTSUPP
#define ESTR_ANSI_EOPNOTSUPP \
"The operation you requested is not supported. Some socket functions don't make sense for all types of sockets, and others may not be implemented for all communications protocols. In the GNU system, this error can happen for many calls when the object does not support the particular operation; it is a generic indication that the server knows nothing to do for that call.  "

#define EPFNOSUPPORT ANSI_EPFNOSUPPORT
#define ESTR_ANSI_EPFNOSUPPORT \
"The socket communications protocol family you requested is not supported.  "

#define EAFNOSUPPORT ANSI_EAFNOSUPPORT
#define ESTR_ANSI_EAFNOSUPPORT \
"The address family specified for a socket is not supported; it is inconsistent with the protocol being used on the socket. See Sockets.  "

#define EADDRINUSE ANSI_EADDRINUSE
#define ESTR_ANSI_EADDRINUSE \
"The requested socket address is already in use. See Socket Addresses.  "

#define EADDRNOTAVAIL ANSI_EADDRNOTAVAIL
#define ESTR_ANSI_EADDRNOTAVAIL \
"The requested socket address is not available; for example, you tried to give a socket a name that doesn't match the local host name. See Socket Addresses.  "

#define ENETDOWN ANSI_ENETDOWN
#define ESTR_ANSI_ENETDOWN \
"A socket operation failed because the network was down.  "

#define ENETUNREACH ANSI_ENETUNREACH
#define ESTR_ANSI_ENETUNREACH \
"A socket operation failed because the subnet containing the remote host was unreachable.  "

#define ENETRESET ANSI_ENETRESET
#define ESTR_ANSI_ENETRESET \
"A network connection was reset because the remote host crashed.  "

#define ECONNABORTED ANSI_ECONNABORTED
#define ESTR_ANSI_ECONNABORTED \
"A network connection was aborted locally.  "

#define ECONNRESET ANSI_ECONNRESET
#define ESTR_ANSI_ECONNRESET \
"A network connection was closed for reasons outside the control of the local host, such as by the remote machine rebooting or an unrecoverable protocol violation.  "

#define ENOBUFS ANSI_ENOBUFS
#define ESTR_ANSI_ENOBUFS \
"The kernel's buffers for I/O operations are all in use. In GNU, this error is always synonymous with ENOMEM; you may get one or the other from network operations.  "

#define EISCONN ANSI_EISCONN
#define ESTR_ANSI_EISCONN \
"You tried to connect a socket that is already connected. See Connecting.  "

#define ENOTCONN ANSI_ENOTCONN
#define ESTR_ANSI_ENOTCONN \
"The socket is not connected to anything. You get this error when you try to transmit data over a socket, without first specifying a destination for the data. For a connectionless socket (for datagram protocols, such as UDP), you get EDESTADDRREQ instead.  "

#define EDESTADDRREQ ANSI_EDESTADDRREQ
#define ESTR_ANSI_EDESTADDRREQ \
"No default destination address was set for the socket. You get this error when you try to transmit data over a connectionless socket, without first specifying a destination for the data with connect.  "

#define ESHUTDOWN ANSI_ESHUTDOWN
#define ESTR_ANSI_ESHUTDOWN \
"The socket has already been shut down.  "

#define ETOOMANYREFS ANSI_ETOOMANYREFS
#define ESTR_ANSI_ETOOMANYREFS \
"???  "

#define ETIMEDOUT ANSI_ETIMEDOUT
#define ESTR_ANSI_ETIMEDOUT \
"A socket operation with a specified timeout received no response during the timeout period.  "

#define ECONNREFUSED ANSI_ECONNREFUSED
#define ESTR_ANSI_ECONNREFUSED \
"A remote host refused to allow the network connection (typically because it is not running the requested service).  "

#define ELOOP ANSI_ELOOP
#define ESTR_ANSI_ELOOP \
"Too many levels of symbolic links were encountered in looking up a file name. This often indicates a cycle of symbolic links.  "

#define ENAMETOOLONG ANSI_ENAMETOOLONG
#define ESTR_ANSI_ENAMETOOLONG \
"Filename too long (longer than PATH_MAX; see Limits for Files) or host name too long (in gethostname or sethostname; see Host Identification).  "

#define EHOSTDOWN ANSI_EHOSTDOWN
#define ESTR_ANSI_EHOSTDOWN \
"The remote host for a requested network connection is down.  "

#define EHOSTUNREACH ANSI_EHOSTUNREACH
#define ESTR_ANSI_EHOSTUNREACH \
"The remote host for a requested network connection is not reachable.  "

#define ENOTEMPTY ANSI_ENOTEMPTY
#define ESTR_ANSI_ENOTEMPTY \
"Directory not empty, where an empty directory was expected. Typically, this error occurs when you are trying to delete a directory.  "

#define EPROCLIM ANSI_EPROCLIM
#define ESTR_ANSI_EPROCLIM \
"This means that the per-user limit on new process would be exceeded by an attempted fork. See Limits on Resources, for details on the RLIMIT_NPROC limit.  "

#define EUSERS ANSI_EUSERS
#define ESTR_ANSI_EUSERS \
"The file quota system is confused because there are too many users.  "

#define EDQUOT ANSI_EDQUOT
#define ESTR_ANSI_EDQUOT \
"The user's disk quota was exceeded.  "

#define ESTALE ANSI_ESTALE
#define ESTR_ANSI_ESTALE \
"Stale NFS file handle. This indicates an internal confusion in the NFS system which is due to file system rearrangements on the server host. Repairing this condition usually requires unmounting and remounting the NFS file system on the local host.  "

#define EREMOTE ANSI_EREMOTE
#define ESTR_ANSI_EREMOTE \
"An attempt was made to NFS-mount a remote file system with a file name that already specifies an NFS-mounted file. (This is an error on some operating systems, but we expect it to work properly on the GNU system, making this error code impossible.)  "

#define EBADRPC ANSI_EBADRPC
#define ESTR_ANSI_EBADRPC \
"???  "

#define ERPCMISMATCH ANSI_ERPCMISMATCH
#define ESTR_ANSI_ERPCMISMATCH \
"???  "

#define EPROGUNAVAIL ANSI_EPROGUNAVAIL
#define ESTR_ANSI_EPROGUNAVAIL \
"???  "

#define EPROGMISMATCH ANSI_EPROGMISMATCH
#define ESTR_ANSI_EPROGMISMATCH \
"???  "

#define EPROCUNAVAIL ANSI_EPROCUNAVAIL
#define ESTR_ANSI_EPROCUNAVAIL \
"???  "

#define ENOLCK ANSI_ENOLCK
#define ESTR_ANSI_ENOLCK \
"No locks available. This is used by the file locking facilities; see File Locks. This error is never generated by the GNU system, but it can result from an operation to an NFS server running another operating system.  "

#define EFTYPE ANSI_EFTYPE
#define ESTR_ANSI_EFTYPE \
"Inappropriate file type or format. The file was the wrong type for the operation, or a data file had the wrong format. "

#define EAUTH ANSI_EAUTH
#define ESTR_ANSI_EAUTH \
"???  "

#define ENEEDAUTH ANSI_ENEEDAUTH
#define ESTR_ANSI_ENEEDAUTH \
"???  "

#define ENOSYS ANSI_ENOSYS
#define ESTR_ANSI_ENOSYS \
"Function not implemented. This indicates that the function called is not implemented at all, either in the C library itself or in the operating system. When you get this error, you can be sure that this particular function will always fail with ENOSYS unless you install a new version of the C library or the operating system.  "

#define ENOTSUP ANSI_ENOTSUP
#define ESTR_ANSI_ENOTSUP \
"Not supported. A function returns this error when certain parameter values are valid, but the functionality they request is not available. This can mean that the function does not implement a particular command or option value or flag bit at all. For functions that operate on some object given in a parameter, such as a file descriptor or a port, it might instead mean that only that specific object (file descriptor, port, etc.) is unable to support the other parameters given; different file descriptors might support different ranges of parameter values."

#define EILSEQ ANSI_EILSEQ
#define ESTR_ANSI_EILSEQ \
"While decoding a multibyte character the function came along an invalid or an incomplete sequence of bytes or the given wide character is invalid.  "

#define EBACKGROUND ANSI_EBACKGROUND
#define ESTR_ANSI_EBACKGROUND \
"In the GNU system, servers supporting the term protocol return this error for certain operations when the caller is not in the foreground process group of the terminal. Users do not usually see this error because functions such as read and write translate it into a SIGTTIN or SIGTTOU signal. See Job Control, for information on process groups and these signals.  "

#define EDIED ANSI_EDIED
#define ESTR_ANSI_EDIED \
"In the GNU system, opening a file returns this error when the file is translated by a program and the translator program dies while starting up, before it has connected to the file.  "

#define ED ANSI_ED
#define ESTR_ANSI_ED \
"The experienced user will know what is wrong.  "

#define EGREGIOUS ANSI_EGREGIOUS
#define ESTR_ANSI_EGREGIOUS \
"You did what?  "

#define EIEIO ANSI_EIEIO
#define ESTR_ANSI_EIEIO \
"Go home and have a glass of warm, dairy-fresh milk.  "

#define EGRATUITOUS ANSI_EGRATUITOUS
#define ESTR_ANSI_EGRATUITOUS \
"This error code has no purpose.  "

#define EBADMSG ANSI_EBADMSG
#define ESTR_ANSI_EBADMSG \
"TBD"

#define EIDRM ANSI_EIDRM
#define ESTR_ANSI_EIDRM \
"TBD"

#define EMULTIHOP ANSI_EMULTIHOP
#define ESTR_ANSI_EMULTIHOP \
"TBD"

#define ENODATA ANSI_ENODATA
#define ESTR_ANSI_ENODATA \
"TBD"

#define ENOLINK ANSI_ENOLINK
#define ESTR_ANSI_ENOLINK \
"TBD"

#define ENOMSG ANSI_ENOMSG
#define ESTR_ANSI_ENOMSG \
"TBD"

#define ENOSR ANSI_ENOSR
#define ESTR_ANSI_ENOSR \
"TBD"

#define ENOSTR ANSI_ENOSTR
#define ESTR_ANSI_ENOSTR \
"TBD"

#define EOVERFLOW ANSI_EOVERFLOW
#define ESTR_ANSI_EOVERFLOW \
"TBD"

#define EPROTO ANSI_EPROTO
#define ESTR_ANSI_EPROTO \
"TBD"

#define ETIME ANSI_ETIME
#define ESTR_ANSI_ETIME \
"TBD"

#define ECANCELED ANSI_ECANCELED
#define ESTR_ANSI_ECANCELED \
"Operation canceled; an asynchronous operation was canceled before it completed."

#define ERESTART ANSI_ERESTART
#define ESTR_ANSI_ERESTART \
"TBD"

#define ECHRNG ANSI_ECHRNG
#define ESTR_ANSI_ECHRNG \
"TBD"

#define EL2NSYNC ANSI_EL2NSYNC
#define ESTR_ANSI_EL2NSYNC \
"TBD"

#define EL3HLT ANSI_EL3HLT
#define ESTR_ANSI_EL3HLT \
"TBD"

#define EL3RST ANSI_EL3RST
#define ESTR_ANSI_EL3RST \
"TBD"

#define ELNRNG ANSI_ELNRNG
#define ESTR_ANSI_ELNRNG \
"TBD"

#define EUNATCH ANSI_EUNATCH
#define ESTR_ANSI_EUNATCH \
"TBD"

#define ENOCSI ANSI_ENOCSI
#define ESTR_ANSI_ENOCSI \
"TBD"

#define EL2HLT ANSI_EL2HLT
#define ESTR_ANSI_EL2HLT \
"TBD"

#define EBADE ANSI_EBADE
#define ESTR_ANSI_EBADE \
"TBD"

#define EBADR ANSI_EBADR
#define ESTR_ANSI_EBADR \
"TBD"

#define EXFULL ANSI_EXFULL
#define ESTR_ANSI_EXFULL \
"TBD"

#define ENOANO ANSI_ENOANO
#define ESTR_ANSI_ENOANO \
"TBD"

#define EBADRQC ANSI_EBADRQC
#define ESTR_ANSI_EBADRQC \
"TBD"

#define EBADSLT ANSI_EBADSLT
#define ESTR_ANSI_EBADSLT \
"TBD"

#define EDEADLOCK ANSI_EDEADLOCK
#define ESTR_ANSI_EDEADLOCK \
"TBD"

#define EBFONT ANSI_EBFONT
#define ESTR_ANSI_EBFONT \
"TBD"

#define ENONET ANSI_ENONET
#define ESTR_ANSI_ENONET \
"TBD"

#define ENOPKG ANSI_ENOPKG
#define ESTR_ANSI_ENOPKG \
"TBD"

#define EADV ANSI_EADV
#define ESTR_ANSI_EADV \
"TBD"

#define ESRMNT ANSI_ESRMNT
#define ESTR_ANSI_ESRMNT \
"TBD"

#define ECOMM ANSI_ECOMM
#define ESTR_ANSI_ECOMM \
"TBD"

#define EDOTDOT ANSI_EDOTDOT
#define ESTR_ANSI_EDOTDOT \
"TBD"

#define ENOTUNIQ ANSI_ENOTUNIQ
#define ESTR_ANSI_ENOTUNIQ \
"TBD"

#define EBADFD ANSI_EBADFD
#define ESTR_ANSI_EBADFD \
"TBD"

#define EREMCHG ANSI_EREMCHG
#define ESTR_ANSI_EREMCHG \
"TBD"

#define ELIBACC ANSI_ELIBACC
#define ESTR_ANSI_ELIBACC \
"TBD"

#define ELIBBAD ANSI_ELIBBAD
#define ESTR_ANSI_ELIBBAD \
"TBD"

#define ELIBSCN ANSI_ELIBSCN
#define ESTR_ANSI_ELIBSCN \
"TBD"

#define ELIBMAX ANSI_ELIBMAX
#define ESTR_ANSI_ELIBMAX \
"TBD"

#define ELIBEXEC ANSI_ELIBEXEC
#define ESTR_ANSI_ELIBEXEC \
"TBD"

#define ESTRPIPE ANSI_ESTRPIPE
#define ESTR_ANSI_ESTRPIPE \
"TBD"

#define EUCLEAN ANSI_EUCLEAN
#define ESTR_ANSI_EUCLEAN \
"TBD"

#define ENOTNAM ANSI_ENOTNAM
#define ESTR_ANSI_ENOTNAM \
"TBD"

#define ENAVAIL ANSI_ENAVAIL
#define ESTR_ANSI_ENAVAIL \
"TBD"

#define EISNAM ANSI_EISNAM
#define ESTR_ANSI_EISNAM \
"TBD"

#define EREMOTEIO ANSI_EREMOTEIO
#define ESTR_ANSI_EREMOTEIO \
"TBD"

#define ENOMEDIUM ANSI_ENOMEDIUM
#define ESTR_ANSI_ENOMEDIUM \
"TBD"

#define EMEDIUMTYPE ANSI_EMEDIUMTYPE
#define ESTR_ANSI_EMEDIUMTYPE \
"TBD"

#define ESTR_TK_OK "Operation returned without errors"
#define ESTR_TK_ERROR "General TinKer catch all error"
#define ESTR_ERR_TIMEOUT "ITC has timedout"
#define ESTR_ERR_OBJDEL "ITC has been deleted"
#define ESTR_ERR_OBJID "ITC id incorrect ??"
#define ESTR_ERR_OBJTYPE "ITC type doesn'e mach object id"
#define ESTR_ERR_OBJFULL "Nodes objecttable is full"
#define ESTR_ERR_BUFSIZ "The allocated buffer is smaller than a message could be"
#define ESTR_ERR_NOQCB "Exceeds max numer of queues"
#define ESTR_ERR_FULL "The message buffer is full"
#define ESTR_ERR_NOSCB "Exceeds max numer of semaphores"
#define ESTR_ERR_NOSEM "Only if NOWAIT was selected"
#define ESTR_ERR_TATSDEL "There were threads waiting"
#define ESTR_ERR_NOMEM "No more memory"
#define ESTR_ERR_BLOCKLIMIT "Can't block more procs on queue or semaphore"
#define ESTR_ERR_UNDEF_PTIMER "No such or invalid ptimer"
#define ESTR_ERR_TIME_NEG "Trying to set event for time that's passed"
#define ESTR_ERR_TIMER_DELETED "The timer (i.e. the requested event) has been deleted"
#define ESTR_ERR_NO_MORE_TIMERS "You try to exeed the maximum number of pending timers"
#define ESTR_ERR_UNDEF_HEAPID "No such or invalid heap"

#endif
