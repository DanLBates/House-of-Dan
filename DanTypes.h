/* DanTypes.h */

#ifndef H_DANTYPES
#define H_DANTYPES

/*
    WG14/N843 C99 Standard, Section 7.18

    These types are part of the ANSI C99 standards

    Currently, only the exact-width integer types and constants
    WG14/N843 (see C99 Section 7.18.1.1)
    change #if 0 to #if 1 if necessary - but let the user beware.
*/
#if 0
typedef signed   char  int8_t;
typedef signed   int   int16_t;
typedef signed   long  int32_t;
typedef signed long long int64_t;
typedef unsigned char  uint8_t;
typedef unsigned int   uint16_t;
typedef unsigned long  uint32_t;
typedef unsigned long long uint64_t;
#endif

/* end of Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */

//Rest not ANSI C99 but useful types and macros
#ifndef BOOL
typedef unsigned char  BOOL;    //where bool is not a native type */
#endif
#ifndef BYTE
typedef unsigned char  BYTE;
#endif
#ifndef WORD
typedef unsigned short WORD;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
#endif
#ifndef WORD64
typedef unsigned long long WORD64;
#endif
#ifndef QUAD
typedef unsigned long long QUAD;
#endif

#define MALLOC(x)       ((x *) malloc(sizeof(x)))
#define CALLOC(n, x)    ((x *) calloc((n), sizeof(x)))
#ifdef __GNUC__
//{The following allows an array to be allocated on the Heap
//which can be accessed in a Pascal array manner.
//For instance if I wanted an array with indexed from -3..5 of int.
// int *array
// ARRAY(array, -3, 5);
// to free use: free(array-3)
// (another version for not gnuc where the cast is coded would work)
//} this is slick, but some people would not like this
#define ARRAYAUX(b, l, u)\
    b= (typeof(*b)*) (calloc((sizeof(*b)), (u - l + 1))) + l
#define ARRAY(base, lower, upper) ARRAYAUX(base, (lower), (upper))
#endif
//{Now the versions that work with arrays allocated as static or automatic
//   float rawarray[ARRAYSZ(-3,9)];
//   float *array
//   array = ARRAYFIX(rawarray,-3,9);
// reference as needed array[-3], array[-2], array[-1], array[0], array[1],
// array[2], array[3], array[4], array[5], array[6], array[7], array[8],
//}array[9].
#define ARRAYSZ(ll,ul) ((ul)-(ll)+1)
#define ARRAYFIX(b, ll, ul) (b + ll)

#ifndef    LENGTHOF
#define LENGTHOF(array) (sizeof(array) / sizeof (array[0]))
#endif

#ifndef    lengthof
#define lengthof(array) (sizeof(array) / sizeof (array[0]))
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

//{Be forewarned about BIT with GNU compiler. if bitno is a variable
//>= 32 the shift operator will rotate the bit around like a modulo 32
// shift. Which is how the hardware works. If a constant == 32 is employed
// you get compile time warnings, but the correct result (when fed into BITS)
// using 1ULL stops the warnings but has other problems.
//the ? operator while sometimes inefficient solves the problem. It is never
//} inefficient with a constant bitno (which is the majority of it's usage).
#define ULL unsigned long long
#define BIT(bitno)           ((bitno) >= 64 ? 0uLL : (1uLL << (bitno)))
#define BITS(hi,lo)          (BIT((hi)+1) - BIT(lo))
#define MASK(lowbitno,nbits) (BIT((lowbitno)+(nbits)) - BIT(lowbitno))
#define _ROT32L(x,c)         ((x >> (32-c)) | (x << c))
#define ROT32L(dw,n)         _ROT32L((unsigned long)(dw),(unsigned long)(n))
#define _ROT32R(x,c)         ((x << (32-c)) | (x >> c))
#define ROT32R(dw,n)         _ROT32R((unsigned long)(dw),(unsigned long)(n))
#define _ROT64L(x,c)         ((x >> (64-c)) | (x << c))
#define ROT64L(dw,n)         _ROT64L((ULL)(dw),(ULL)(n))
#define _ROT64R(x,c)         ((x << (64-c)) | (x >> c))
#define ROT64R(dw,n)         _ROT64R((ULL)(dw),(ULL)(n))
//BITVAL is for unsigned fields for it does not propigate the sign bit
#define BITVAL(val,lowbitno,nbits) (((val) >> (lowbitno)) & MASK(0, (nbits)))
#define CLEARBIT(lval, bit) lval &= (~(bit))
#define SETBIT(lval, bit)   lval |= (bit)
//BITVALSGN is for signed use and will propagate the sign bit. It also
//  assumes a 32 bit holding arithmetic. A 64 bit version is straight
//  forward.
#define BITVALSGN(val,lowbitno,nbits)   ((val << (32-lowbitno-nbits)) >> (32-nbits))
#define BITVALSGN64(val,lowbitno,nbits) ((val << (64-lowbitno-nbits)) >> (64-nbits))

// LOWESTBITSET Just to round out the fielc
#define LOWESTBITSETHELPER(x) ((-x) & x)
#define LOWESTBITSET(X) LOWESTBITSETHELPER(((signed) (x)))
//Define signed division by 2^n  ----- x must have the attribute signed
#define SIGNEDDIV_2totheN(x,n) (((x) + (((x) >> 31) & MASK(0,(n)))) >> (n))
#define SIGNEDDIV_2(x)  (((x) - ((x) >> 31)) >> 1)

//{ Care must be taken with all the MIN or MAX Macros not to
//use with an argument which has a side effect when evaluated
// e.g. MIN(x++, y--) is a no no!!!
//}For GNU C++ only could use <? and >? operators side effects would not matter
#ifdef __GNUC__
#define MIN(x,y)                  ((x) <? (y))
#else
#define MIN(x,y)                  ((x) < (y) ? (x) : (y))
#endif

#define MIN3(x,y,z)               (MIN(MIN((x),(y)),(z)))

#ifdef __GNUC__
#define MAX(x,y)                  ((x) >? (y))
#else
#define MAX(x,y)                  ((x) > (y) ? (x) : (y))
#endif

#define MAX3(x,y,z)               (MAX(MAX((x),(y)),(z)))

#define LOWBOUNDED(lo, x)         (MAX((lo), (x)))
#define HIGHBOUNDED(x, hi)        (MIN((x), (hi)))
#define LOWHIGHBOUNDED(lo, x, hi) (HIGHBOUNDED(LOWBOUNDED((lo), (x)), (hi)))

#define ABS(x) MAX((x),-(x))

// For floating point approximately equal
#define APPROXHELPER(x, y, eps) ((y > (x-eps)) && (y < (x+eps)))
#define APPROX(x, y, eps) APPROXHELPER((x),(y), (MAX((ABS(x),ABS(y))*ABS(eps))))

// where a & b are bool or BOOL
// allow a IMPLIES b
#define IMPLIES <=

//{BITCOUNT(x) how many bits of x are a '1'. count the one bits.
//This is not the fastest algorithm for a population count
//} it is however pretty good.
#define BX_(x) ((x) - (((x)>>1)&0x77777777) \
- (((x)>>2)&0x33333333) \
- (((x)>>3)&0x11111111))
#define BITCOUNT(x) (((BX_(x)+(BX_(x)>>4)) & 0x0F0F0F0F) % 255)

/*
    functions are normally public, for readibility only
      PUBLIC  void foobar(void)
      PRIVATE void foobar(void)
*/
#define PUBLIC

enum    { NO,        YES         };
enum    { FALSE,     TRUE        };
enum    { SUCCESS,   FAIL        };
enum    { OFF,       ON          };
enum    { DISABLE,   ENABLE      };
enum    { NEVER,     FOREVER     };
enum    { STATUS_OK, STATUS_ERROR};

#ifndef OK
#define OK      0
#endif

/* the following 8 macros turn r-values into l-values R is for reference     */
/* especially useful to accessing pieces of address space by constant address*/
/* U_CR unsigned char reference       */
/* U_SR unsigned short reference      */
/* U_LR unsigned long reference       */
/* U_LLR unsigned long long reference */
/* S_CR signed char reference         */
/* S_SR signed short reference        */
/* S_LR signed long reference         */
/* S_LLR signed long long reference   */
#define U_CR(x)  (*(volatile unsigned char  *)     (x))
#define U_SR(x)  (*(volatile unsigned short *)     (x))
#define U_LR(x)  (*(volatile unsigned long  *)     (x))
#define U_LLR(x) (*(volatile unsigned long long *) (x))
#define S_CR(x)  (*(volatile signed   char  *)     (x))
#define S_SR(x)  (*(volatile signed   short *)     (x))
#define S_LR(x)  (*(volatile signed   long  *)     (x))
#define S_LLR(x) (*(volatile signed   long long *) (x))

//Control character Macro:: CONTROL('L') produce the value be ^L = 0x0C = FF
#define CONTROL(ch) ((ch) - '@')

//Nail to the ground here
// PC or X86 is LITTLE ENDIAN as defined on our target system

//workaround for BloodShed not implementing printf specifier "%016llX" properly
// latest gcc v 4.9.2 handles "%016llx" but not "%016ullx" LLFIX obsolete
//LITTLE ENDIAN fix only. ex: printf("%08X%08X", LLFIX(x));
#define LLFIX(z) ((union llfixunion)(z)).ul[1], ((union llfixunion)(z)).ul[0]
union llfixunion
{
      signed long long ll;
      unsigned long long ull;
      unsigned long ul[2];
};

// htons host order to network order short
#ifndef htons
#define htons(A) ((((A) & 0xff00) >> 8) | ((A) & 0x00ff) << 8))
#endif

// htonl host order to network order long
#ifndef htonl
#define htonl(A) ((((A) & 0xff000000) >> 24) | (((A) & 0x00ff0000) >> 8) | \
(((A) & 0x0000ff00) << 8) | (((A) & 0x000000ff) << 24))
#endif

// ntohs network order to host order short
#ifndef ntohs
#define ntohs htons
#endif

// ntohl network order to host order long
#ifndef ntohl
#define ntohl htonl
#endif

#ifdef __GNUC__
//typeof is a GNU specific feature
//SWAP(a,b) it swaps a with b using very local swaptemp for the scope of swap
#define SWAP(x,y) {typeof(x) swaptemp; swaptemp = y; y = x; x = swaptemp;}
#endif /* __GNUC__ */

#ifdef  DEBUG
//{following effective on a testing on P.C. for UNITTEST.
//e.g. if x were 3 DUMPW(x) would print <newline>x = 0003 = 3 = 3
//}the argument of DUMP{B|W|L} should be a variable reference w/o side effects
#define DUMPBXXX(x,y) printf("\n%30s = %02X = %u = %d", #x, y, y, x)
#define DUMPWXXX(x,y) printf("\n%30s = %04X = %u = %d", #x, y, y, x)
#define DUMPB(x) DUMPBXXX(x, (x) & 0xff)
#define DUMPW(x) DUMPWXXX(x, (x) & 0xffff)
// #define DUMPL(x) printf("\n%30s = %08lX = %lu = %ld", #x, x, x, x)
#define DUMPL(x) printf("\n%30s = %08X = %u = %d", #x,(int) x,\
(unsigned int)x, (int)x)
#define DUMPQ(x) printf("\n%30s = %16Xll = %ull = %lld", #x, (unsigned) x. \
(unsigned long long)x, (signed long long)x)
#define PRIVATE         /* force name into the linkstep      */
#define ASSERT(x) if(!(x))\
        {\
        printf("\nASSERTION FAILURE: %s %u\nASSERT(%s)\n",\
                __FILE__, __LINE__, #x);\
        }
#else
#define PRIVATE static  /* name/ref not passed into linkstep    */
                        /* assertions and dump debug statements */
                        /* not compiled unless DEBUG is defined */
#define ASSERT(x)
#define DUMPB(x)
#define DUMPW(x)
#define DUMPL(x)
#endif

#endif /* H_DANTYPES */
