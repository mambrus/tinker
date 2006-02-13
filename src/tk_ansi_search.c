/*!
@file
@ingroup kernel_reimpl_ansi

@brief ANSI search & sort functions reimplemented

The functions in this file are all reimplementations of ANSI functions
for searching and sorting, that TinKer relies on but that are commonly
missing from many embedded toolchain suppliers.

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi

@see kernel_reimpl_ansi

*/

#include "tk_ansi.h"
#include <string.h>
#define MAX_SORT_ELEMENT_SIZE 255  //!< Limitation of each sorting elements size


/*!
@brief Swap <em>l</em> and <em>r</em> in <em>a</em>

Swapping helper function will swap the elements at positions <em>l</em>
and <em>r</em> in array <em>a</em> with an element size of <em>sz</em>.

@note To avoid time consuming mallocs, a temp-variable <em>t</em> is
allocated on the stack. This implies that in case _swap is used in a
recursive functions, your <b>recursion depth is limited</b> by the
maximum size of the temporary variable variable. Furthermore,
implementation also implies that <b>elements bigger than
MAX_SORT_ELEMENT_SIZE will be corrupted</b>. Hence, you must make
certain these limitation doesn't affect you're usage.

@todo supply a malloc based version.

@note The temp-variable <em>t</em> can't be global, or it would make
this function non re-entrant and non thread safe.
*/



void _swap (
   void *a,              //!< The array to be sorted
   int l,                //!< left index
   int r,                //!< right index
   int sz                //!< Size of each element   
){
   char t[MAX_SORT_ELEMENT_SIZE];
   memcpy(t,                  &((char*)a)[l*sz],   sz);
   memcpy(&((char*)a)[l*sz],  &((char*)a)[r*sz],   sz);
   memcpy(&((char*)a)[r*sz],  t,                   sz);
}

unsigned int qsort_depth = 0;
unsigned int curr_depth = 0;
/*!
@brief Quicksort algorithm (the back-end of qsort)

This is a rather straight-forward implementation of the quick-sort
algorithm. It's supposed to be used as a back-end to the 
re-implemented ANSI function \ref qsort. I.e. it has to be able to work
on arrays with elements of any size (i.e. scalars, structs & pointers), which
makes the implementation a little bit tricky. Notice the call to the
compare function - note that we're <em>passing a <TT>void**</TT> to a
function that expects a <TT>void*</TT> </em>. This means that no matter
what's in the array, we're <b>always comparing references</b> and never
actual elements.

@note You must make sure of that you're compare function always operates
on references as well. 

Here comes a couple of examples of 3 different compare functions.
- a scalar - in our case a character
- a structure - in our case an array with fixed number of elements
- a pointer list - in our case pointers to strings.

Please make sure you understand the difference between especially the
last two cases.

@note Any nonzero expression is considered true in C. (ref: ANSI)

@code
int my_charcmp (  
   const void *L,
   const void *R 
){
   int rc;

   char l = *(char*)L;
   char r = *(char*)R;
   rc = (l==r)?0:( (l<r)?-1 : 1 );
   return rc;
}
@endcode

@code
int my_strcmp ( const void *L, const void *R ){
   return strcmp((char*)L,(char*)R);   
}
@endcode

@code
int my_strvcmp ( const void *L, const void *R ){
   return strcmp(*(char**)L,*(char**)R);   
}
@endcode

@note <b>This is a recursive version of quick-sort</b>


@note The algorithm is said to be unstable if not a sentinel at a[0]
is present, which is guaranteed to be less than any other a[1]..a[N].
The front-end doesn't bother with this a.t.m. since we plan to invoke
the <i>median-of-three</i> partitioning optimization (which as a
side-effect takes care of this rare special case).


@todo I'm not sure if the sorting function (\ref comparison_fn_t)
behavior is how ANSI qsort expects it to work. Better check this.

@todo Develop a non-recursive version of this function

@todo Develop a shell-sort optimization for N<C

@todo Develop a <i>median-of-three</i> partitioning optimization

*/
void _tk_quicksort (
   void *a,              //!< The array to be sorted
   int l,                //!< left index
   int r,                //!< right index
   int sz,               //!< Size of each element
   comparison_fn_t cmp   //!< Comparison function 
){
   int i,j;
   void *v;

   curr_depth++;
   if (curr_depth > qsort_depth)
      qsort_depth=curr_depth;

   if ( r > l ){

      v = (void**)(((char*)a)+(r*sz));       
      i = l-1; j = r;
      for (;;){
         while ( cmp(  (void**)(((char*)a)+(++i*sz)), v ) < 0) ;
         while ( cmp(  (void**)(((char*)a)+(--j*sz)), v ) > 0) ;
         if (i >= j) break;
         _swap(a,i,j,sz);
      }
      _swap(a,i,r,sz);
      
      _tk_quicksort(a,l,i-1  ,sz,cmp);
      _tk_quicksort(a,i+1,r  ,sz,cmp);
   }
   curr_depth--;
}

#if defined(TK_NEEDS_QSORT)
/*!
@brief ANSI qsort 

To sort an array using an arbitrary comparison function, use the qsort
function. The prototype for this function is in stdlib.h.

Function: void qsort (void *array, size_t count, size_t size,
comparison_fn_t compare)

The qsort function sorts the array <em>array</em>. The array
contains <em>count</em> elements, each of which is of size
<em>size</em>.

The <em>compare</em> function is used to perform the comparison on the
array elements. This function is called with two pointer arguments and should
return an integer less than, equal to, or greater than zero
corresponding to whether its first argument is considered less than,
equal to, or greater than its second argument.


@note Warning: If two objects compare as equal, their order after sorting is
unpredictable. That is to say, the sorting is not stable. This can make
a difference when the comparison considers only part of the elements.
Two elements with the same sort key may differ in other respects.

If you want the effect of a stable sort, you can get this result by
writing the comparison function so that, lacking other reason
distinguish between two elements, it compares them by their addresses.
Note that doing this may make the sorting algorithm less efficient, so
do it only if necessary.

GNU reference: @see http://www.gnu.org/software/libc/manual/html_mono/libc.html#Array%20Sort%20Function

*/
void qsort (
   void *array,             //!< The array to be sorted
   size_t count,            //!< Number of elements in the array
   size_t size,             //!< Size of each element
   comparison_fn_t compare  //!< Comparison function 
){
   qsort_depth = 0;
   _tk_quicksort(array,0,count-1,size,compare);
}
#endif

#if defined(TK_NEEDS_BSEARCH)
/*!
@brief ANSI bsearch

To search a sorted array for an element matching the key, use the
bsearch function. The prototype for this function is in the header file
stdlib.h.

void * bsearch (const void *key, const void *array, size_t count, size_t
size, comparison_fn_t compare) 

The bsearch function searches the sorted array <em>array</em> for an
object that is equivalent to <em>key</em>. The array contains
<em>count</em> elements, each of which is of size <em>size</em> bytes.

The compare function is used to perform the comparison. This function is
called with two pointer arguments and should return an integer less
than, equal to, or greater than zero corresponding to whether its first
argument is considered less than, equal to, or greater than its second
argument. <i>The elements of the array must already be sorted in
ascending order according to this comparison function.</i>


@return The return value is a pointer to the matching array element, or
a null pointer if no match is found. If the array contains more than one
element that matches, the one that is returned is unspecified. 

This function derives its name from the fact that it is implemented
using the binary search algorithm. 

GNU reference: @see http://www.gnu.org/software/libc/manual/html_mono/libc.html#Array%20Search%20Function
*/

void * bsearch (
   const void *key,         //!< Serch for this key
   const void *array,       //!< Sorted array to search for key
   size_t count,            //!< Number of elements in the array
   size_t size,             //!< Size of each element
   comparison_fn_t compare  //!< Comparison function 
){
}
#endif


  
/*! 
 * @addtogroup CVSLOG CVSLOG
 *  $Log: tk_ansi_search.c,v $
 *  Revision 1.3  2006-02-13 14:31:24  ambrmi09
 *  Crude first version of ANSI qsort implemented.
 *
 *  Revision 1.2  2006/02/09 23:05:25  ambrmi09
 *  Doxygen related fixes
 *
 *  Revision 1.1  2006/02/09 22:40:39  ambrmi09
 *  Initial creation of ANSI re-implementation module
 *
 *  
 *******************************************************************/
 













