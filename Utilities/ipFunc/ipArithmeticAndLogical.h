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
 *  Arithmetic and Logical Operators.
 *  This header is for grouping purposes for the
 *  documentation only. Don't use it, use ipFunc.h instead.
 */

#ifndef DOXYGEN_IGNORE
#include <ipFunc.h>
#endif

#ifndef _ipFunc_h
#define _ipFunc_h

/* function prototypes */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

ipPicDescriptor *ipFuncInv    ( ipPicDescriptor *pic_old,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncEqual  ( ipPicDescriptor *pic_old,   
                                ipFuncFlagI_t   kind,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncAddC   ( ipPicDescriptor *pic_old,
                                ipFloat8_t       value,
                                ipFuncFlagI_t    keep,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncSubC   ( ipPicDescriptor *pic_old,
                                ipFloat8_t       value,
                                ipFuncFlagI_t    keep,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncMultC  ( ipPicDescriptor *pic_old,
                                ipFloat8_t       value,  
                                ipFuncFlagI_t    keep,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncDivC   ( ipPicDescriptor *pic_old,
                                ipFloat8_t       value,  
                                ipFuncFlagI_t    keep,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncMultI  ( ipPicDescriptor *pic_old,
                                ipPicDescriptor *pic_new,
                                ipFuncFlagI_t   keep,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncDivI   ( ipPicDescriptor *pic_old,
                                ipPicDescriptor *pic_new,
                                ipFuncFlagI_t   keep,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncAddI   ( ipPicDescriptor *pic_old,
                                ipPicDescriptor *pic_new,  
                                ipFuncFlagI_t   keep,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncAddSl  ( ipPicDescriptor *pic_old,
                                ipFuncFlagI_t   keep );

ipPicDescriptor *ipFuncSubI   ( ipPicDescriptor *pic_old,
                                ipPicDescriptor *pic_new,  
                                ipFuncFlagI_t   keep,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncAnd    ( ipPicDescriptor *pic_1,
                                ipPicDescriptor *pic_2,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncOr     ( ipPicDescriptor *pic_1,
                                ipPicDescriptor *pic_2,
                                ipPicDescriptor *pic_return );

ipPicDescriptor *ipFuncNot    ( ipPicDescriptor *pic_old,
                                ipPicDescriptor *pic_return );

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif


#endif /* _ipFunc_h */ 
/* DON'T ADD ANYTHING AFTER THIS #endif */

