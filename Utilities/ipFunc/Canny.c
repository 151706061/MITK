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

/** @file
 *  this function performs an edge detection using the Canny operator
 */
  
/** @brief performs an edge detection using the Canny operator.
 *
 *  The Canny-operator needs quite a lot memory. This can cause problems
 *  with large images.
 *
 *  @param pic_old pointer to original image
 *  @param dim_mask dimension of mask
 *  @param len_mask length of filtering mask
 *  @param threshold threshold to decide whether an edge belongs to 
 *                  the image or not
 *  @param border tells how the edge is transformed        
 *
 *  @arg @c ipFuncBorderOld  : original greyvalues        
 *  @arg @c ipFuncBorderZero : edge is set to minimal greyvalue
 *
 *  @return pointer to transformed iamge ( of type ipFloat8_t )
 *
 * AUTHOR & DATE
 
 */

/* include files                                                     */

#include "ipFuncP.h"

ipPicDescriptor *ipFuncCanny( ipPicDescriptor *pic_old,
                              ipUInt4_t       dim_mask,
                              ipUInt4_t       len_mask,
                              ipFloat8_t      threshold,
                              ipFuncFlagI_t   border ) ;

#ifndef DOXYGEN_IGNORE

#ifndef lint
  static char *what = { "@(#)ipFuncCanny\t\tDKFZ (Dept. MBI)\t"__DATE__ };
#endif


/* definition of macros                                              */

#define CANNY0( type_o, pic_old, pic_help1, pic_help2, pic_help3, m )     \
{                                                                         \
  ipPicFORALL_5( CANNY1, pic_help1, pic_old, pic_help2, pic_help3, m, type_o );\
}

#define CANNY1( type_h, pic_help1, pic_old, pic_help2, pic_help3, m, type_o )\
{                                                                          \
  ipUInt4_t     i, j;                /* loop index                      */ \
  ipUInt4_t     len_m;                                                     \
  ipInt4_t      ind[_ipPicNDIM];     /* loop index vector               */ \
  ipUInt4_t     off[_ipPicNDIM];     /* offset vector                   */ \
  ipUInt4_t     off_m;               /* offset in mask                  */ \
  ipFloat8_t    help[_ipPicNDIM];                                          \
  ipFloat8_t    help2;                                                     \
                                                                           \
                                                                           \
  /* transformation of image                                            */ \
                                                                           \
  len_m = m->length / dim_mask;                                            \
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
                  for ( i = 0; i < dim_mask; i++ ) help[i] = 0;            \
                  off_m = 0;                                               \
                  for ( i = 0; i < dim_mask; i++ )                         \
                    for ( j = 0; j < len_m; j++ )                          \
                    {                                                      \
                       help[i] = ( ipFloat8_t )m->mask_vekt[off_m] *       \
                                   (( type_o * ) pic_old->data )           \
                                     [off[0]+m->off_vekt[off_m]] + help[i];\
                       off_m++;                                            \
                    }                                                      \
                  help2 = 0;                                               \
                  for ( i = 0; i < dim_mask; i++ )                         \
                  {                                                        \
                    (( type_h * )pic_help1->data )[off[0]+off_p[i]] =      \
                                                        ( type_h ) help[i];\
                    help2 = help2 + help[i] * help[i];                     \
                  }                                                        \
                  (( type_h * ) pic_help2->data )[off[0]] =                \
                                                         ( type_h ) help2; \
                  off[0]++;                                                \
                }                                                          \
              }                                                            \
            }                                                              \
          }                                                                \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }                                                                        \
}

#define CANNY2( type_h, pic_help2, pic_help3, pic_help1, m )               \
{                                                                          \
  ipUInt4_t     i, j;                /* loop index                      */ \
  ipUInt4_t     len_m;                                                     \
  ipInt4_t      ind[_ipPicNDIM];     /* loop index vector               */ \
  ipUInt4_t     off[_ipPicNDIM];     /* offset vector                   */ \
  ipUInt4_t     off_m;               /* offset in mask                  */ \
  ipFloat8_t    help[_ipPicNDIM];                                          \
  ipFloat8_t    help2;                                                     \
                                                                           \
                                                                           \
  /* transformation of image                                            */ \
                                                                           \
  len_m = m->length / dim_mask;                                            \
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
                  for ( i = 0; i < dim_mask; i++ ) help[i] = 0;            \
                  off_m = 0;                                               \
                  for ( i = 0; i < dim_mask; i++ )                         \
                    for ( j = 0; j < len_m; j++ )                          \
                    {                                                      \
                       help[i] = help[i] + ( ipFloat8_t )m->mask_vekt[off_m] *\
                                 (( type_h * ) pic_help2->data )           \
                                               [off[0]+m->off_vekt[off_m]];\
                       off_m++;                                            \
                    }                                                      \
                  help2 = 0;                                               \
                  for ( i = 0; i < dim_mask; i++ )                         \
                  {                                                        \
                    help2  = help[i] * (( type_h * )pic_help1->data )      \
                                                [off[0]+ off_p[i]] + help2;\
                  }                                                        \
                  (( type_h * ) pic_help3->data )[off[0]] = ( type_h )help2;\
                  off[0]++;                                                \
                }                                                          \
              }                                                            \
            }                                                              \
          }                                                                \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }                                                                        \
}

#define CANNY3( type_h, pic_help2, pic_help3, threshold )                  \
{                                                                          \
   ipUInt4_t  i;                    /* loop variable                    */ \
   ipUInt4_t  no_elem;              /* number of pixels                 */ \
                                                                           \
   no_elem =  _ipPicElements ( pic_help2 );                                \
   for ( i = 0; i < no_elem; i++ )                                         \
     (( type_h * )pic_help3->data )[i] =                                   \
        ( ( (( type_h * ) pic_help3->data )[i] != 0 ) &&                   \
          ( (( type_h * ) pic_help2->data )[i] > threshold ) ) ?           \
            1 : 0;                                                         \
} 

/* ----------------------------------------------------------------- */
/*
** ipFuncCanny
*/
/* ----------------------------------------------------------------- */

ipPicDescriptor *ipFuncCanny( ipPicDescriptor *pic_old,
                              ipUInt4_t       dim_mask,
                              ipUInt4_t       len_mask,
                              ipFloat8_t      threshold,
                              ipFuncFlagI_t   border ) 
{
  #include "gradient.h"

  ipPicDescriptor *pic_help1;         /* pointer to image               */
  ipPicDescriptor *pic_help2;         /* pointer to image               */
  ipPicDescriptor *pic_help3;         /* pointer to image               */
  ipPicDescriptor *pic_help4;         /* pointer to image               */
  ipPicDescriptor *pic_new;           /* pointer to image               */
  ipPicDescriptor *pic_mask;          /* sobel mask                     */
  ipUInt4_t       pos;                /* position in m->off_vekt        */
  ipUInt4_t       i, j;               /* loopindex                      */
  ipUInt4_t       off_mask;           /* loopindex                      */
  ipFuncMasc_t    *m;                 /* compressed mask                */
  ipInt4_t        offset;                 
  ipInt4_t        beg[_ipPicNDIM];
  ipInt4_t        end[_ipPicNDIM];
  ipInt4_t        ind[_ipPicNDIM];
  ipUInt4_t       size[_ipPicNDIM];
  ipInt4_t        n[_ipPicNDIM];
  ipUInt4_t       off_p[_ipPicNDIM];  /* offsets to calculate help1     */
  ipUInt4_t       offset_h;           /* offset in pic_help1            */

  /* check whether data are correct                                     */

  if ( _ipFuncError ( pic_old ) != ipFuncOK ) return ( ipFuncERROR );

  if ( ( dim_mask < 1 ) || ( dim_mask > 4 ) )
    {
       _ipFuncSetErrno ( ipFuncDIMMASC_ERROR );
       return ( ipFuncERROR );
    }

  if ( pic_old->dim < dim_mask ) 
    {
       _ipFuncSetErrno ( ipFuncDIMMASC_ERROR );
       return ( ipFuncERROR );
    }

  /* initialisation of pic_mask                                         */

  pic_mask = ipPicNew ();
  if ( pic_mask == NULL ) 
    {
       _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
       return ( ipFuncERROR );
    }

  pic_mask->type = ipPicInt;
  pic_mask->bpe  = 16;
  pic_mask->dim  = dim_mask+1;
  for ( i = 0; i < dim_mask; i++ )
    pic_mask->n[i] = 3;
  pic_mask->n[dim_mask] = dim_mask;

  if ( dim_mask == 4 ) 
    pic_mask->data = mask4;      
  else if ( dim_mask == 3 ) 
    pic_mask->data = mask3;      
  else if ( dim_mask == 2 )
    pic_mask->data = mask2; 

  /* initialisation of vectors                                          */
  
  size [0] = 1;                                                             
  for ( i = 1; i < pic_old->dim; i++ )                                          
    size[i] = size[i-1] * pic_old->n[i-1];                                  
  for ( i = pic_old->dim; i < _ipPicNDIM; i++ )                                 
    size[i] = 0;                                                            

  offset_h = size[pic_old->dim-1] * pic_old->n[pic_old->dim-1];

  for ( i = 0; i < dim_mask; i++ )
    n[i] = pic_mask->n[i] / 2 + 1;
  for ( i = dim_mask; i < _ipPicNDIM; i++ )
    n[i] = 0;

  for ( i = 0; i < dim_mask; i++ )
    {
       end[i] = pic_old->n[i] - pic_mask->n[i] / 2;        
       beg[i] = ( ( pic_mask->n[i] % 2 ) == 1 ) ?
                ( pic_mask->n[i] / 2 ) : ( pic_mask->n[i] / 2 - 1 );        
       off_p[i] = i * offset_h;
    }
  for ( i = dim_mask; i < _ipPicNDIM; i++ )
    beg[i] = 0;

  for ( i = dim_mask; i < pic_old->dim; i++ )        
    end[i] = pic_old->n[i];        
  for ( i = pic_old->dim; i < _ipPicNDIM; i++ )        
    end[i] = beg[i] + 1;

  /* create a new picture, copy the header, allocate memory             */

  pic_help2 = ipPicCopyHeader ( pic_old, NULL );
  if ( pic_help2 == NULL ) 
    {
       pic_mask->data = NULL;
       ipPicFree ( pic_mask);
       _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
       return ( ipFuncERROR );
    }
  if ( pic_old->type == ipPicUInt || pic_old->type == ipPicInt ) 
    {
       pic_help2->type = ipPicFloat;
       pic_help2->bpe  = 64;
    }
  pic_help2->data = calloc ( _ipPicElements ( pic_help2 ), pic_help2->bpe / 8 );
  if ( pic_help2->data == NULL )
    {
       pic_mask->data = NULL;
       ipPicFree ( pic_mask);
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }

  pic_help1 = ipPicCopyHeader ( pic_help2, NULL );
  if ( pic_help1 == NULL )
    {
       pic_help2->data = NULL;
       ipPicFree ( pic_help2);
       pic_mask->data = NULL;
       ipPicFree ( pic_mask);
       _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
       return ( ipFuncERROR );
    }
  pic_help1->dim = pic_help2->dim + 1;
  pic_help1->n[pic_help1->dim-1] = dim_mask;
  pic_help1->data = calloc ( _ipPicElements ( pic_help1 ), pic_help1->bpe / 8 );
  if ( pic_help1->data == NULL ) 
    {
       pic_help2->data = NULL;
       ipPicFree ( pic_help2);
       pic_mask->data = NULL;
       ipPicFree ( pic_mask);
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }
  
  pic_help3 = ipPicCopyHeader ( pic_help2, NULL );
  if ( pic_help3 == NULL ) 
    {
       pic_mask->data = NULL;
       ipPicFree ( pic_mask );
       pic_help1->data = NULL;
       ipPicFree ( pic_help1 );
       pic_help2->data = NULL;
       ipPicFree ( pic_help2 );
       _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
       return ( ipFuncERROR );
    }
  pic_help3->data = calloc ( _ipPicElements ( pic_help3 ), pic_help3->bpe / 8 );
  if ( pic_help3->data == NULL ) 
    {
       pic_mask->data = NULL;
       ipPicFree ( pic_mask );
       pic_help1->data = NULL;
       ipPicFree ( pic_help1 );
       pic_help2->data = NULL;
       ipPicFree ( pic_help2 );
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }

  /* allocate mask-structure                                            */

  m = malloc ( sizeof ( ipFuncMasc_t ) );
  if ( m == NULL ) 
    {
       pic_mask->data = NULL;
       ipPicFree ( pic_mask );
       pic_help1->data = NULL;
       ipPicFree ( pic_help1 );
       pic_help2->data = NULL;
       ipPicFree ( pic_help2 );
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }
  m->off_vekt  = malloc ( _ipPicElements( pic_mask ) * sizeof ( ipInt4_t ) );
  if ( m->off_vekt == NULL ) 
    {
       pic_mask->data = NULL;
       ipPicFree ( pic_mask );
       pic_help1->data = NULL;
       ipPicFree ( pic_help1 );
       pic_help2->data = NULL;
       ipPicFree ( pic_help2 );
       pic_help3->data = NULL;
       ipPicFree ( pic_help3 );
       free ( m );
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }
  m->mask_vekt = malloc ( _ipPicElements( pic_mask ) * sizeof ( ipFloat8_t ) );
  if ( m->mask_vekt == NULL ) 
    {
       pic_mask->data = NULL;
       ipPicFree ( pic_mask );
       pic_help1->data = NULL;
       ipPicFree ( pic_help1 );
       pic_help2->data = NULL;
       ipPicFree ( pic_help2 );
       pic_help3->data = NULL;
       ipPicFree ( pic_help3 );
       free ( m->off_vekt );
       free ( m );
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );
       return ( ipFuncERROR );
    }

  /* calculate offset vector for the compressed mask                    */

  m->length = 0;

  off_mask = 0;
  for ( i = 0; i < pic_mask->n[dim_mask] ; i++ )
    for ( ind[3] = -1; ind[3] < n[3]; ind[3]++ )
      for ( ind[2] = -1; ind[2] < n[2]; ind[2]++ )
        for ( ind[1] = -1; ind[1] < n[1]; ind[1]++ )
          for ( ind[0] = -1; ind[0] < n[0]; ind[0]++ )
            {
              if ( (( ipInt2_t * )pic_mask->data)[off_mask] != 0 )
                {
                   offset = 0;
                   for ( j = 0; j < dim_mask; j++ )
                     offset = offset + ind[j] * size[j];
                   m->off_vekt[m->length] = offset;
                   m->length++;
                }
              off_mask++;
            }

  /* remove elements that are zero from mask                          */

  pos = 0;
  for ( i = 0; i < m->length; i++ )
    {
      while ( (( ipInt2_t * )pic_mask->data)[pos] == 0 ) pos++;
      m->mask_vekt[i] = ( ipFloat8_t )(( ipInt2_t * )pic_mask->data)[pos];
      pos++;
    }

  /* smooth image using a binomial filter                              */

  pic_new = ipFuncGausF ( pic_old, len_mask, dim_mask, border );

  /* calculate second derivation                                       */

  ipPicFORALL_4 ( CANNY0, pic_new, pic_help1, pic_help2, pic_help3, m );       
  /* project second derivation to gradient                             */

  ipPicFORALL_3 ( CANNY2, pic_help2, pic_help3, pic_help1, m );

  /* zero crossing                                                     */

  pic_help4 = ipFuncZeroCr ( pic_help3 );
 
  ipPicFORALL_2 ( CANNY3, pic_help2, pic_help4, threshold );

  /* free memory                                                       */

  free ( m->off_vekt );
  free ( m->mask_vekt );
  free ( m );
  ipPicFree ( pic_new );
  ipPicFree ( pic_help1 );
  ipPicFree ( pic_help2 );
  ipPicFree ( pic_help3 );
  pic_mask->data = NULL;
  ipPicFree ( pic_mask );

  /* Copy Tags */

  ipFuncCopyTags(pic_help4, pic_old);

  return ( pic_help4 );
}
                               
#endif 
