#include <ctype.h>
#include "simple_logger.h"
#include "gfc_text.h"

int gfc_stricmp(const char *a,const char *b)
{
    int i,v = 0;
    const char *A,*B;
    char bufferA[1025];
    char bufferB[1025];
    A = a;
    B = b;
    do
    {
        memset(bufferA,0,sizeof(bufferA));
        memset(bufferB,0,sizeof(bufferB));
        for (i = 0; (i < 1024) && (*B != '\0') && (*A != '\0');i++,A++,B++)
        {
            bufferA[i] = tolower(*A);
            bufferB[i] = tolower(*B);
        }
        if ((*B == '\0')||(*A == '\0'))
        {
            v = strcmp(bufferA,bufferB) + (tolower(*A) - tolower(*B));
            return v;
        }
        v = strcmp(bufferA,bufferB);
    }
    while(v == 0);
    return v;
}

/**
 * @brief case insensitive number string compare
 * @param a one of the strings to compare
 * @param b one of the strings to compare
 * @param n how many characters to compare
 * @returns strcmp results
 */
int gfc_strincmp(const char *a,const char *b,int n)
{
    int i,v = 0,c = 0;
    const char *A,*B;
    char bufferA[1025];
    char bufferB[1025];
    A = a;
    B = b;
    do
    {
        memset(bufferA,0,sizeof(bufferA));
        memset(bufferB,0,sizeof(bufferB));
        for (i = 0; (i < 1024) && (*B != '\0') && (*A != '\0') && (c < n);i++,A++,B++,c++)
        {
            bufferA[i] = tolower(*A);
            bufferB[i] = tolower(*B);
        }
        if ((*B == '\0')||(*A == '\0'))
        {
            v = strcmp(bufferA,bufferB) + (tolower(*A) - tolower(*B));
            return v;
        }
        v = strcmp(bufferA,bufferB);
    }
    while((v == 0) && (c < n));
    return v;
}

int gfc_str_suffix(const char *str, const char *suffix)
{
    // credit stack overflow
    size_t lenstr,lensuffix;
    if (!str || !suffix)
        return 0;
    lenstr = strlen(str);
    lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)return 0;
    return strncmp(str + (lenstr - lensuffix), suffix, lensuffix) == 0;
}
/*eol@eof*/
