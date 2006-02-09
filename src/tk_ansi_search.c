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


#if defined(TK_NEEDS_QSORT)
/*!
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
}
#endif

#if defined(TK_NEEDS_BSEARCH)
/*!
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
 *  Revision 1.2  2006-02-09 23:05:25  ambrmi09
 *  Doxygen related fixes
 *
 *  Revision 1.1  2006/02/09 22:40:39  ambrmi09
 *  Initial creation of ANSI re-implementation module
 *
 *  
 *******************************************************************/
 













