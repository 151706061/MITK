/*****************************************************************************

 Copyright (c) 1993-2000,  Div. Medical and Biological Informatics, 
 Deutsches Krebsforschungszentrum, Heidelberg, Germany
 All rights reserved.

 Redistribution and use in source and binary forms, with or without 
 modification, are permitted provided that the following conditions are met:

 - Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

 - Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

 - All advertising materials mentioning features or use of this software must 
   display the following acknowledgement: 
          
     "This product includes software developed by the Div. Medical and 
      Biological Informatics, Deutsches Krebsforschungszentrum, Heidelberg, 
      Germany."

 - Neither the name of the Deutsches Krebsforschungszentrum nor the names of 
   its contributors may be used to endorse or promote products derived from 
   this software without specific prior written permission. 

   THIS SOFTWARE IS PROVIDED BY THE DIVISION MEDICAL AND BIOLOGICAL
   INFORMATICS AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE DIVISION MEDICAL AND BIOLOGICAL INFORMATICS,
   THE DEUTSCHES KREBSFORSCHUNGSZENTRUM OR CONTRIBUTORS BE LIABLE FOR 
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
   IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN 
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

 Send comments and/or bug reports to:
   mbi-software@dkfz-heidelberg.de

*****************************************************************************/

/**@file
 *  This functions looks for areas in an image with the same greyvalue.
 *  Each of these areas gets it's own greyvalue.  
 */

/** This functions looks for areas in an image with the same greyvalue.
 *  Each of these areas gets it's own greyvalue.  
 *
 *  CAUTION! In case of many small regions to label, the routine may 
 *  break down without exception handling, if the number of labels is 
 *  running out.
 *
 *  @param pic_old      pointer to original image structure
 *  @param no_label    number of different labels (return value)
 *
 *  @return pointer to new image
 *
 * USES
 *  function _ipFuncError  - checks image data
 *  function _ipFuncExtr   - calculates extreme greyvalues
 *  function _ipFuncHist   - calculates greyvalue histogram
 *
 * AUTHOR & DATE 
 */

/* include files                                                       */

#include "ipFuncP.h"

ipPicDescriptor *ipFuncLabel ( ipPicDescriptor *pic_old,
                               ipUInt4_t       *no_lab );

#ifndef DOXYGEN_IGNORE

#ifndef lint
  static char *what = { "@(#)ipFuncLabel\t\tDKFZ (Dept. MBI)\t"__DATE__ };
#endif



/* definition of macros                                                */

#define LABEL1( type, pic_old, pic_new, no_label, type_new )            \
{                                                                       \
  ipUInt4_t       i, j;        /* loop index                         */ \
  ipUInt4_t       offset;      /* offset of actual pixel             */ \
  ipBool_t        in_lab;      /* currently in label flag            */ \
  ipUInt4_t       end;                                                  \
                                                                        \
  offset   = 0;                                                         \
  no_label = 0;                                                         \
                                                                        \
  end    = _ipPicElements ( pic_old ) / pic_old->n[0];                  \
  for ( j = 0; j < end; j++ )                                           \
    for ( i = 0, in_lab = ipFalse; i < pic_old->n[0]; i++ )             \
      {                                                                 \
       /*if ( (( type * )pic_old->data )[offset]  == max )  */          \
         if ( (( type * )pic_old->data )[offset]  )                     \
            {                                                           \
              if ( in_lab )                                             \
                (( type_new * )pic_new->data )[offset] =                \
                                              (type_new) no_label;      \
              else                                                      \
                {                                                       \
                   in_lab = ipTrue;                                     \
                   no_label++;                                          \
                   (( type_new * )pic_new->data )[offset] =             \
                                                 (type_new) no_label;   \
                }                                                       \
            }                                                           \
         else in_lab = ipFalse;                                         \
         offset++;                                                      \
      }                                                                 \
}

#define LABEL2( type, pic_new, a, a_new, a_sort, size )                 \
{                                                                       \
  ipUInt4_t       aa;          /* actual anchestor                   */ \
  ipUInt4_t       al;          /* actual label                       */ \
  ipUInt4_t       i, j, k, l;  /* loop index                         */ \
  ipUInt4_t       end;                                                  \
  ipInt4_t        off;         /* offset of actual pixel             */ \
  ipInt4_t        dist;        /* distnace between actual and        */ \
                               /* neighbour pixel                    */ \
                                                                        \
  for ( l = 1; l < pic_new->dim; l++ )                                  \
    {                                                                   \
       dist = size[l];                                                  \
       end  = _ipPicElements ( pic_new ) / size[l+1];                   \
       for ( k = 0; k < end; k++ )                                      \
         for ( j = 1; j < pic_new->n[l]; j++ )                          \
           {                                                            \
              off = j * size[l] + k * size[l+1];                        \
              for ( i = 0; i < size[l]; i++ )                         \
                {                                                       \
                   /* calculate actual ancestors                     */ \
                                                                        \
                   al = (( type * )pic_new->data ) [off];               \
                   aa = (( type * )pic_new->data ) [off - dist];        \
                                                                        \
                   off++;                                               \
                                                                        \
                   /* if this is a line label                        */ \
                                                                        \
                   if ( al )                                            \
                     {                                                  \
                        if ( aa )                                       \
                          {                                             \
                             /* test whether anchestor found         */ \
                             if ( a[al] == al )                         \
                               a[al] = a_new[a[aa]];                    \
                             else if ( a[al] != a[a[al]] )              \
                               a[al] = a[a[al]];                        \
                             else if ( a[al] != a_new[a[aa]] )          \
                               a_new[a[aa]] = a[al];                    \
                          }                                             \
                     }                                                  \
                }                                                       \
           }                                                            \
    }                                                                   \
}

#define LABEL3( type, pic_new, a, a_sort )                              \
{                                                                       \
  ipUInt4_t       i;           /* loop index                         */ \
  ipUInt4_t       no_elem;                                              \
                                                                        \
  no_elem = _ipPicElements ( pic_new );                                 \
  for ( i = 0; i < no_elem; i++ )                                       \
    (( type * )pic_new->data )[i] =                                     \
       ( type ) a_sort[a[( ipUInt4_t ) (( type * )pic_new->data )[i]]]; \
}


/* ------------------------------------------------------------------- */
/* 
**  ipFuncLabel                     
*/
/* ------------------------------------------------------------------- */

ipPicDescriptor *ipFuncLabel ( ipPicDescriptor *pic_old,
                               ipUInt4_t       *no_lab )
{
  ipPicDescriptor *pic_new;      /* pointer to new image structure     */
  ipUInt4_t       i;             /* loop index                         */
  ipUInt4_t       *a;            /* pointer to list with all anchestors*/
  ipUInt4_t       *a_new;        /* look up table for new labels       */
  ipUInt4_t       *a_sort;       /* pointer to list of sorted anchest. */
  ipUInt4_t       no_label;      /* number of labels                   */
  ipUInt4_t       new_label;     /*                                    */
  ipUInt4_t       size[_ipPicNDIM];
  ipFloat8_t      min, max;      /* extreme greyvalues in image        */
  
  /* check image data                                                  */

  if ( _ipFuncError ( pic_old ) != ipFuncOK ) return ( ipFuncERROR );

  /* calculate extreme greyvalues in image                             */
  if ( ipFuncExtr ( pic_old, &min, &max ) != ipFuncOK ) return ( ipFuncERROR );

  /* check whether image is binary                                     */
  /*
  ipFuncHist ( pic_old, min, max, &hist, &size_hist );
  if ( hist == NULL ) 
    {
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }

  i     = 0;
  no_gv = 0;
  while ( i <= ( ipUInt4_t ) ( fabs ( min ) + max ) && no_gv < 3 )
    {
       if ( hist [i] != 0 ) no_gv++;
       i++;
    }
  
  if ( no_gv != 2 ) 
    {
       free ( hist );
       _ipFuncSetErrno ( ipFuncDATA_ERROR );
       return ( ipFuncERROR );
    }
  */

  /* allocate memory for new image                                     */

  pic_new = ipPicCopyHeader ( pic_old, NULL );
  pic_new->type = ipPicInt;
  pic_new->bpe  = 16;
  pic_new->data = calloc ( _ipPicElements ( pic_new ), pic_new->bpe / 8 );
  if ( pic_new == NULL )
    {
        _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
        return ( ipFuncERROR );
    }

  /* preparation of image                                              */

  ipPicFORALL_3 ( LABEL1, pic_old, pic_new, no_label, ipInt2_t );

  if(no_label>SHRT_MAX)
  {
	  ipPicFree(pic_new);
	  pic_new = ipPicCopyHeader ( pic_old, NULL );
	  pic_new->type = ipPicUInt;
	  pic_new->bpe  = 32;
	  pic_new->data = calloc ( _ipPicElements ( pic_new ), pic_new->bpe / 8 );
	  if ( pic_new == NULL )
		{
			_ipFuncSetErrno ( ipFuncPICNEW_ERROR );
			return ( ipFuncERROR );
		}
	  ipPicFORALL_3 ( LABEL1, pic_old, pic_new, no_label, ipUInt4_t );
  }

  /* allocation and initialisation of vectors                          */
 
  a      = malloc ( ( no_label + 1 ) * sizeof ( ipUInt4_t ) );
  if ( a == NULL ) 
    {
        ipPicFree ( pic_new );
        _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
        return ( ipFuncERROR );
    }
  a_new  = malloc ( ( no_label + 1 ) * sizeof ( ipUInt4_t ) );
  if ( a_new == NULL )
    {
        free ( a );
        ipPicFree ( pic_new );
        _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
        return ( ipFuncERROR );
    }
  a_sort = malloc ( ( no_label + 1 ) * sizeof ( ipUInt4_t ) );
  if ( a_sort == NULL )
    {
        free ( a );
        free ( a_new );
        ipPicFree ( pic_new );
        _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
        return ( ipFuncERROR );
    }

  for ( i = 0; i <= no_label; i++ ) 
    {
       a[i]      = i; 
       a_new[i]  = i;
       a_sort[i] = 0;
    }

  size[0] = 1;
  for ( i = 1; i <= pic_old->dim; i++ )
    size[i] = size[i-1] * pic_old->n[i-1];
  for ( i = pic_old->dim + 1; i < _ipPicNDIM; i++ )
    size[i] = size[pic_old->dim];

  /* change anchestor list                                             */

  ipPicFORALL_4 ( LABEL2, pic_new, a, a_new, a_sort, size );

  /* replace anchestors with new anchestors                            */

  for ( i = 0; i <= no_label; i++ )
   {
    a[i] = a_new[a[i]];
   }
  for ( i = 0; i <= no_label; i++ )
   {
    a[i] = _ipGetANew ( a_new, a[i] );
   }

  /*
  ** replace image with new labels                                       
  */

  /* hit used labels with 1                                           */

  for ( i = 0; i < no_label; i++ ) a_sort[a[i]] = 1;

  /* write new ascending labels to a_sort                             */

  for ( i = 0, new_label = 0; i <= no_label; i++ )
    {
       if ( a_sort[i] )
         {
            new_label++;
            a_sort[i] = new_label;
         } 
    }

  /* renumber pixels with new labels                                  */

  ipPicFORALL_2 ( LABEL3, pic_new, a, a_sort );

  /**no_lab = no_label;*/

  *no_lab = new_label;

  if((new_label<=SHRT_MAX) && ( pic_new->bpe != 16 ))
  {
     ipPicDescriptor * tmp;
     tmp=ipFuncConvert(pic_new, ipPicInt, 16);
     ipPicFree(pic_new);
     pic_new=tmp;
  }

  /* free memory                                                      */


  /* Copy Tags */

  ipFuncCopyTags(pic_new, pic_old);
  
  
                              
  free ( a ); 
  free ( a_new );
  free ( a_sort );
  return ( pic_new );

}
#endif

