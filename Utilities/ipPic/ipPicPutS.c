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
 * $RCSfile$
 *--------------------------------------------------------------------
 * DESCRIPTION
 *   writes a PicFile to disk
 *
 * $Log$
 * Revision 1.10  2002/11/13 17:53:00  ivo
 * new ipPic added.
 *
 * Revision 1.8  2000/05/04 12:52:40  ivo
 * inserted BSD style license
 *
 * Revision 1.7  2000/05/04 12:36:01  ivo
 * some doxygen comments.
 *
 * Revision 1.6  2000/01/17  18:32:00  andre
 * *** empty log message ***
 *
 * Revision 1.5  1999/11/27  19:15:08  andre
 * *** empty log message ***
 *
 * Revision 1.4  1998/09/01  15:26:43  andre
 * *** empty log message ***
 *
 * Revision 1.3  1998/05/18  12:13:54  andre
 * *** empty log message ***
 *
 * Revision 1.2  1997/09/15  13:21:19  andre
 * switched to new what string format
 *
 * Revision 1.1.1.1  1997/09/06  19:09:59  andre
 * initial import
 *
 * Revision 0.0  1993/04/02  16:18:39  andre
 * Initial revision
 *
 *
 *--------------------------------------------------------------------
 *  COPYRIGHT (c) 1993 by DKFZ (Dept. MBI) Heidelberg, FRG
 */
#ifndef lint
  static char *what = { "@(#)ipPicPutSlice\t\tDKFZ (Dept. MBI)\t"__DATE__"\t$Revision$" };
#endif

#include "ipPic.h"

void ipPicPutSlice( char *outfile_name, ipPicDescriptor *pic, ipUInt4_t slice )
{
  ipPicDescriptor *pic_in;

  ipUInt4_t tags_len;

  FILE *outfile;

  pic_in = ipPicGetHeader( outfile_name,
                           NULL );

  if( pic_in == NULL )
    {
      if( slice == 1 )
        {
          pic->n[pic->dim] = 1;
          pic->dim += 1;

          ipPicPut( outfile_name, pic );

          pic->dim -= 1;
          pic->n[pic->dim] = 0;

          return;
        }
      else
        return;
    }

  pic_in = ipPicGetTags( outfile_name,
                         pic_in );

  outfile = fopen( outfile_name, "r+b" );

  if( outfile == NULL )
    {
      /*ipPrintErr( "ipPicPut: sorry, error opening outfile\n" );*/
      /*return();*/
    }

  if( pic->dim != pic_in->dim - 1 )
    {
      fclose( outfile );
      return;
    }
  else if( pic->n[0] != pic_in->n[0]  )
    {
      fclose( outfile );
      return;
    }
  else if( pic->n[1] != pic_in->n[1]  )
    {
      fclose( outfile );
      return;
    }

  if( slice > pic_in->n[pic_in->dim-1] )
    pic_in->n[pic_in->dim-1] += 1;

  /* write oufile */
  /*fseek( outfile, 0, SEEK_SET );*/
  rewind( outfile );
  fwrite( ipPicVERSION, 1, sizeof(ipPicTag_t), outfile );

  fseek( outfile, sizeof(ipUInt4_t), SEEK_CUR ); /* skip tags_len */

  ipFWriteLE( &(pic_in->type), sizeof(ipUInt4_t), 1, outfile );
  ipFWriteLE( &(pic_in->bpe), sizeof(ipUInt4_t), 1, outfile );
  ipFWriteLE( &(pic_in->dim), sizeof(ipUInt4_t), 1, outfile );

  ipFWriteLE( pic_in->n, sizeof(ipUInt4_t), pic_in->dim, outfile );

  fseek( outfile, pic_in->info->pixel_start_in_file + _ipPicSize(pic) * (slice - 1), SEEK_SET );

  ipFWriteLE( pic->data, pic->bpe / 8, _ipPicElements(pic), outfile );

  /*fseek( outfile, 0, SEEK_END );*/

  fclose( outfile );

  ipPicFree(pic_in);

  /*return();*/
}
