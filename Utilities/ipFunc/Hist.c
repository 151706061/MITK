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
 *  this function calculates the greyvalue histogram of an image
 */

/**  this function calculates the greyvalue histogram of an image
 *
 *  @param pic_old  pointer to the image that should be inverted
 *  @param min_gv   min. greyvalue in histogram
 *  @param max_gv   max. greyvalue in histogram
 *  @param hist     pointer to the greyvalue histogram ( return value )
 *  @param size_hist   pointer to the number of elements in the histogram
 *                ( result value )
 *
 *  @return @arg @c ipFuncERROR     - if an error occured
 *  @return @arg @c ipFuncOK        - if no error occured
 *
 * AUTHOR & DATE
 */

/* include-Files                                                        */

#include "ipFuncP.h"   

ipInt4_t  ipFuncHist ( ipPicDescriptor *pic_old, 
                       ipFloat8_t      min_gv,
                       ipFloat8_t      max_gv,
                       ipUInt4_t       **hist,
                       ipUInt4_t       *size_hist ) ;

#ifndef DOXYGEN_IGNORE


#ifndef lint
  static char *what = { "@(#)ipFuncHist\t\tDKFZ (Dept. MBI)\t"__DATE__ };
#endif



 
/* definition of HIST-macro                                             */

#define HIST( type, pic, help, factor )                                  \
{                                                                        \
  ipUInt4_t  i;                                                          \
  ipUInt4_t  no_elem;                                                    \
  ipUInt4_t  index;                                                      \
                                                                         \
  /* calculate histogram                                             */  \
                                                                         \
  no_elem =  _ipPicElements ( pic );                                     \
  for ( i = 0; i < no_elem; i++ )                                        \
    {                                                                    \
       index = ( ipUInt4_t )                                             \
               ( factor * ( (( type * ) pic->data)[i] + ( type ) help) );\
       ( hist_help[index] )++;                                           \
    }                                                                    \
} 
               
 

/* -------------------------------------------------------------------  */
/*
*/
/* -------------------------------------------------------------------  */

ipInt4_t  ipFuncHist ( ipPicDescriptor *pic_old, 
                       ipFloat8_t      min_gv,
                       ipFloat8_t      max_gv,
                       ipUInt4_t       **hist,
                       ipUInt4_t       *size_hist ) 
{
  ipUInt4_t       *hist_help;
  ipFloat8_t      help;            /* absolute of min_gv                */
  ipFloat8_t      min, max;        /* extreme greyvalues                */
  ipUInt4_t       factor;          /* factor to calculate histogram of  */
                                   /* float images                      */

  /* check whether data are correct                                     */

  if ( _ipFuncError ( pic_old ) != ipFuncOK ) return ( ipFuncERROR );
  if ( min_gv == 0 && max_gv == 0 )
    {
       if ( ipFuncExtr ( pic_old, &min_gv, &max_gv ) != ipFuncOK ) return ( ipFuncERROR );
    }
  else 
    {
       if ( min_gv > max_gv ) 
         {
            _ipFuncSetErrno ( ipFuncDATA_ERROR );
            return ( ipFuncERROR );
         }
       if ( ipFuncExtr ( pic_old, &min, &max ) != ipFuncOK ) return ( ipFuncERROR );
       if ( min_gv > min || max_gv < max )
         {
            _ipFuncSetErrno ( ipFuncDATA_ERROR );
            return ( ipFuncERROR );
         }
    }

  /* calculate variables which are necessary to calculate histogram     */
  /* index                                                              */

  help = fabs ( min_gv );

  if ( pic_old->type == ipPicInt || pic_old->type == ipPicUInt )
    factor = 1;
  else if ( pic_old->type == ipPicFloat )
    factor = 1000;
  else 
    {
       _ipFuncSetErrno (ipFuncTYPE_ERROR );
       return ( ipFuncERROR );
    }
  
  /* allocate memory                                                    */

  *size_hist = factor * ( ( ipUInt4_t )  max_gv + ( ipUInt4_t ) help );

/*hist  = ( ipUInt4_t ** ) malloc ( sizeof ( ipUInt4_t * ) );*/
  *hist = ( ipUInt4_t * )  calloc ( *size_hist+1, sizeof ( ipUInt4_t ) );
  hist_help = *hist;
  if ( hist_help == NULL ) 
    {
       _ipFuncSetErrno ( ipFuncMALLOC_ERROR );                    
       return ( ipFuncERROR );
    }

  /* macro to calculate the histogram                                   */

  ipPicFORALL_2 ( HIST, pic_old, help, factor );

  return ( ipFuncOK );
}
#endif
