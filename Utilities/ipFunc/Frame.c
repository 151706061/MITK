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
  * this function adds a frame around an image
 */

/** @brief adds a frame around an image
 *
 *  The thickness of the frame can be chosen separately for each dimension 
 *  (parameter edge). The original image will not be changed, instead a 
 *  new one will be returned.
 * 
 * @param pic_old image to which the frame will be added
 * @param edge vector with the thichness' of the frame in each direction
 * @param value this greyvalue is used for the frame
 *
 * $Log$
 * Revision 1.2  2000/05/24 15:29:42  tobiask
 * Changed the installation paths of the package.
 *
 * Revision 1.4  2000/05/04 12:52:10  ivo
 * inserted BSD style license
 *
 * Revision 1.3  2000/03/06 17:02:39  ivo
 * ipFunc now with doxygen documentation.
 *
 * Revision 1.2  2000/02/18 14:58:02  ivo
 * Tags are now copied into newly allocated images.
 * Bugs fixed in ipFuncFrame, ipFuncRegGrow, _ipFuncBorderX and ipFuncHitMiss.
 *
 * Revision 1.1.1.1  2000/02/18  15:30:50  ivo
 * memory leak removed: ipFuncKeep in call of ipFuncWindowR substituted
 * by ipFuncKeep (pic_new is already a new image!).
 *
 *
 * AUTHOR & DATE
 */

#include "ipFuncP.h"
ipPicDescriptor *ipFuncFrame ( ipPicDescriptor *pic_old,
                               ipUInt4_t       *edge,
                               ipFloat8_t      value );
#ifndef DOXYGEN_IGNORE

#ifndef lint
  static char *what = { "@(#)????????\t\tDKFZ (Dept. MBI)\t"__DATE__ };
#endif

/* include files                                                                  */



/* ------------------------------------------------------------------------------ */
/*
*/
/* ------------------------------------------------------------------------------ */

ipPicDescriptor *ipFuncFrame ( ipPicDescriptor *pic_old,
                               ipUInt4_t       *edge,
                               ipFloat8_t      value )
{
  ipPicDescriptor *pic_new;            /* pointer to transformed image            */

  pic_new = _ipFuncBorderX ( pic_old, edge, value );
  if ( ipFuncErrno > ipFuncOK )
    return ( ipFuncERROR );
  else 
    pic_new = ipFuncWindowR ( pic_new, pic_old, edge, ipFuncNoKeep );

  /* Copy Tags */

  strncpy( pic_new->info->version, pic_old->info->version, _ipPicTAGLEN );
  pic_new->info->tags_head = _ipPicCloneTags( pic_old->info->tags_head );
  pic_new->info->write_protect = pic_old->info->write_protect;

  return ( pic_new );
}
#endif