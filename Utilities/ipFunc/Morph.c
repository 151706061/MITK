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
 *  this function performs a morphological operation
 */

/** @brief performs a morphological operation. 
 *
 *  Depending on the parameter kind a erosion or dilation is performed. If the original
 *  image is an binary image each pixel is connected with the surrounding
 *  pixels which are non zero in the kernel by using logical operations 
 *  ( and for erosion, or for dilation ). Greylevel images are transformed
 *  by taking the minimum (erosion) of the surrounding pixels or the      
 *  maximum (dilation).
 *
 *  @param pic_old   pointer to the original image
 *  @param mask         pointer to the kernel
 *  @param kind         tells whether erosion or dilation is performed
 *                 0 (ipERO)  :   erosion
 *                 1 (ipDILA) :   dilation
 *  @param border      tells how the edge is transformed
 *  @arg @c     ipFuncBorderOld  :   original greyvalues
 *  @arg @c     ipFuncBorderZero :   edge is set to zero
 *
 * @return pointer to the transformed image
 *
 * @par Uses
 *  @arg _ipFuncError()     - check the image data          
 *  @arg ipFuncHist()      - calculate the greylevel histogram
 *  @arg  _ipFuncCompressM() - compress filtering mask
 *
 * AUTHOR & DATE
 */

/* include files                                                        */

#include "ipFuncP.h"

ipPicDescriptor *_ipFuncMorph ( ipPicDescriptor *pic_old,
                               ipPicDescriptor *mask,
                               _ipFuncFlagF_t  kind, 
                               ipFuncFlagI_t   border );
#ifndef DOXYGEN_IGNORE

#ifndef lint
  static char *what = { "@(#)ipFuncMorph\t\tDKFZ (Dept. MBI)\t"__DATE__ };
#endif

/* include files                                                        */

#include "ipFuncP.h"

/* definition of macros and constants                                     */

#define ERO_B  &&
#define DILA_B ||
#define ERO_G  <
#define DILA_G >

#define INIT( type, pic_new )                                              \
{                                                                          \
  ipUInt4_t     i;                   /* loop variable                   */ \
  ipUInt4_t     no_elem;                                                   \
                                                                           \
  no_elem = _ipPicElements ( pic_new );                                    \
  for ( i = 0; i < no_elem; i++ )                                          \
    (( type * )pic_new->data )[i] = 1;                                     \
}


#define MORPH_B( type, pic_old, pic_new, m, beg, end, OP )                 \
{                                                                          \
  ipUInt4_t     i;                   /* loop index                      */ \
  ipUInt4_t     offset;              /* offset                          */ \
  ipInt4_t      ind[_ipPicNDIM];     /* loop index vector               */ \
  ipUInt4_t     off[_ipPicNDIM];     /* offset vector                   */ \
                                                                           \
  /* transformation of image                                            */ \
                                                                           \
  for ( i = 0; i < m->length; i++ )                                        \
  {                                                                        \
    offset = m->off_vekt[i];                                               \
    for ( ind[7] = beg[7]; ind[7] < end[7]; ind[7]++ )                     \
    {                                                                      \
      off[7] = ind[7] * size[7];                                           \
      for ( ind[6] = beg[6]; ind[6] < end[6]; ind[6]++ )                   \
      {                                                                    \
        off[6] = ind[6] * size[6] + off[7];                                \
        for ( ind[5] = beg[5]; ind[5] < end[5]; ind[5]++ )                 \
        {                                                                  \
          off[5] = ind[5] * size[5] + off[6];                              \
          for ( ind[4] = beg[4]; ind[4] < end[4]; ind[4]++ )               \
          {                                                                \
            off[4] = ind[4] * size[4] + off[5];                            \
            for ( ind[3] = beg[3]; ind[3] < end[3]; ind[3]++ )             \
            {                                                              \
              off[3] = ind[3] * size[3] + off[4];                          \
              for ( ind[2] = beg[2]; ind[2] < end[2]; ind[2]++ )           \
              {                                                            \
                off[2] = ind[2] * size[2] + off[3];                        \
                for ( ind[1] = beg[1]; ind[1] < end[1]; ind[1]++ )         \
                {                                                          \
                  off[1] = ind[1] * size[1] + off[2];                      \
                  off[0] = beg[0] + off[1];                                \
                  for ( ind[0] = beg[0]; ind[0] < end[0]; ind[0]++ )       \
                  {                                                        \
                    (( type * )pic_new->data )[off[0]] =                   \
                        (( type * )pic_new->data)[off[0]] OP               \
                        (( type * )pic_old->data)[off[0] + offset];        \
                    off[0]++;                                              \
                  }                                                        \
                }                                                          \
              }                                                            \
            }                                                              \
          }                                                                \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }                                                                        \
}

#define MORPH_G( type, pic_old, pic_new, m, beg, end, OP )                 \
{                                                                          \
  ipUInt4_t     i;                   /* loop index                      */ \
  ipInt4_t      ind[_ipPicNDIM];     /* loop index vector               */ \
  ipUInt4_t     off[_ipPicNDIM];     /* offset vector                   */ \
  type          help, help2;                                               \
                                                                           \
                                                                           \
  /* transformation of image                                            */ \
                                                                           \
  for ( ind[7] = beg[7]; ind[7] < end[7]; ind[7]++ )                       \
  {                                                                        \
    off[7] = ind[7] * size[7];                                             \
    for ( ind[6] = beg[6]; ind[6] < end[6]; ind[6]++ )                     \
    {                                                                      \
      off[6] = ind[6] * size[6] + off[7];                                  \
      for ( ind[5] = beg[5]; ind[5] < end[5]; ind[5]++ )                   \
      {                                                                    \
        off[5] = ind[5] * size[5] + off[6];                                \
        for ( ind[4] = beg[4]; ind[4] < end[4]; ind[4]++ )                 \
        {                                                                  \
          off[4] = ind[4] * size[4] + off[5];                              \
          for ( ind[3] = beg[3]; ind[3] < end[3]; ind[3]++ )               \
          {                                                                \
            off[3] = ind[3] * size[3] + off[4];                            \
            for ( ind[2] = beg[2]; ind[2] < end[2]; ind[2]++ )             \
            {                                                              \
              off[2] = ind[2] * size[2] + off[3];                          \
              for ( ind[1] = beg[1]; ind[1] < end[1]; ind[1]++ )           \
              {                                                            \
                off[1] = ind[1] * size[1] + off[2];                        \
                off[0] = beg[0] + off[1];                                  \
                for ( ind[0] = beg[0]; ind[0] < end[0]; ind[0]++ )         \
                {                                                          \
                  help = (( type* )pic_old->data)[off[0]];                 \
                  for ( i = 0; i < m->length; i++ )                        \
                  {                                                        \
                    help2 = (( type * )pic_old->data)[m->off_vekt[i]+off[0]];\
                    help  = ( help OP help2 ) ? help : help2;              \
                  }                                                        \
                  (( type * )pic_new->data)[off[0]] = help;                \
                  off[0]++;                                                \
                }                                                          \
              }                                                            \
            }                                                              \
          }                                                                \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }                                                                        \
                                                                           \
}

/* ---------------------------------------------------------------------- */
/* 
** function _ipFuncMorph
*/
/* ---------------------------------------------------------------------- */

ipPicDescriptor *_ipFuncMorph ( ipPicDescriptor *pic_old,
                               ipPicDescriptor *mask,
                               _ipFuncFlagF_t  kind, 
                               ipFuncFlagI_t   border )
{
  ipPicDescriptor *pic_new;        /* pointer to transformed image         */
  ipFuncMasc_t    *m;              /* compressed mask and belonging offsets*/
  ipInt4_t        beg[_ipPicNDIM];
  ipInt4_t        end[_ipPicNDIM];
  ipUInt4_t       size[_ipPicNDIM];    /*                                  */  
  ipUInt4_t       no_gv;           /* number of different greyvalues       */
  ipUInt4_t       i;               /* loop index                           */
  ipUInt4_t       size_hist;       /* number of elements in histogram      */
  ipUInt4_t       *hist;           /* pointer to greyvalue histogram       */
  ipFloat8_t      min, max;        /* extreme greyvalues in image          */

  /* check image data                                                      */

  if ( _ipFuncError ( pic_old ) != ipFuncOK ) return ( ipFuncERROR );
  if ( _ipFuncError ( mask ) != ipFuncOK ) return ( ipFuncERROR );
  if ( mask->dim > pic_old->dim ) 
    {
       _ipFuncSetErrno ( ipFuncDIMMASC_ERROR );
       return ( ipFuncERROR );
    }
 
  for ( i = 0; i < mask->dim; i++ )
    if ( mask->n[i] > pic_old->n[i] ) 
      {
         _ipFuncSetErrno ( ipFuncDATA_ERROR );
         return ( ipFuncERROR );
      }

  pic_new = NULL;

  /* initialisation of vectors                                             */  
                                                                            
  size [0] = 1;                                                             
  for ( i = 1; i < _ipPicNDIM; i++ )                                        
    size[i] = size[i-1] * pic_old->n[i-1];                                  
  size[pic_old->dim] = 0;                                                   

  /* compress filtering mask                                               */

  m = _ipFuncCompressM ( mask, pic_old, ipFuncNoReflect, beg, end );
  if ( m == NULL ) 
    {
       free ( hist );
       ipPicFree ( pic_new );
       return ( ipFuncERROR );
    }

  /* allocate and initialize pic_new                                       */
 
  if ( border == ipFuncBorderOld ) 
    pic_new = ipPicClone ( pic_old );
  else if ( border == ipFuncBorderZero )
    { 
       pic_new = ipPicCopyHeader ( pic_old, NULL );
       if ( pic_new == NULL )
         {
            _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
            return ( ipFuncERROR );
         }
       pic_new->data = calloc ( _ipPicElements ( pic_new ), pic_new->bpe / 8 );
       if ( pic_new->data == NULL )
         {
            ipPicFree ( pic_new );
            _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
            return ( ipFuncERROR );
         }

       if ( kind == ipFuncEroF ) 
         {
            ipPicFORALL ( INIT, pic_new );                                       
         }
    }
  else 
    {
       _ipFuncSetErrno ( ipFuncFLAG_ERROR );
       return ( ipFuncERROR );
    }
 
  if ( pic_new == NULL )  
    {  
       _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
       return ( ipFuncERROR );
    }

  /* check whether binary or greylevel image                               */
  
  if ( ipFuncExtr ( pic_old, &min, &max ) != ipFuncOK ) return ( ipFuncERROR );
  ipFuncHist ( pic_old, min, max, &hist, &size_hist );
  if ( hist == NULL ) 
    {
       ipPicFree ( pic_new );
       return ( ipFuncERROR );
    }

  i     = 0;
  no_gv = 0;
  while ( i <= ( ipUInt4_t ) ( fabs ( min ) + max ) && no_gv < 3 )
    {
       if ( hist [i] != 0 ) no_gv++;
       i++;
    }
  

  /* transform image (depending on kind and no_gv)                         */

  if ( kind == ipFuncEroF )                             /* erosion         */
    {
       if ( no_gv == 2 )                                /* binary image    */
         {
            ipPicFORALL_5 ( MORPH_B, pic_old, pic_new, m, beg, end, ERO_B );
         }
       else if ( no_gv > 2 )                            /* greylevel image */
         {
            ipPicFORALL_5 ( MORPH_G, pic_old, pic_new, m, beg, end, ERO_G );
         }
       else 
         {
            free ( hist );
            free ( m->off_vekt );
            free ( m->mask_vekt );
            free ( m );
            /*_ipFuncSetErrno ( ipFuncDATA_ERROR );*/
            return ( pic_new );
         }
    }
  else if ( kind == ipFuncDilaF )                        /* dilation        */
    {
       if ( no_gv == 2 )                                /* binary image    */
         {
            ipPicFORALL_5 ( MORPH_B, pic_old, pic_new, m, beg, end, DILA_B );
         }
       else if ( no_gv > 2 )                            /* greylevel image */
         {
            ipPicFORALL_5 ( MORPH_G, pic_old, pic_new, m, beg, end, DILA_G );
         }
       else 
         {
            free ( hist );
            free ( m->off_vekt );
            free ( m->mask_vekt );
            free ( m );
            /*_ipFuncSetErrno ( ipFuncDATA_ERROR );*/
            return ( pic_new );
         }
    }
  else
    {
       ipPicFree ( pic_new );
       free ( hist );
       free ( m->off_vekt );
       free ( m->mask_vekt );
       free ( m );
       _ipFuncSetErrno ( ipFuncFLAG_ERROR );
       return ( ipFuncERROR );
    }

  /* free memory                                                           */
  free ( m->off_vekt );
  free ( m->mask_vekt );
  free ( m );
  free ( hist );

  return ( pic_new );
}
#endif
