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

/*
 * _ipFuncBox2d 
 *---------------------------------------------------------------------
 * DESCRIPTION
 *   This Function calulates the bounding box                               
 *
 * FUNCTION DECLARATION
 *
 * PARAMETERS
 *   pic_old  - binary image: contains the region of interest
 *   beg      - pointer to left upper corner
 *   end      - pointer to right lower corner
 *
 * RETURN VALUES
 *   ipFuncOK     - no error occured
 *   ipFuncERROR  - error occured (description of error in ipFuncErrno)
 *
 * UPDATES
 *   this version could only be used for 3D images, in later version 
 *   it should be possible to use it for every dimension
 *
 * AUTHOR & DATE
 *  Antje Schroeder	08.01.97
 *
 *---------------------------------------------------------------------
 * COPYRIGHT (c) 1995 by DKFZ (Dept. MBI) HEIDELBERG, FRG
 */
/* include Files                                                        */

#include "ipFuncP.h"

/* definition of Box-macro                                              */

#define BOX( type, pic, help_beg, help_end )                              \
{                                                                         \
  ipInt4_t    i, j, l;                                                   \
  ipUInt4_t    offset;                                                    \
  ipInt4_t    end_x, end_y;                                              \
  ipBool_t    found;                                                      \
                                                                          \
  for ( l = 0; l < pic->dim; l++ )                                        \
    {                                                                     \
       i = 0;                                                             \
       found = ipFalse;                                                   \
       end_y = pic->n[order[l*pic->dim+1]];                               \
       while (( i < end_y ) && ( !found ) )                               \
         {                                                                \
            j = 0;                                                        \
            end_x = pic->n[order[l*pic->dim]];                            \
            while (( j < end_x ) && ( !found ) )                          \
              {                                                           \
                 offset = j * size[order[l * pic->dim]] +                 \
                          i * size[order[l * pic->dim + 1]];              \
                 if ( (( type * )pic->data )[ offset ] )                  \
                   {                                                      \
                      found = ipTrue;                                     \
                      help_beg[1-l] = i;                                  \
                   }                                                      \
                 j++;                                                     \
              }                                                           \
           i++;                                                           \
         }                                                                \
                                                                          \
       i = pic->n[order[l*pic->dim+1]] -1;                                \
       found = ipFalse;                                                   \
       while (( i >= 0 ) && ( !found ) )                                  \
         {                                                                \
            j =  pic->n[order[l*pic->dim]] -1;                            \
            while (( j >= 0 ) && ( !found ) )                             \
              {                                                           \
                 offset = j * size[order[l * pic->dim]] +                 \
                          i * size[order[l * pic->dim + 1]];              \
                 if ( (( type * )pic->data )[ offset ] )                  \
                   {                                                      \
                      found = ipTrue;                                     \
                      help_end[1-l] = i;                                  \
                   }                                                      \
                 j--;                                                     \
              }                                                           \
           i--;                                                           \
         }                                                                \
     }                                                                    \
                                                                          \
                                                                          \
}



/* -------------------------------------------------------------------  */
/*
*/
/* -------------------------------------------------------------------  */

ipUInt4_t _ipFuncBox2d ( ipPicDescriptor *pic_old,
                         ipUInt4_t       **beg,
                         ipUInt4_t       **end )            
{

  ipUInt4_t       n[_ipPicNDIM];          /* number of pixels in each   */
                                          /* dimension                  */
  ipUInt4_t       size[_ipPicNDIM];       /* size of each direction     */
  ipUInt4_t       i, j, k, l;             /* loop index                 */
  ipUInt4_t       *help_beg, *help_end;
  ipUInt4_t       order[] = { 0,  1,
                              1,  0 };

  /* check data                                                         */

  if ( _ipFuncError ( pic_old ) != ipFuncOK ) return ( ipFuncERROR );
   
  if ( pic_old->dim > 2 )   /* just in for the first version */
    {
       _ipFuncSetErrno ( ipFuncDIM_ERROR );
       return ( ipFuncERROR );
    }

  /* initialization of vectors */

  size[0] = 1;
  for ( i = 1; i < pic_old->dim; i++ )
     size[i] = size[i-1] * pic_old->n[i-1];
 
  /* allocation of result vectors                                       */

  *beg = ( ipUInt4_t * ) malloc ( pic_old->dim * sizeof ( ipUInt4_t ) );
  help_beg = *beg;
  *end = ( ipUInt4_t * ) malloc ( pic_old->dim * sizeof ( ipUInt4_t ) );
  help_end = *end;

  ipPicFORALL_2 ( BOX, pic_old, help_beg, help_end );                      

  return ( ipFuncOK );
}
