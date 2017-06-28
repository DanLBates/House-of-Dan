//SignedMagic.cpp
//hacker's delight pg 174

#include <iostream>
#include <cctype>
#include <cstdlib>

using namespace std;

struct ms{
    unsigned long M;     //Magic number
    int  s;     //Shift amount
};

struct ms magic(long d)        // Must have 2 <= d <= 2**31-1
{                               // or -2**31 <= d <= -2
    int p;
    unsigned long ad, anc, delta, q1, r1, q2, r2, t;
    const unsigned long two31 = 0x80000000L;        //2**31
    struct ms mag;

    ad = abs(d);
    t = two31 + ((unsigned) d >> 31);
    anc = t - 1 - t % ad;               //absolute value of nc.
    p = 31;                             // Init. p.
    q1 = two31/anc;                     // Init. q1 = 2**p/|nc|.
    r1 = two31 - q1 * anc;              // Init. r1 = rem(2**p, |nc|)/
    q2 = two31 / ad;                    // Init. q2 = 2**p/|d|.
    r2 = two31 - q2*ad;                 // Init. r2 = rem(2**p, |d|).
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
    mag.s = p - 32;                     // shift amount to return
    return mag;
}

int main(void)
{
struct ms answer;
long d;
long test;
long long testll;
char ch;

while(1)
    {
    cout << "Enter constant divisor:";
    cin >> d;
    test = abs(d);
    if((d > -2) && (d < 2))
        {
        cout << endl << "bad divisor must be >= 2 or <= -1" << endl;
        return 999;
        }
    if((-test & test) == test)
        {
        cout << " this is a power of two " << endl;
        if(d < 0)
            cout << "negate n " << endl;
        cout << " t = ((n >>s k) >>u (32 - k))" << endl;
        cout << "q = (n + t) >>s k" << endl;
        continue;
        }
    cout << endl;
    answer = magic(d);
    cout << "magic = " << answer.M << "L shift = " << answer.s << "         0x"
            << hex << answer.M << dec << "L" << endl;
    cout << "Do you want to test (be aware this takes a while)?";
    cin >> ch;
    cout << endl;
    ch = toupper(ch);
    if(ch == 'Y')
        {
        test = 1;
        while(test)
            {
            testll = test;
            testll = (testll * answer.M);
            testll = testll >> (32 + answer.s);
            if(test < 0)
                testll++;
            if(testll != (test / d))
                {
                cout << " whoops " << test << "  0x" << hex << test << dec <<
                   " " << test/d << endl;
                cout << testll << hex << "   0x" << testll << dec << endl;
                break;
                }
            test++;
            }
            cout << "done" << endl;
        }
    }
return 0;
}
