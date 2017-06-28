// Generates Morse Code Tables for MCP
#include <stdio.h>
#include <time.h>

unsigned char MorseDef[128];
char character;
char line[120];
unsigned short stack_top_index;
unsigned char stack[10];

void initMorseDef (void)
{
    short indx;
    for(indx=0; indx < sizeof(MorseDef); indx++)
        MorseDef[indx] = 0;
}

void MorseEncode (void)
{
    char ch;
    short cursor = 0;
    unsigned char newbit, bits;
    stack_top_index = 0;            //clean stack
    do
    {
        ch = line[cursor];          // examine one di or dah at a time
        if(ch == '.')
            stack[stack_top_index++] = 0; //remember di
        else if(('_' == ch) || ('-' == ch))
            stack[stack_top_index++] = 1; //remember dah
        else
            break;
        cursor++;
    }
    while(1);
    stack[stack_top_index++] = 1;   // The stop bit in the encoding
// Now recover and encode
    bits = 0;
    while(stack_top_index > 0)
    {
        newbit = stack[--stack_top_index];
        bits <<= 1;
        bits = bits | newbit;
    }
    MorseDef[character] = bits;
}

void MorseTableWrite (void)
{
    short table_index;
    FILE *morseout;
    time_t rawtime;
    time(&rawtime);
    morseout = fopen("MorseTable.h", "w");
    fprintf(morseout, "//MorseTable.h generated from MorseTones.txt %s",
              ctime(&rawtime));
    fprintf(morseout, "//MorseTones.txt is of the form\n");
    fprintf(morseout, "//A .-\n");
    fprintf(morseout, "//B -...\n");
    fprintf(morseout, "//C -.-.\n");
    fprintf(morseout, "//D -..\n");
    fprintf(morseout, "//  The . representing dit(s) the - representing dah(s)\n");
    fprintf(morseout, "// producing the following table\n");
    fprintf(morseout, "unsigned char MorseTable[128] = {\n");
    for(table_index=0; table_index < sizeof(MorseDef); table_index++)
    {
        if((table_index >= ' ') && (table_index < 127))
            if(table_index == '\\')
                fprintf(morseout,"/*\\\\*/");
            else
                fprintf(morseout, "/*%c*/ ", table_index);
        else
            fprintf(morseout, "      ");
        fprintf(morseout,"0x%02x", MorseDef[table_index]);
        if(table_index < 127)
            fprintf(morseout, ", ");
        if((table_index & 0x7) == 0x7)
            fprintf(morseout, "\n");
    }
    fprintf(morseout, "};\n");
    fclose(morseout);
}
int main (int argc, char *argv[])
{
    FILE *morsein;
    short index_i;
    initMorseDef();
    morsein = fopen("MorseTones.txt", "r");
    while(!feof(morsein))
    {
        fscanf(morsein, "%c %s\n", &character, line);
        MorseEncode();
    }
    fclose(morsein);
    for(index_i='A'; index_i <= 'Z'; index_i++)
        MorseDef[tolower(index_i)] = MorseDef[index_i];
    // Output the table
    MorseTableWrite();
}
