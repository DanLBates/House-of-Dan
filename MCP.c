/*============================================================================*\
| MCP optpracticefilename                                                      |
|                                                                              |
| Morse code practice. Uses file either explicitly specified or implicit       |
|   default of MCP.txt                                                         |
|                                                                              |
| Uses Beep multimedia interface.                                              |
| runs in DOS window with a single character to drive a menu.                  |
|                                                                              |
| Developed by Dan Bates        29-Apr-2017 2:28:22 pm                         |
|                                                                              |
| Licensed for use by Gnu Public License zzzzzzzzz                             |
|                                                                              |
| While a fancy window interface is not incorporated in the Internation Morse  |
| Code Practice Program, it allows the following: Slower wait for response tim-|
| ing. This let's you memorize the characters a little slower. With the other  |
| programs you have to have Morse Code pretty well down pat before practicing. |
| You have the source - so you can customize this.                             |
| You also can Play "P" instead of Test "T" and copy full speed on paper.      |
|                                                                              |
| This program does not elimnate the need to listen to and practice with the   |
| multitude of audio files available on the Inter Net.                         |
|                                                                              |
| The use of kbhit is problematic. This is not a portable function. There      |
| is no GNU or POSIX equivalent.                                               |
|                                                                              |
\*============================================================================*/
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "MorseTable.h"

#define  MAXSTRINGS 2000

// Please change DATETIME and BASEREV when you change code!!!
#define BASEREV "0.1"
// While developing you can change following #if 1 to #if 0
#if 1
#define REV BASEREV
#define DATETIME "23-Aug-2017"
#else
#define REV BASEREV"+development"
#define DATETIME __DATE__ " " __TIME__
#endif
#ifdef __GNUC__
#pragma message (REV" "DATETIME" @ "__DATE__" "__TIME__)
#endif

#define CONTROL(x) (x - '@')
#define BOOL    unsigned char

static unsigned primes[] = {7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
                            59, 61, 67, 71, 73, 79, 83};

#define NPRIMES (sizeof(primes)/sizeof(unsigned))

unsigned hertz;                 // tone for morse code elements typically 600Hz
unsigned wordsPerMinute;        // How many times to send word "CODEX " in 1 minute
unsigned NumberofStrings;       // How many strings in phrase file read in at init.
unsigned ErrorCounts;           // Error beeps under test.
int dittime;                    // The time for one di element a function of WPM.
int dahtime;                    // The time for one dah element typ 3*dittime
int elementspace;               // Space between elements. typ = dittime
int characterspace;             // Space between characters typ = dahtime
int wordspace;                  // Space between words typ 7*dittime
int selectPhrase;               // number of phrase to play or test with.
int initSelectPhrase;       // when we go thru all - we are back here.
int selectSkip;                 // A prime number to skip each time so that
                                // we do not repeat until all phrases are used.
int TestCount;                  // Count number of tests here
int PlayCount;                  // Number of Play's

char     *practiceStrings[MAXSTRINGS];

unsigned char BulkMessageMode;  // To use with practice messages which end with ^Z
unsigned char dodisplaymenu;    // To display menu & cleared as Menu displayed

void myabort (char *msg, int errorCode) // something bad happened, we will exit.
{//abort
printf("\n%s abort code:%d\n", msg, errorCode);
exit(errorCode);
}//abort

BOOL Yorn (char *msg)  // Asks a question & only take yes/no replys
{//Yorn
char what;
while(TRUE)
    {//keep at it till we see [YyNn]
        printf("\n%s", msg);
        what = toupper(getch());        // y or n in any case
        if('Y' == what)
            break;
        if('N' == what)
            break;
        printf("\tResponse must be 'Y' or 'N'"); // bad reply
    }//keep at it till we see [YyNn]
printf("%c\n", what);             // show what the reply was
return ('Y' == what);   // return boolean true if yes, false if no.
}//Yorn

unsigned SetParamsFromWPM (unsigned wpm)
{//Parameters from words per minute
 // Now from WPM figure out the timings for elements and spaces
    dittime        = 1000 / wpm;
    dahtime        = 3 * dittime;
    elementspace   =     dittime;
    characterspace = 3 * dittime;
    wordspace      = 7 * dittime;
    TestCount      = 0;
    return wpm;
}//Parameters from words per minute

void Init (char *initfilename)
{//Init
    FILE *filehandle;
    char myline[220], *ptrtomyline;
    int  linelength;
    // display a banner line.
    BulkMessageMode = 0;            // initially not in bulk message mode
    filehandle = fopen(initfilename, "r");  // open the phrase file
    if(filehandle == NULL)
        myabort(initfilename, 11);
    hertz = wordsPerMinute = 0;
    fscanf(filehandle,"Hz=%d,WPM=%u\n", &hertz, &wordsPerMinute); //1st hertz & wpm
    if((wordsPerMinute == 0)|| (hertz == 0))
        myabort("Need Hz=600,WPM=6", 16);
    NumberofStrings = 0;    // Don't have any practice strings yet
    // Now to get the practice strings
    while(!feof(filehandle))
        {// Reading practice strings
        ptrtomyline = fgets(myline, sizeof(myline), filehandle);
        if(feof(filehandle))
            break;
        if(NULL == ptrtomyline)
            myabort("could not open the phrase file!", 12);
        if(MAXSTRINGS <= NumberofStrings)
            myabort("Not enough string space to hold all practice strings", 13);
        linelength = strlen(myline); //need terminating zero but not new line
        myline[linelength-1] = '\0';

        if(linelength <= 0)
            myabort("Bad string read", 14);
        practiceStrings[NumberofStrings] = malloc(linelength+2);    // get space to copy
        if(NULL == practiceStrings[NumberofStrings])
            myabort("Can't malloc space for practice string", 15);
        strncpy(practiceStrings[NumberofStrings], myline, linelength);
        NumberofStrings++;
        }// Reading practice strings
    wordsPerMinute = SetParamsFromWPM(wordsPerMinute);
    //
    // Seed the random number generator with time. Comment out for debug.
    //
    srand(time(NULL));
    // close the phrase file
    fclose(filehandle);
    if(*(practiceStrings[NumberofStrings - 1]) == '$')
        {// Message is bulk probably a ARRL practice message
            BulkMessageMode = 1;
            printf("Bulk Message Mode\n");
            selectPhrase = 0;
        }
}//Init

void DisplayMenu (void)
{//DisplayMenu
        // First tell a little about our configuration.
    printf("\nMCP %s %s", REV, DATETIME);
    printf("\nNumber of Strings = %u", NumberofStrings);
    printf("\nWords Per Minute = %u", wordsPerMinute);
    printf("\n dittime=%d dahtime=%d elementspace=%d charspace=%d wordspace=%d",
           dittime, dahtime, elementspace, characterspace, wordspace);
    printf("\n #-Plays=%d #-Tests=%d Errors=%u",
           PlayCount, TestCount, ErrorCounts);
            // Now the Menu. We accept at this level the character w/o control
    printf("\n^A Show the answer phrase          ^C Control vars");
    printf("\n^E Echo back phrase                ^H Help with character");
    printf("\n^Q quit/exit                       ^M this menu");
    printf("\n^P Play All                        ^R Resend last character");
    printf("\n^S select a test phrase Randomly   ^T Test you one char at a time");
    printf("\n^V Show Error Count                ^X quit/exit");
    printf("\n^Z Whatever Test function we need! 0  Zero the Test Count.\n");
    printf("\n");
    dodisplaymenu = FALSE;
}//DisplayMenu

/*============================================================================*\
|                               ShowErrCount                                   |
\*============================================================================*/
void ShowErrCount (void)
{//ShowErrCount
    printf("\nErrors=%u\n", ErrorCounts);
    if(ErrorCounts)
        if(Yorn("Zero Error count?"))
            ErrorCounts = 0;
}//ShowErrCount

/*============================================================================*\
| To display a quoted string for instructual purposes                          |
\*============================================================================*/
char * DisplayQuotedString(char *ptrchar)
{//DisplayQuotedString
    char TheChar;
    printf("\n");
    while(TRUE)
    {//till eos or another "'"
        TheChar = *ptrchar;
        if(TheChar == 0)    //Just in case there is no closing single quote
            break;
        ptrchar++;
        if('\'' == TheChar) //When there is a closing quote
            break;
        printf("%c", TheChar);
    }//till eos or another "'"
    return ptrchar; // ptrchar points to the zero in zero terminated string
                    // or to just past the quote
}//DisplayQuotedString

/*============================================================================*\
|       Send one single character all elements and spacings.                   |
\*============================================================================*/
void PlayChar (char theChar)
{//PlayChar
    unsigned char coding;
    if(theChar == ' ')   // All we do for space is wait wordspace milliseconds
    {
       Sleep(wordspace); // and we are done
       return;
    }
        coding = MorseTable[theChar];
    if(0 == coding)
    {//A char we cannot play
        printf("%c", theChar);
        return;
    }//A char we cannot play
    while (coding != 1)
    {// bit by bit
        if((coding & 1) == 1)
        {//Perform Dah
            Beep(hertz, dahtime);    //Morse code dah
            /*
            The following commented out line should be present.
            However Beep(hertz, dahtime) seems to have a pause of its own!
            Sleep(elementspace);     //This long after Dah
            */
        }//Perform Dah
        else
        {//Perform Dit
           Beep(hertz, dittime);    // Morse code dit
           Sleep(elementspace);     //This long after dit
        }//Perform Dit
        coding >>= 1;
    }// bit by bit
    Sleep(characterspace);
}//PlayChar

/*============================================================================*\
|  Send a phrase (i.e. a zero terminated string. To do this we send each and   |
|  every character of the string.                                              |
\*============================================================================*/
void PlayPhrase (char *phrase)
{//PlayPhrase
    char *ptrchar;                  // this point in string to next char to play
    char theChar;                                   // and this we extract to send.
    ptrchar = phrase;
    while((theChar = *ptrchar++) != '\0')
    {
        if(theChar == '\'')
        {//Some CAI text to show
                ptrchar = DisplayQuotedString(ptrchar);
                printf("\n");
                continue;
        }//Some CAI text to show
        PlayChar(theChar);
        if(kbhit())             // Striking any key aborts the phrase output
        {// abort the playing of phrase
            printf("\nPlay function aborted!!!\n");
            return;
        }// abort the playing of phrase
    }
}//PlayPhrase

/*============================================================================*\
| time the Play phrase and count the words                                     |
\*============================================================================*/
double timePlayPhrase (char *phrase, float *seconds, int *wordcount)
{//timePlayPhrase
    clock_t the_time;
    float secs;
    double wpm;
    int words;
    unsigned char in_word;

    the_time = clock();
    PlayPhrase(phrase);
    the_time = clock() - the_time;
    secs = ((float) the_time) / CLOCKS_PER_SEC;
    words = 0;  in_word = FALSE;
    while(*phrase)
    {//count blocks of non-space
        if(!in_word)
        {//currently looking at spaces
            if(' ' != *phrase)
            {//inside word
                in_word = TRUE;
                words++;
            }//inside word
        }//currently looking at spaces
        else
        {//currently looking at non-spaces
            if(' ' == *phrase)
                in_word = FALSE;
        }//currently looking at non-spaces
        phrase++;
    }//count blocks of non-space

    *seconds   = secs;
    *wordcount = words;
    wpm        = words;
    wpm        = wpm/secs * 60.0;
    return wpm;
}//timePlayPhrase

/*============================================================================*\
| The User wants to hear the selected [selectPhrase] phrase                    |
\*============================================================================*/
void PlayAll (void)
{//PlayAll
    unsigned i;
    char     ch;
    double   wpm = 0.0;
    float    seconds;
    int      wordcount;

    if(BulkMessageMode)
        for(i=0; i < NumberofStrings; i++)
            wpm = timePlayPhrase(practiceStrings[i], &seconds, &wordcount);
    else
        wpm = timePlayPhrase(practiceStrings[selectPhrase],&seconds,&wordcount);
    PlayCount++;
    if(1 == PlayCount)
        ch = ' ';
    else
        ch = 's';
    printf("\n%d Phrase%c %d words in %5.2f seconds Played wpm=%5.2f\n",
           PlayCount, ch, wordcount, seconds + 0.005, wpm + 0.005);
}//PlayAll

/*============================================================================*\
| The User wants to cheat and see the selected phrase.                         |
\*============================================================================*/
void ShowLine (void)
{
    char *ptrchar;
    char theChar;
    unsigned i;
    if(BulkMessageMode)
        {// show all
            printf("\n");
            for(i=0; i < NumberofStrings; i++)
                printf("%s\n", practiceStrings[i]);
            return;
        }
    ptrchar = practiceStrings[selectPhrase];
    printf("\n%s\n", ptrchar);
}

/*============================================================================*\
| Of all the strings we know - we have been asked to choose one.               |
\*============================================================================*/
void SelectAPracticePhrase (void)
{//SelectAPracticePhrase
    static char needinit = 1;
    if(BulkMessageMode)
    {// Bulk Message Mode 'S' or ^S not allowed
        selectPhrase = 0;
        printf("In Bulk Message Mode this just selects the one phrase again\n");
        return;
    }
    if(needinit)
    {//first time thru
        // selectSkip must be relatively prime to NumberofStrings.
        // We pick a prime from a list randomly and then make sure that
        // NumberofStrings is not divisible by the prime.
        selectPhrase = rand() % NumberofStrings;
        selectSkip   = rand() % NPRIMES;
        initSelectPhrase = selectPhrase;
        while(0 == (NumberofStrings % primes[selectSkip]))
            selectSkip = (selectSkip + 1) % NPRIMES;
        selectSkip = primes[selectSkip];
        needinit = 0;
    }//first time thru
    else
    {//after first time through
        selectPhrase = (selectPhrase + selectSkip) % NumberofStrings;
        if(selectPhrase == initSelectPhrase)
        {//Been thru them all. We have wrapped around.
         // Now for a slight variation of pattern.
            selectPhrase++;
            if(selectPhrase >= NumberofStrings)
                selectPhrase = 0;
            initSelectPhrase = selectPhrase;
        }//Been thru them all. We have wrapped around.
    }//after first time through
    printf("Phrase selected. P/T/A>>>\n");
}//SelectAPracticePhrase

/*============================================================================*\
| The User wants to be tested on the string.                                   |
\*============================================================================*/
void TestPhrase (char *phrase)
{//TestPhrase
    char *ptrchar;
    char theChar;
    char guess;
    unsigned char bad = FALSE;
    ptrchar = phrase;
    while((theChar = *ptrchar) != '\0')
    {
        if('\'' == theChar)
        {// quoted to display something. A crude CAI
                ptrchar = DisplayQuotedString(++ptrchar);
                printf("\n");
                continue;
        }// quoted to display something. A crude CAI
        if(' ' == theChar)
        {//Don't worry about spaces in test mode right now
                printf(" ");    // Just show them
                Sleep(characterspace);
                ptrchar++;      // and go to next
                continue;
        }//Don't worry about spaces in test mode right now
        if(0 == MorseTable[theChar])
        {// No coding for this character
            printf("%c", theChar);  //We give this character away for free
            ptrchar++;              //Skip it, as we cannot play it.
            continue;
        }// No coding for this character
        if(!bad)    //Don't replay until told to
            PlayChar(theChar);
        bad = FALSE;
        guess = toupper(getch());
        if(CONTROL('R') == guess)
            continue;
        if(CONTROL('H') == guess)
        {//help me w/one character
            printf("%c", tolower(theChar));
            ptrchar++;
            continue;
        }//help me w/one character
        if((CONTROL('Q') == guess) || (CONTROL('X') == guess))
            return; // This guy is tired of the test so return back.
        else if('!' == guess)
        {// Hidden command the error count decremented!
            if(0 < ErrorCounts)
            {// Back down error count
                ErrorCounts--;
                continue;
            }// Back down error count
        }// Hidden command the error count decremented!
        if(guess != theChar)
        {//bad answer
            Beep(350,500);  // a boop to tell user he is wrong.
            ErrorCounts++;  // Even if it was only a finger fumble.
            bad = TRUE;
            continue;
        }//bad answer
        printf("%c", guess);
        Sleep(250);
                ptrchar++;
    }
}//TestPhrase

/*============================================================================*\
| The User wants to be tested on the selected phrase.                          |
\*============================================================================*/
void TestYou (void)
{//TestYou
    unsigned i;
    ErrorCounts = 0;
    if(BulkMessageMode)
        for(i=0; i < (NumberofStrings-1); i++)
        {
            TestPhrase(practiceStrings[i]);
            printf("\n");
        }
    else
        TestPhrase(practiceStrings[selectPhrase]);
    printf("\nTest %d Complete.\n", ++TestCount);
}//TestYou

/*============================================================================*\
| This is whatever we need at the time. for testing.                           |
\*============================================================================*/
void Z (void)
{//Z
    printf("\n%d", time(NULL));
}//Z

/*============================================================================*\
| User types and we play or test or both as desired.                           |
\*============================================================================*/
void Echo (void)
{//Echo
    char line[500];
    char *ptrchar;
    short indexi;
    char  chartoupper;
    printf("\nEnter phrase:");
    gets(line);
    ptrchar = line;
        while(*ptrchar = toupper(*ptrchar))
                ptrchar++;
    if(Yorn("Echo this?"))
    {
        printf("\n");
        PlayPhrase(line);
    }
    printf("\n");
    if(Yorn("Test with this?"))
    {
        printf("\n");
        TestPhrase(line);
        printf("\nTest of Echo Complete.\n");
    }
    else
        printf("\n");
}//Echo

unsigned char ParamChanged; // a side effect from MaybeEditParm

void MaybeEditParm (int *Parm, const char ParmString[])
{//MaybeEditParm
    char promptString[80];
    ParamChanged = 0;
    snprintf(promptString, 80, "%s:%d Change Yes or No?", ParmString, *Parm);
    if(Yorn(promptString))
    {// Yes we edit Parm
        printf("%s:", ParmString);
        scanf("%d", Parm);
        ParamChanged = 1;
    }// Yes we edit Parm
}//MaybeEditParm

void ControlVars (void)
{//ControlVars
    MaybeEditParm(&hertz,          "Hertz for tone");
    MaybeEditParm(&wordsPerMinute, "Words Per Minute");
    if(ParamChanged)
        wordsPerMinute = SetParamsFromWPM(wordsPerMinute);
    MaybeEditParm(&dittime,        "Dittime");
    MaybeEditParm(&dahtime,        "Dahtime");
    MaybeEditParm(&elementspace,   "Element Space");
    MaybeEditParm(&characterspace, "Character Space");
    MaybeEditParm(&wordspace,      "Word Space");
    printf("\nControl Vars Done\n");
}//ControlVars

void MasterControlLoop (void)
{//MasterControlLoop
    int cmd;
    selectPhrase = 0;
    DisplayMenu();
    while(TRUE)
    {//Looking for a command
        if(dodisplaymenu)
            DisplayMenu();
        cmd = toupper(getch());
        if((CONTROL('A') == cmd) || ('A' == cmd))   // Show Answer
            ShowLine();
        else if((CONTROL('C') == cmd) || ('C' == cmd)) // Control vars
            ControlVars();
        else if((CONTROL('E') == cmd) || ('E' == cmd)) // get a phrase and either
            Echo();         //  or test with it.    // echo it back, or test.
        else if((CONTROL('M') == cmd) || ('M' == cmd))  // Menu
            dodisplaymenu = TRUE;
        else if((CONTROL('Q') == cmd) || (CONTROL('X') == cmd) ||
                ('Q' == cmd)          || ('X' == cmd))
            exit(0);
        else if((CONTROL('P') == cmd) || ('P' == cmd)) // Play All.
            PlayAll();
        else if((CONTROL('S') == cmd) || ('S' == cmd)) // select new exercize
            SelectAPracticePhrase();
        else if((CONTROL('T') == cmd) || ('T' == cmd)) // test with exercize
            TestYou();
        else if((CONTROL('V') == cmd) || ('V' == cmd)) // Show Error Count
            ShowErrCount();
        else if((CONTROL('Z') == cmd) || ('Z' == cmd)) // try things - experiment
                Z();
        else if('0' == cmd)
        {//Zero the test count
            printf("\nTest Count & Play Count are both now zero.\n");
            TestCount = PlayCount = 0;
        }//Zero the test count
        else if('!' == cmd)
            ErrorCounts = 0;
    }//Looking for a command
    printf("\n Problem MasterControlLoop should never fall through and it did!\n");
    exit(0);
}//MasterControlLoop

int main (int argc, char *argv[])
{//main
    printf("\nMorse Code Practice program MCP %s\t%s\n", REV, DATETIME);
    if(argc != 2)
        myabort("Usage MCP [practicefile]", 10);
    if(argc == 2)
        Init(argv[1]);
    else
        Init("MCP.txt");
    MasterControlLoop();
    return 0;
}//main
