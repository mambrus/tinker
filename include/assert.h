//assert macro

#if defined(__CYGWIN32__)  || defined(__CYGWIN__)  || \
	defined(__GNUC__)      || defined(__USE_GNU)   || \
	defined(WIN32)         || defined(_BC50_)

	#error "DONT USE TINKER'S ASSERT FOR THIS TOOL_CHAIN"
#endif

//#ifdef NDEBUG
//#  define assert(p)   ((void)0)
//#else
//#  
//#  define assert(p) ((p) ? (void)0 : (void) _tk_assertfail( \
//                    #p, __FILE__, __LINE__ ) )              
//#endif

/*!
The assert macro. 

@note Always assert, ignore NDEBUG setting
*/
#define assert(p) ((p) ? (void)0 : (void) _tk_assertfail( \
                    #p, __FILE__, __LINE__ ) )              
