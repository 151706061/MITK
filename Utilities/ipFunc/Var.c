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
 *    this function calculates the variance of all greyvalues
 *    in an image
 */

/**   this function calculates the variance of all greyvalues
 *    in an image
 *
 *    @param pic          pointer to the image 
 *    @param &max         result variable: mean greyvalue
 *  
 *  @return  ipFuncERROR    - if an error occured
 *  @return  ipFuncOK       - if no error occures
 *
 * AUTHOR & DATE
 */
 
/* include-files                                              */

#include "ipFuncP.h"
 
ipFloat8_t ipFuncVar ( ipPicDescriptor *pic );

#ifndef DOXYGEN_IGNORE

/* definition of extreme value macro                          */

#define MEAN_2( type, pic, mean, mean_2 )                      \
  {                                                            \
    ipUInt4_t   i, no_elem;                                    \
                                                               \
    mean_2 = 0.;                                               \
                                                               \
    no_elem = _ipPicElements ( pic );                          \
    for ( i = 0; i < no_elem; i++ )                            \
      {                                                        \
        mean_2 = ( ( ( type * ) pic->data ) [i] - mean ) *     \
                 ( ( ( type * ) pic->data ) [i] - mean ) +     \
                 mean_2;                                       \
      }                                                        \
  }
 
/* ========================================================== */
/*
** function picVar  : calculates the mean value (mean) 
**                    of an image (pic)
*/
/* ========================================================== */

ipFloat8_t ipFuncVar ( ipPicDescriptor *pic )
{

  ipFloat8_t var;             
  ipFloat8_t mean;            
  ipFloat8_t mean_2;           

  /* check image data                                         */

  if ( _ipFuncError ( pic ) != ipFuncOK ) return ( ipFuncERROR );

  if (  _ipPicElements ( pic ) == 1 ) 
    var = 0;
  else
    {
       /* calculate mean value                                */

       mean =  ipFuncMean ( pic );

       ipPicFORALL_2( MEAN_2, pic, mean, mean_2 );   
  
       var =  mean_2 / ( ipFloat8_t ) ( _ipPicElements ( pic ) - 1 );
    }

  return( var );
}
#endif
