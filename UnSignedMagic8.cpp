//UnSignedMagic8.cpp
//hacker's delight pg 185

#include <iostream>
#include <cctype>
#include <cstdlib>

using namespace std;

struct mu{
    unsigned char M;  //Magic number
    char  a;          // "add" indicator
    char  s;          //Shift amount
};

struct mu magicu(unsigned char d)      // Must have 1 <= d <= 2**8-1
{
    short p;
    unsigned char nc, delta, q1, r1, q2, r2;
    struct mu magu;

    magu.a = 0;                         // Initialize add indicator
    nc = - 1 - (-d) % d;                // unsigned arithmetic here.
    p = 7;                              // Init. p.
    q1 = 0x80 /nc;                      // Init. q1 = 2**p/nc.
    r1 = 0x80 - q1 * nc;                // Init. r1 = rem(2**p, nc).
    q2 = 0x7F / d;                      // Init. q2 = (2**p - 1)/d.
    r2 = 0x7F - q2*d;                   // Init. r2 = rem(2**p - 1, |d|).
    do  {
        p = p + 1;
        if(r1 >= nc - r1)
            {
            q1 = 2 * q1 + 1;            // Update q1.
            r1 = 2 * r1 - nc;           // Update r1.
            }
        else
            {
            q1 = 2*q1;
            r1 = 2*r1;
            }
        if(r2 + 1 >= d - r2)
            {
            if(q2 >= 0x7F) magu.a = 1;
            q2 = 2*q2 + 1;
            r2 = 2*r2 + 1 - d;
            }
        else
            {
            if(q2 >= 0x80) magu.a = 1;
            q2 = 2 * q2;                // Update q2.
            r2 = 2 * r2 + 1;            // Update r2 = rem(2**p, |d|).
            }
        delta = d - 1 - r2;
        } while (p < 16 &&
                 (q1 < delta || (q1 == delta && r1 == 0)));

    magu.M = q2 + 1;                    // Magic number and
    magu.s = p - 8;                     // shift amount to return
    return magu;
}

int main(void)
{
struct mu answer;
unsigned char  d;
unsigned char  test;
unsigned short testll;
unsigned short us;
char ch;

while(1)
    {
    cout << "Enter constant divisor:";
    cin >> us; d = us;
    test = d;
    if(d < 2)
        {
        cout << endl << "bad divisor must be >= 2" << endl;
        return 999;
        }
    if((-test & test) == test)
        {
        cout << " this is a power of two " << endl;
        cout << "q = n >> k" << endl;
        continue;
        }
    cout << endl;
    answer = magicu(d);
    cout << "magic = " << (us=answer.M) << " shift = " << (us=answer.s) <<
            " add indicator = " << (us=answer.a) <<   "         0x"
            << hex << (us=answer.M) << dec << " " << endl;
    cout << "For bytes we always test!";
    cout << endl;
    test = 1;
    while(test)
        {
        testll = test;
        testll = (testll * answer.M);
        testll = testll >> 8;
        if(answer.a)
            testll += test;
        testll = testll >> answer.s;
        if(testll != (test / d))
            {
            cout << " whoops " << test << "  0x" << hex <<(us=test)<< dec <<
               " " << test/d << endl;
            cout << testll << hex << "   0x" << testll << dec << endl;
            break;
            }
        test++;
        }
        cout << "done" << endl;
        cout << "Working with unsigned char (8 bit) quantities" << endl;
        cout << "Multiply x by " << (us=answer.M) << endl;
        cout << "Take high byte of result" << endl;
        if(answer.a)
            cout << "then add x to that " << endl;
        cout << "then shift that result right by "<< (us=answer.s)
                        << " bits"<< endl;
        cout << "This will be equivalent to dividing x by " << (us=d) << endl;
    }
return 0;
}
