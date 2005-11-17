/*******************************************************************
 *
 *  DESCRIPTION: tk.h Headerfile for TinKerOs
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.1 $
 *
 *  $Log: tk.h,v $
 *  Revision 1.1  2005-11-17 09:59:09  ambrmi09
 *  Initial revision
 *
 *  Revision 1.5  1998/02/16 18:05:12  mickey
 *  Added macro assert.
 *   Added extra wakeup event
 *
 *  Revision 1.4  1998/02/14 10:27:45  mickey
 *  Handling of wakeup event
 *  void tk_exit( unsigned int ) replaces void exit( unsigned int)
 *
 *  Revision 1.3  1998/02/01 20:03:28  mickey
 *  Jiihaa, first version of a working semaphore
 *
 *  Revision 1.2  1998/01/28 21:32:26  mickey
 *  First working revision after tidying, with both Borland and Codewright IDE
 *
 *  Revision 1.1  1998/01/28 20:12:41  mickey
 *  Initial revision
 *
 *  
 *******************************************************************/
 
#ifndef tk_h
#define tk_h

/** include files **/
#include <time.h>

/** local definitions **/
#define max_procs 			25
#define max_prio_levels 	0x10  //fix this, Idle needs to bee last in last prio( needs one extra )
#define max_procs_at_prio 	0x8
#define proc_name_len 		0x08

//Some defines for the logic
#define TK_OK 		0
#define TK_ERROR    1
#define YES			1
#define NO			0

//Include components 
#define	IPC			YES

//assert macro
#ifdef NDEBUG
#  define assert(p)   ((void)0)
#else
#  
#  define assert(p) ((p) ? (void)0 : (void) __tk_assertfail( \
                    #p, __FILE__, __LINE__ ) )					
#endif


typedef enum{FALSE,TRUE}BOOL;
typedef unsigned int funk(void *);
typedef funk *funk_p;

// The following 2 defines the process status and on what it is blocked.
// __T = TERM  = Process is waiting for one or more children to terminate
// _S_ = SLEEP = Process is blocked on timer (sleeping)
// Q__ = QUEUE = Process is blocked on queue or semafor
typedef enum {
	READY=0x0,		_______T=0x1,	______S_=0x2,	______ST=0x3,
	_____Q__=0x4,	_____Q_T=0x5,	_____QS_=0x6,	_____QST=0x7,
	ZOMBIE=0X80
}PROCSTATE;

typedef enum{TERM=1,SLEEP=2,QUEUE=4}STATEBITS;
typedef enum{E_CHILDDEATH, E_TIMER, E_IPC, E_IPC2}wakeE_t;

typedef struct proc_t_s{
	unsigned int	Pid,Gid;				//Process ID and Parent ID (Gid)
	unsigned int	noChilds;				//Numb of procs this has created
	char			name[proc_name_len];	//Name of the process
	BOOL			isInit;					//Memory for stack is allocated
	PROCSTATE		state;					//State of the process
	char			*stack;					//Bottom of stack
	char			*sp;					//Top of stack
	size_t			stack_size;				//Size of stack
	clock_t  		wakeuptime;				//When to wake up if sleeping
	wakeE_t			wakeupEvent;			//Helper variable mainly for IPC
	unsigned int	Prio,Idx;				//Helpers, prevent need of lookup
}proc_t;
/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

void tk_main( void );

void runTask(unsigned int RID,unsigned int SID);
int deleteTask(unsigned int PID);
unsigned int createTask(
   char *name,
   unsigned int prio,
   funk_p f,
   void *inpar,
   size_t stack_size
);
//Allocate at least 2k stack if you use printf in task
//else 64bytes is probably sufficent.
void createKern( void );
void deleteKern( void );
void schedul( void );
void tk_exit( int ec );
unsigned int sleep( unsigned int time );
unsigned int MyPid( void );				
proc_t *MyProc_p( void );/* Gives a way to enter my own proc (for TKOS only)*/
extern void root( void );/* supplied by the developper */

void __tk_assertfail(	 /* emulates __assertfail */
	char *assertstr, 
	char *filestr, 
	int line
);

/** private functions **/
#endif








