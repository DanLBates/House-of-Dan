//SignedMagic16.cpp
//hacker's delight pg 174

#include <iostream>
#include <cctype>
#include <cstdlib>

using namespace std;

struct ms{
    unsigned short M; //Magic number
    short  s;         //Shift amount
};

struct ms magic(short d)        // Must have 2 <= d <= 2**15-1
{                               // or -2**31 <= d <= -2
    short p;
    unsigned short  ad, anc, delta, q1, r1, q2, r2, t;
    const unsigned short two15 = 0x8000;        //2**15
    struct ms mag;

    ad = abs(d);
    t = two15 + ((unsigned) d >> 15);
    anc = t - 1 - t % ad;               //absolute value of nc.
    p = 15;                             // Init. p.
    q1 = two15/anc;                     // Init. q1 = 2**p/|nc|.
    r1 = two15 - q1 * anc;              // Init. r1 = rem(2**p, |nc|)/
    q2 = two15 / ad;                    // Init. q2 = 2**p/|d|.
    r2 = two15 - q2*ad;                 // Init. r2 = rem(2**p, |d|).
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
    mag.s = p - 16;                     // shift amount to return
    return mag;
}

int main(void)
{
struct ms answer;
short d;
long ld;
short test;
long testll;
char ch;

while(1)
    {
    cout << "Enter constant divisor:";
    cin >> ld;
    if(0 == ld)
            return 999;
    d = ld;
    test = abs(d);
    if((ld < -32768) || (ld > 32767))
        {
        cout << endl << "bad divisor must be >= -32768 or < 32767" << endl;
        continue;
        }
    if((d > -2) && (d < 2))
        {
        cout << endl << "bad divisor must be >= 2 or <= -1" << endl;
        continue;
        }
    if((-test & test) == test)
        {
        cout << " this is a power of two " << endl;
        cout << " the notation >>s mean shift right signed" << endl;
        cout << " the notation >>u mean shift right unsigned" << endl;
        if(d < 0)
            cout << "negate n " << endl;
        cout << " t = ((n >>s k) >>u (16 - k))" << endl;
        cout << "q = (n + t) >>s k" << endl;
        continue;
        }
    cout << endl;
    answer = magic(abs(d));
    cout << "magic = " << answer.M << " shift = " << answer.s << "         0x"
            << hex << answer.M << dec << " " << endl;
    cout << "Do you want to test?";
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
            testll = testll >> (16 + answer.s);
            if(test < 0)
                testll++;
            if(d < 0)
                testll = - testll;
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
    cout << "Working with signed short (16 bit) quantities" << endl;
    cout << "Multiply x by " << answer.M << endl;
    cout << "then signed shift that result right by "<< (answer.s+16)
                << " bits"<< endl;
    cout << "If the result is negative then increment the result." << endl;
    if(d < 0)
        cout << "Negate the result." << endl;
    cout << "This will be equivalent to dividing x by " << d << endl;
    }
return 0;
}
