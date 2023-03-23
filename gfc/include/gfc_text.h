#ifndef __GFC_TEXT__H__
#define __GFC_TEXT__H__
/**
 * gfc_text
 * @license The MIT License (MIT)
   @copyright Copyright (c) 2019 EngineerOfLies
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

/**
 * @purpose the MGL text library provides common string support for fixed length
 *  strings.  For use with unlimited strings mgl will use glib's GString type
 */
#include <string.h>
#include "gfc_types.h"
#include "gfc_color.h"
/**
 * constant lenth character buffers.
 * These will be used when the convenience of GString is not needed
 */

/**
 * @brief believe it or not but standard C does not have stricmp.  This does that
 * @param a one of the strings to compare
 * @param b one of the strings to compare
 * @returns strcmp results
 */
int gfc_stricmp(const char *a,const char *b);

/**
 * @brief case insensitive number string compare
 * @param a one of the strings to compare
 * @param b one of the strings to compare
 * @param n how many characters to compare
 * @returns strcmp results
 */
int gfc_strincmp(const char *a,const char *b,int n);

/**
 * @brief check if the str ends with the suffix
 * @param str the string to query
 * @param suffix the search criteria
 * @returns 1 if the string ends with the suffix or 0 if not
 */
int gfc_str_suffix(const char *str, const char *suffix);

/**
 * @brief 16 character buffer used for short tags
 */
typedef char TextWord[16];
#define GFCWORDLEN   16
#define gfc_word_cmp(a,b) (strncmp(a,b,GFCWORDLEN))
#define gfc_word_icmp(a,b) (strincmp(a,b,GFCWORDLEN))
#define gfc_word_cpy(dst,src) (strncpy(dst,src,GFCWORDLEN))
#define gfc_word_clear(a)  (memset(a,0,sizeof(char)*GFCWORDLEN))
#define gfc_word_sprintf(a,...) snprintf(a,GFCWORDLEN,__VA_ARGS__)
/**
 * @brief 128 character buffer used for statements
 */
typedef char TextLine[128];
#define GFCLINELEN   128
#define gfc_line_cmp(a,b) (strncmp(a,b,GFCLINELEN))
#define gfc_line_icmp(a,b) (strincmp(a,b,GFCLINELEN))
#define gfc_line_cpy(dst,src) (strncpy(dst,src,GFCLINELEN))
#define gfc_line_clear(a)  (memset(a,0,sizeof(char)*GFCLINELEN))
#define gfc_line_sprintf(a,...) snprintf(a,GFCLINELEN,__VA_ARGS__)
/**
 * @brief 1024 character buffer used for text blocks
 */
typedef char TextBlock[1024];
#define GFCTEXTLEN   1024
#define gfc_block_cmp(a,b) (strncmp(a,b,GFCTEXTLEN))
#define gfc_block_icmp(a,b) (strincmp(a,b,GFCTEXTLEN))
#define gfc_block_cpy(dst,src) (strncpy(dst,src,GFCTEXTLEN))
#define gfc_block_clear(a)  (memset(a,0,sizeof(char)*GFCTEXTLEN))
#define gfc_block_sprintf(a,...) snprintf(a,GFCTEXTLEN - 1,__VA_ARGS__)



#endif
