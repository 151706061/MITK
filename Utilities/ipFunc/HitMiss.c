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
 *  this function performs the morphological hit and miss operation
 */

/** @brief performs the morphological hit and miss operation
 *
 *  @param pic_old    pointer to original image
 *  @param pic_masks    pointer to a set of hit and miss masks
 *                 ( if pic_masks = NULL -> default masks are used )
 *  @param border      tells how the edge is transformed
 *  @arg @c ipFuncBorderOld  original greyvalues        
 *  @arg @c ipFuncBorderZero : edge is set to minimal greyvalue
 *
 *  @return pointer to image after hit and miss operation 
 *
 * AUTHOR & DATE
 */

/* include files                                                      */

#include "ipFuncP.h"

ipPicDescriptor *ipFuncHitMiss ( ipPicDescriptor *pic_old, 
                                 ipPicDescriptor *pic_masks,
                                 ipFuncFlagI_t   border );

#ifndef DOXYGEN_IGNORE

#ifndef lint
  static char *what = { "@(#)ipFuncHitMiss\t\tDKFZ (Dept. MBI)\t"__DATE__ };
#endif



/* ------------------------------------------------------------------ */
/*
** ipFuncHitMiss         
*/
/* ------------------------------------------------------------------ */

ipPicDescriptor *ipFuncHitMiss ( ipPicDescriptor *pic_old, 
                                 ipPicDescriptor *pic_masks,
                                 ipFuncFlagI_t   border )
{
  ipBool_t          allocated=ipFalse;
  ipPicDescriptor   *pic_new;  /* pointer to new image                */
  ipUInt1_t         mask[] =   /* hit and miss mask (edge detection)  */
                    { 0, 0, 0, 0, 1, 0, 0, 0, 0, 
                      0, 1, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 1, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 1, 0, 0, 0,
                      0, 0, 0, 0, 1, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 1, 0,
                      0, 0, 0, 0, 1, 0, 0, 0, 0,
                      0, 0, 0, 1, 0, 0, 0, 0, 0,
                      0, 1, 0, 0, 0, 0, 0, 0, 0 };

  if ( pic_masks == NULL )
    {
       allocated = ipTrue;
       pic_masks = ipPicNew ();
       if ( pic_masks == NULL ) 
         {
           _ipFuncSetErrno ( ipFuncPICNEW_ERROR );
           return ( ipFuncERROR );
         }
       pic_masks->dim  = 3;
       pic_masks->type = ipPicUInt;
       pic_masks->bpe  = 8;
       pic_masks->n[0] = 3;
       pic_masks->n[1] = 3;
       pic_masks->n[2] = 8;
       pic_masks->data = mask;
    } 
  pic_new = _ipFuncHitMissI ( pic_old, pic_masks, border );
  
  if ( allocated )
    {
       pic_masks->data = NULL;
       ipPicFree ( pic_masks );
    }

  /* Copy Tags */

  ipFuncCopyTags(pic_new, pic_old);
  
        

  return ( pic_new );
}               
#endif