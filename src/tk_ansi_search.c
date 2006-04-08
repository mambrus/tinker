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
#include <stdlib.h>
#define MAX_SORT_ELEMENT_SIZE 255  //!< Limitation of each sorting elements size


/*
#if !defined(APP_NEEDS_INTERNALS)
void _tk_quicksort ( void *, int, int, int, comparison_fn_t );
int  _tk_bsearch   ( void *, void *, int, int, int, comparison_fn_t );
#endif
*/

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

unsigned int qsort_depth = 0; //!< @brief Recursion debug variable @internal
unsigned int curr_depth = 0;  //!< @brief Recursion debug variable @internal
/*!
@brief Quicksort algorithm (the back-end of ANSI reimpl. \ref qsort)

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

/*!
@brief Bsearch algorithm (the back-end of ANSI reimpl. \ref bsearch)

Very straight-forward implementation without recursion. I doubt this one
can be optimized much further.

@note To use bsearch on <b>vector tables</b> and to use the same \ref
comparison_fn_t as when you sorted the table in the first place,
remember you have to pass a pointer to a pointer as the first argument.
I.e. the argument you pass has to be of the same indirection level as
the elements in the list are.

The function will return the index number of the element to search for,
or a negative value if the element is not found. However, the "right-most"
element is also always returned and can deducted by taking the absolute
value that is returned and subtract 1,

If negative, the absolute value minus 1 tells which index is closest to
the elements "right". I.e. for search key \f$ k \f$, return value \f$
f(k) \f$ the the search index \f$ i \f$ and the right-most index \f$
i_R \f$ follows the definitions below:<p>
1)

\f[   

i =    

   \cases{   
   
      f(k) & \mbox{if f(k) is positive  } \cr 
      
      invalid & \mbox{if f(k) is negative } \cr   
      
   }  
   
\f]

2)

\f[   i_R = abs(f(k)) - 1 \f]

@attention The "feature" with also getting the right-most index if \f$ k \f$ is
not found, is meant to be used by a special version of insertion sort.
For normal search use, all you have to do is test against negative
vaules, and then use the value as an index. 

@returns index number <em>AND</em> "right-most" index 

@note The reason \f$ i_R \f$ is passed instead of \f$ i_L \f$ is to
avoid difficulties coding the value around index zero. By using \f$
i_R \f$, the resulting value after transformation is always positive
(note that 0 is defined as positive).

<h3>Example: Determining "in-between" indexes.</h3>
@code
   int left_idx;
   int right_idx;
   
   int idx = _tk_bsearch(&my_key,my_array,0,sz-1,sizeof(my_type),my_cmpf); 
   if (idx < 0){
      //Key not found, but would fit between left_idx & right_idx
      left_idx  = -1*idx-2;
      right_idx = -1*idx-1;
   }
@endcode

@attention See \ref bsearch for another important example (pit-fall).

     
*/

int _tk_bsearch (
   void *v,              //!< Search for this key
   void *a,              //!< The array to be seached   
   int l,                //!< left index
   int r,                //!< right index
   int sz,               //!< Size of each element
   comparison_fn_t cmp   //!< Comparison function 
){
   int   x;              // Current mid index
   void *m;              // Mid element
   int   rc;             // Last compare result
   
   while ( r >= l ){
      x = (r+l)/2;

      m = (void**)(((char*)a)+ (x*sz) );
      
      rc = cmp( v, m );
      if ( rc == 0)
         return x;

      if ( rc < 0)
         r = x - 1;
      else
         l = x + 1;
   }
   if ( rc < 0){
      //Key should had been to the left of the last evaluated index.
      /*
      printf("<--- [%d]\n",x);
      printf("[%d]..x..[%d]\n",x-1,x);
      printf("------------------------\n");
      printf("After transform, RL=%d\n\n",x);
      */
      return  ( -1*x -1);
   }else{
      //Key should had been to the right of the last evaluated index.
      /*
      printf("[%d] --->\n",x);
      printf("[%d]..x..[%d]\n",x,x+1);
      printf("------------------------\n");
      printf("After transform, RL=%d\n\n",x+1);
      */
      return  ( -1*x -2);
   }
}

#ifdef TINKER_SEARCH_SORT
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

<h3>Example: Pitfalls with character arrays</h3>
(The following also applies to \ref _tk_bsearch)<p>
This is correct:
@code      
      char *varray[] = {"One","Two,"Three"};
      char key[] = "Two";
      char *p1 = key;      
      
      bsearch(&p1,varray,3,sizeof(char*),my_strvcmp);            
@endcode            

This doesn't do what you think, (<tt>key</tt> and <tt>&key</tt> gives the
same value to \ref bsearch) 

@code            
     :
     rv = bsearch(&key,varray,3,sizeof(char*),my_strvcmp);
@endcode      

This is illegal (compile error).
@code            
     :
     rv = bsearch(&(char*)key,varray,3,sizeof(char*),my_strvcmp);
@endcode

This pitfall is easy to fall into while working with character arrays
that are supposed to be interpreted as strings (you can't assign other
data-types the same way or as easily as with strings).
 
One way to avoid the above scenario is to let <tt>key</tt> be a pointer
to begin with. The following is perfectly legal and works like a charm,
though not very practical (who needs a key that's constant?).:

@code             
   char *varray[] = {"One","Two,"Three"}; 
   char *key2 = "Two";
   bsearch(&key2,varray,3,sizeof(char*),my_strvcmp); 
@endcode

Or you can use malloc, then assign and then use - but I find using
char[] + one extra variable more convenient.
      

*/
void *bsearch (
   const void *key,         //!< Serch for this key
   const void *array,       //!< Sorted array to search for key
   size_t count,            //!< Number of elements in the array
   size_t size,             //!< Size of each element
   comparison_fn_t compare  //!< Comparison function 
){
   int i;

   i = _tk_bsearch((void*)key,(void*)array,0,count-1,size,compare);
   if (i>=0)
      return (void**)(((char*)array)+ (i*size) );
   else
      return NULL;
}
#endif

  
/*! 
 * @defgroup CVSLOG_tk_search_c tk_search_c
 * @ingroup CVSLOG
 *  $Log: tk_ansi_search.c,v $
 *  Revision 1.9  2006-04-08 10:16:02  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.8.2.1  2006/04/03 20:07:29  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.8  2006/03/27 13:40:15  ambrmi09
 *  As part of the preparation for the first release, code has been cleaned up a little
 *  and project has been checked that it will build on all it's intended targets.
 *
 *  Problems that remained had to do the ANSI wrapping.
 *
 *  Some modifications were neserary to make the BC5 build, but the result is cleaner
 *  and more consistent with the rest of the wrapping. As a consequence, stdlib.h was
 *  introduced.
 *
 *  Revision 1.7  2006/03/05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.6  2006/02/22 13:05:47  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.5  2006/02/16 15:11:00  ambrmi09
 *  Introduced a new component for better and safer useage of the heap.
 *  Package is called \red KMEM and the files are tk_mem.c and tk_mem.h (so
 *  far).
 *
 *  Started to take care of the long needed issue with error codes and
 *  better error handling. Introduced errno.h to begin with, whitch is part
 *  of the package \ref kernel_reimpl_ansi. Its not a good solution yet,
 *  since both kernel and ANSI codes are in the same file we have to invent
 *  a way to omit the ANSI defines when a tool-chain that has errno.h is
 *  used.
 *
 *  Revision 1.4  2006/02/14 16:14:06  ambrmi09
 *  Bsearch implemented, a lot of doc regarding \ref qsort \ref bsearch
 *  \ref _tk_qsort and \ref _tk_bsearch is added.
 *
 *  Also the qsort/bsearch example from GNU libc ref manual is copy &
 *  pasted into the root thread.
 *
 *  Revision 1.3  2006/02/13 14:31:24  ambrmi09
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
 













