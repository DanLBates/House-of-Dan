//UnSignedMagic.cpp
//hacker's delight pg 185

#include <iostream>
#include <cctype>
#include <cstdlib>

using namespace std;

struct mu{
    unsigned long M;   //Magic number
    int  a;            // "add" indicator
    int  s;            //Shift amount
};

struct mu magicu(unsigned long d)      // Must have 1 <= d <= 2**31-1
{
    int p;
    unsigned long nc, delta, q1, r1, q2, r2;
    struct mu magu;

    magu.a = 0;                         // Initialize add indicator
    nc = - 1 - (-d) % d;                // unsigned arithmetic here.
    p = 31;                             // Init. p.
    q1 = 0x80000000L/nc;                // Init. q1 = 2**p/nc.
    r1 = 0x80000000L - q1 * nc;         // Init. r1 = rem(2**p, nc).
    q2 = 0x7FFFFFFFL / d;               // Init. q2 = (2**p - 1)/d.
    r2 = 0x7FFFFFFFL - q2*d;            // Init. r2 = rem(2**p - 1, |d|).
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
            if(q2 >= 0x7FFFFFFFL) magu.a = 1;
            q2 = 2*q2 + 1;
            r2 = 2*r2 + 1 - d;
            }
        else
            {
            if(q2 >= 0x80000000L) magu.a = 1;
            q2 = 2 * q2;                // Update q2.
            r2 = 2 * r2 + 1;            // Update r2 = rem(2**p, |d|).
            }
        delta = d - 1 - r2;
        } while (p < 64 &&
                 (q1 < delta || (q1 == delta && r1 == 0)));

    magu.M = q2 + 1;                    // Magic number and
    magu.s = p - 32;                    // shift amount to return
    return magu;
}

int main(void)
{
struct mu answer;
unsigned long d;
unsigned long test;
unsigned long long testll;
char ch;

while(1)
    {
    cout << "Enter constant divisor:";
    cin >> d;
    test = d;
    if(d < 2)
        {
        cout << endl << "bad divisor must be >= 2 or <= -1" << endl;
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
    cout << "magic = " << answer.M << "L shift = " << answer.s <<
            " add indicator = " << answer.a <<   "         0x"
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
            testll = testll >> 32;
            if(answer.a)
                testll += test;
            testll = testll >> answer.s;
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
        cout << "Working with unsigned long (32 bit) quantities" << endl;
        cout << "Multiply x by " << answer.M << endl;
        cout << "Take the high 32 bits of that 64 bit result " << endl;
        if(answer.a)
            cout << "then add x to that " << endl;
        cout << "then shift that result right by "<< answer.s << " bits"<< endl;
        cout << "This will be equivalent to dividing x by " << d << endl;
    }
return 0;
}
