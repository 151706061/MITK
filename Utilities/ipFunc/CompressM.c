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
 *  this function removes all elements which are zero and
 *  calculates the offsets of the non-zero elements
 *  which are nonzero
 */
/** @brief removes all elements which are zero from the mask and
 *  calculates the offsets (relative to the actual pixel) for all elements
 *  which are nonzero
 *
 *  @param mask pointer to mask structure
 *  @param pic_old pointer to original image
 *  @param kind tells whether mask should be reflected
 *  @arg @c ipNOREFLECT  : original mask is used
 *  @arg @c ipREFLECT    : reflected mask is used
 *  @param beg pixel number where convolution begins ( return value )
 *  @param end pixel number where convolution ends   ( return value )
 *
 *  @return m pointer to compressed mask structure
 *
 *
 * AUTHOR & DATE
 */

#include "ipFuncP.h"

ipFuncMasc_t *_ipFuncCompressM ( ipPicDescriptor *mask, 
                                ipPicDescriptor *pic_old,
                                ipFuncFlagI_t   kind, 
                                ipInt4_t        beg[_ipPicNDIM],  
                                ipInt4_t        end[_ipPicNDIM] ) ;

#ifndef DOXYGEN_IGNORE

#ifndef lint
  static char *what = { "@(#)ipFuncCompressM\t\tDKFZ (Dept. MBI)\t"__DATE__ };
#endif




#define OFF_M( type, pic_mask, pic, beg, size, m, kind )                 \
{                                                                        \
  ipUInt4_t       i;            /* loop index                         */ \
  ipUInt4_t       pos;          /*                                    */ \
  ipInt4_t        off_mask;     /* offset for mask elements           */ \
  ipInt4_t        offset;       /* offset for picture elements        */ \
  ipInt4_t        incr;                                                  \
  ipUInt4_t       size_m[_ipPicNDIM];                                    \
  ipInt4_t        ind[_ipPicNDIM];   /* loop index vector             */ \
  ipInt4_t        ind_m[_ipPicNDIM]; /* loop index vector             */ \
  ipUInt4_t       n[_ipPicNDIM];                                         \
                                                                         \
  /* initialisation of vectors                                        */ \
                                                                         \
  size_m[0]  = 1;                                                        \
  size[0]    = 1;                                                        \
  for ( i = 1; i < _ipPicNDIM; i++ )                                     \
    {                                                                    \
       size_m[i]  = size_m[i-1] * pic_mask->n[i-1];                      \
       size[i]    = size[i-1] * pic->n[i-1];                             \
    }                                                                    \
                                                                         \
  /* callculate offsets from actual pixel for each mask element       */ \
                                                                         \
  if ( kind == ipFuncNoReflect )                                         \
    {                                                                    \
       off_mask = 0;                                                     \
       incr     = 1;                                                     \
    }                                                                    \
  else if ( kind == ipFuncReflect )                                      \
    {                                                                    \
       off_mask = size_m[pic_mask->dim] -1;                              \
       incr     = -1;                                                    \
    }                                                                    \
  else return ( ipFuncERROR );                                               \
                                                                         \
  m->length = 0;                                                         \
                                                                         \
  for ( ind[7] = 0, ind_m[7] = -beg[7]; ind[7] < pic_mask->n[7];         \
        ind[7]++, ind_m[7]++ )                                           \
    for ( ind[6] = 0, ind_m[6] = -beg[6]; ind[6] < pic_mask->n[6];       \
          ind[6]++, ind_m[6]++ )                                         \
      for ( ind[5] = 0, ind_m[5] = -beg[5]; ind[5] < pic_mask->n[5];     \
            ind[5]++, ind_m[5]++ )                                       \
        for ( ind[4] = 0, ind_m[4] = -beg[4]; ind[4] < pic_mask->n[4];   \
              ind[4]++, ind_m[4]++ )                                     \
          for ( ind[3] = 0, ind_m[3] = -beg[3]; ind[3] < pic_mask->n[3]; \
                ind[3]++, ind_m[3]++ )                                   \
            for ( ind[2] = 0, ind_m[2] = -beg[2];                        \
                  ind[2] < pic_mask->n[2]; ind[2]++, ind_m[2]++ )        \
              for ( ind[1] = 0, ind_m[1] = -beg[1];                      \
                    ind[1] < pic_mask->n[1]; ind[1]++, ind_m[1]++ )      \
                for ( ind[0] = 0, ind_m[0] = -beg[0];                    \
                      ind[0] < pic_mask->n[0]; ind[0]++, ind_m[0]++ )    \
                  {                                                      \
                    if ( (( type * )pic_mask->data )[off_mask] != 0 )    \
                      {                                                  \
                         offset = 0;                                     \
                         for ( i = 0; i < pic_mask->dim; i++ )           \
                           offset = offset + ind_m[i] * size[i];         \
                                                                         \
                         m->off_vekt[m->length] = offset;                \
                         m->length++;                                    \
                      }                                                  \
                    off_mask = off_mask + incr;                          \
                  }                                                      \
                                                                         \
  /* remove elements that are zero from mask                          */ \
                                                                         \
  pos = 0;                                                               \
  for ( i = 0; i < m->length; i++ )                                      \
    {                                                                    \
      while ( (( type * )pic_mask->data )[pos] == 0 ) pos++;             \
      m->mask_vekt[i] = ( ipFloat8_t ) (( type * )pic_mask->data )[pos]; \
      pos++;                                                             \
    }                                                                    \
                                                                         \
}

/* -------------------------------------------------------------------  */
/*
*/
/* -------------------------------------------------------------------  */
 
ipFuncMasc_t *_ipFuncCompressM ( ipPicDescriptor *mask, 
                                ipPicDescriptor *pic_old,
                                ipFuncFlagI_t   kind, 
                                ipInt4_t        beg[_ipPicNDIM],  
                                ipInt4_t        end[_ipPicNDIM] ) 
{
  ipUInt4_t       i, j;               /* loopindex                      */
  ipFuncMasc_t    *m;                 /* length of mask and offsets     */
  ipUInt4_t       size[_ipPicNDIM];


  /* check whether data are correct                                     */

  if ( _ipFuncError ( mask ) != ipFuncOK ) return ( ipFuncERROR );
  if ( _ipFuncError ( pic_old ) != ipFuncOK ) return ( ipFuncERROR );

  /* initialisation of vectors                                          */
  
  for ( i = 0; i < mask->dim; i++ )
    {
       end[i] = pic_old->n[i] - mask->n[i] / 2;                             
       beg[i] = ( ( mask->n[i] % 2 ) == 1 ) ?
                ( mask->n[i] / 2 ) : ( mask->n[i] / 2 - 1 );             
    }

  for ( i = mask->dim; i < _ipPicNDIM; i++ )
    {
       beg[i] = 0;
       mask->n[i] = 1;
    }

  for ( i = mask->dim; i < pic_old->dim; i++ )                              
    end[i] = pic_old->n[i];                                                 
  for ( i = pic_old->dim; i < _ipPicNDIM; i++ )                             
    end[i] = beg[i] + 1;   

  /* allocate mask structure                                            */

  m = malloc ( sizeof ( ipFuncMasc_t ) );
  if ( m == NULL ) 
    {
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }
  m->off_vekt  = malloc ( _ipPicElements( mask )  * sizeof ( ipInt4_t ) );
  if ( m->off_vekt == NULL ) 
    {
       free ( m );
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }
  m->mask_vekt = malloc ( _ipPicElements( mask )  * sizeof ( ipFloat8_t ) );
  if ( m->mask_vekt == NULL ) 
    {
       free ( m->off_vekt );
       free ( m );
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }

  /* macro to compress mask      (for all data types)                   */

  ipPicFORALL_5 ( OFF_M, mask, pic_old, beg, size, m, kind );
 
  return ( m ); 

}
#endif