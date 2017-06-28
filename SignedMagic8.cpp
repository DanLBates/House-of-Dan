//SignedMagic8.cpp
//hacker's delight pg 174

#include <iostream>
#include <cctype>
#include <cstdlib>

using namespace std;

struct ms{
    unsigned char M;  //Magic number
    char  s;          //Shift amount
};

struct ms magic(char d)         // Must have 2 <= d <= 2**8-1
{                               // or -2**8 <= d <= -2
    short p;
    unsigned char  ad, anc, delta, q1, r1, q2, r2, t;
    const unsigned char two7 = 0x80;        //2**7
    struct ms mag;

    ad = abs(d);
    t = two7 + ((unsigned) d >> 7);
    anc = t - 1 - t % ad;               //absolute value of nc.
    p = 7;                              // Init. p.
    q1 = two7/anc;                      // Init. q1 = 2**p/|nc|.
    r1 = two7 - q1 * anc;               // Init. r1 = rem(2**p, |nc|)/
    q2 = two7 / ad;                     // Init. q2 = 2**p/|d|.
    r2 = two7 - q2*ad;                  // Init. r2 = rem(2**p, |d|).
    do  {
        p = p + 1;
        q1 = 2 * q1;                    // Update q1 = 2**p/|nc|.
        r1 = 2 * r1;                    // Update r1 = rem(2**p, |nc|).
        if(r1 >= anc)                   // (Must be unsigned comparison here).
            {
            q1 = q1 + 1;
            r1 = r1 - anc;
            }
        q2 = 2 * q2;                    // Update q2 = 2**p/|d|.
        r2 = 2 * r2;                    // Update r2 = rem(2**p, |d|).
        if(r2 >= ad)                    // (Must be unsigned comparison here).
            {
            q2 = q2 + 1;
            r2 = r2 - ad;
            }
        delta = ad - r2;
        } while (q1 < delta || (q1 == delta && r1 == 0));

    mag.M = q2 + 1;
    if(d < 0) mag.M = - mag.M;          // Magic number and
    mag.s = p - 8;                      // shift amount to return
    return mag;
}

int main(void)
{
struct ms answer;
char d;
char test;
short testll;
short s;
unsigned short us;

while(1)
    {
    cout << "Enter constant divisor:";
    cin >> s;
    if(s == 0)
        return 999;
    if((s < -128) || s > 127)
        {
        cout << endl << "bad divisor must be >= -128 and <=12" << endl;
        continue;
        }
    d = s;
    test = abs(d);
    if((d > -2) && (d < 2))
        {
        cout << endl << "bad divisor must be >= 2 or <= -1" << endl;
        continue;
        }
    if((-128 == d) || (-test & test) == test)
        {
        cout << " this is a power of two " << endl;
        cout << " the notation >>s mean shift right signed" << endl;
        cout << " the notation >>u mean shift right unsigned" << endl;
        if(d < 0)
            cout << "negate n " << endl;
        cout << " t = ((n >>s k) >>u (16 - k))" << endl;
        cout << "q = (n + t) >>s k" << endl;
        cout << "\
#define SIGNEDDIV_2totheN(x,n) (((x) + (((x) >> 31) & MASK(0,(n)))) >> (n))"
        << endl;
        continue;
        }
    cout << endl;
    answer = magic(abs(d));
    cout << "magic = " << (us=answer.M) << " shift = " << (us=answer.s+8) <<
            "         0x" <<
            hex << (us=answer.M) << dec << " " << endl;
    cout << "We always test bytes!!!";
    cout << endl;
    test = 1;
    while(test)
        {
        testll = test;
        testll = (testll * answer.M);
        testll = testll >> (8 + answer.s);
        if(test < 0)
            testll++;
        if(d < 0)
            testll = -testll;
        if(testll != (test / d))
            {
            cout << " whoops " << (s=test) << "  0x" << hex << (s=test) << dec <<
               " " << (s=test)/d << endl;
            cout << testll << hex << "   0x" << testll << dec << endl;
            break;
            }
        test++;
        }
    cout << "done" << endl;
    cout << "Working with signed char (8 bit) quantities" << endl;
    cout << "Multiply x by " << (us=answer.M) << endl;
    cout << "then signed shift that result right by "<< (us=(answer.s+8))
                << " bits"<< endl;
    cout << "If x < 0 then increment result." << endl;
    if(d < 0)
        cout << "Then negate the result!" << endl;
    cout << "This will be equivalent to dividing x by " << (s=d) << endl;
    }
return 0;
}
