//Osnova assembly C compiler, version beta, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
9. October, 2026.
11. October, 2026.
12. October, 2026.
13. October, 2026.
14. October, 2026.
16. October, 2026.
17. October, 2026.
18. October, 2026.
19. October, 2026.
20. October, 2026.
21. October, 2026.
4. November, 2026.
*/

/*
TODO
nisi pokrio inlucde??? include rii i macro isto odma naa staartu, zaa svaaki inlude pokreni lexer funkciju
nemnoj napraviti bin buffer nego za to odmah pisi u file sa append
tribat ce ti lista svih deklaracijaa, maakro, varijaabli itd (hash map ili dict)
zaa deklaaracije, makro body i varijable koristi malloc obavezno iako mozda nije ucinkovito bas
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//FUNCTION DECLARATIONS

//File extension check, checks if 'fpath' has extension 'ext' ('Ext' max lenght is 5 characters including '\0')
int ext_check(char fpath[256], const char *ext);

//MAIN COMPILER FUNCTIONS

/* USER INPUT
Reads user input for selecting input and ouput files
*/
void user_input(char (*p_asm_fpath)[256], char (*p_bin_fpath)[256]);

/* FILE MANIPULATION
Checks output filepath,
opens selected files,
creates text buffer representation of the input file
*/
void file_manipulation(char p_asm_fpath[256], char p_bin_fpath[256], char** p_asm_fbuf, int* p_asm_fsize, FILE** p_fbin);

/* LEXER
Extracts tokens from text buffer,
filters empty tokens out
*/
void lexer(int* p_token_num, char ***p_token_array, int asm_fsize, char* asm_fbuf);

/* CODE GENERATOR
TODO
*/
void code_generator(int p_token_num, char** p_token_array);

//MAIN

char asm_fpath[256]; //Assembly file path
char* asm_fbuf = NULL;
int asm_fsize;

char bin_fpath[256]; //Binary file path
FILE* fbin = NULL;

char **token_array = NULL;
int token_num = 0;

int main()
{

    user_input(&asm_fpath, &bin_fpath);

    printf("\nPART 1, USER INPUT:\n");
    printf("Assembly input file path: %s\n", asm_fpath);
    printf("Binary output file path: %s\n", bin_fpath);

    file_manipulation(asm_fpath, bin_fpath, &asm_fbuf, &asm_fsize, &fbin);

    printf("\nPART 2, FILE MANIPULATION:\n");
    printf("Assembly file text:\n%s", asm_fbuf);

    lexer(&token_num, &token_array, asm_fsize, asm_fbuf);

    printf("\nPART 3, LEXER:\n");
    printf("Number of tokens: %d\n", token_num);
    printf("Tokens:\n");
    for(int i = 0; i < token_num; i++)
        printf("%.2d: %s \n", i, token_array[i]);

    code_generator(token_num, token_array);

    return 0;
}

//FUNCTION DEFINITIONS

//File extension check, returns 0 if 'fpath' has extension 'ext' (Max 'ext' lenght is 5 characters including '\0')
int ext_check(char fpath[256], const char *ext)
{
    int ext_len = strlen(ext); //
    int fpath_len = strlen(fpath); //

    int check = 1;

    int fpath_dot_i = fpath_len -ext_len; //Index where the last dot in a filepath should be

    for(int i = 0; i < ext_len; i++) //
        check = check && (fpath[fpath_dot_i +i] == ext[i]);

    return !check;
}

//MAIN COMPILER FUNCTIONS

/* USER INPUT
Reads user input for selecting input and ouput files
*/
void user_input(char (*p_asm_fpath)[256], char (*p_bin_fpath)[256])
{
    //Reading user input

    //Short program's notice
    printf("Osnova assembly C compiler version beta\n");
    printf("Created by Ivan Jonjic (IJPantic on github), GPL-V3\n \n");

    //Getting assembly file path
    printf("Assembly ('.asm') input file (path): ");
    scanf("%255s", *p_asm_fpath);

    if(ext_check(*p_asm_fpath, ".asm")) //Wrong input file extension check
    {
        printf("\nERROR: Input file does not have '.asm' extension!\n");
        exit(1);
    }

    //Getting binary file path
    printf("Binary ('.bin') output file (path): ");
    scanf("%255s", *p_bin_fpath);

    if(ext_check(*p_bin_fpath, ".bin")) //Wrong output extension check
    {
        printf("\nERROR: Output file does not have '.bin' extension!\n");
        exit(1);
    }

    return;
}

/* FILE MANIPULATION
Checks output filepath,
opens selected files,
creates text buffer representation of the input file
*/
void file_manipulation(char asm_fpath[256], char bin_fpath[256], char** p_asm_fbuf, int* p_asm_fsize, FILE** p_fbin)
{

    //Creating text buffer from the input file
    FILE* asm_f = fopen(asm_fpath, "r"); //Assembly input file
    if(asm_f == NULL) //Failed to read input file check
    {
        printf("\nERROR: This assembly file does not exist!\n");
        exit(1);
    }

    //Getting binary output file ready for writing
    *p_fbin = fopen(bin_fpath, "wb"); //Clear output file before writing
    fwrite("", 0, 0, *p_fbin);
    *p_fbin = fopen(bin_fpath, "ab"); //Setting append mode (Compiled binary code won't be written to the output at once)

    //Reading input file to a buffer
    fseek(asm_f, 0, SEEK_END); //Setting file's pointer to the end of a file
    *p_asm_fsize = ftell(asm_f) +1; //Reading file size (+2 because index starts at 0)
    fseek(asm_f, 0, SEEK_SET); //Setting file's pointer to the beggining of a file

    *p_asm_fbuf = (char*)malloc(*p_asm_fsize); //Creating assembly file buffer

    fread(*p_asm_fbuf, 1, *p_asm_fsize, asm_f); //Writing from a file to the buffer
    fclose(asm_f);

    return;
}

/* LEXER
Extracts tokens from text buffer,
filters empty tokens out
*/
void lexer(int* p_token_num, char ***p_token_array, int asm_fsize, char* asm_fbuf)
{
    char current_char;
    char token_build[256];

    int i;

    //FIRST RUN (Calculates the number of tokens)

    int first_p = 0;
    int last_p = 0;

    for(last_p; last_p < asm_fsize; last_p++)
    {
        current_char = asm_fbuf[last_p];

        i = first_p;

        if(last_p-first_p > 32) //Max token size check
        {
            printf("\nERROR: Words excede max token limit (31)!\n");
            exit(1);
        }

        if(current_char == ' ' || current_char == '\n')
        {
            (*p_token_num)++;

            first_p = last_p++;
        }
    }

    *p_token_array = malloc(*p_token_num * sizeof(char*)); //Token pointer array (points to all tokens in the aassembly code, max token size is 31 characters)

    //SECOND RUN (Extracts tokens from a text buffer)

    first_p = 0;
    last_p = 0;

    int token_i = 0;

    for(last_p; last_p < asm_fsize; last_p++)
    {
        current_char = asm_fbuf[last_p];

        i = first_p;

        if(current_char == ' ' || current_char == '\n')
        {
            for(i; i < last_p; i++) token_build[i -first_p] = asm_fbuf[i]; //slozi ric
            token_build[i -first_p] = '\0';

            (*p_token_array)[token_i] = malloc(last_p-first_p);

            strcpy((*p_token_array)[token_i], token_build); //prenesi ric
            token_i++;

            first_p = last_p +=1;
        }
    }

    return;
}

/* CODE GENERATOR

*/

void code_generator(int token_num, char** p_token_array)
{
    int token_i = 0;

    int opc_token_i;
    int src_token_i;
    int op_token_i;

    for(token_i; token_i < token_num; token_i +=3)
    {
        opc_token_i = token_i;
        src_token_i = token_i +=1;
        op_token_i = token_i +=2;


        if(!strcmp("pci", p_token_array[opc_token_i]))
        {
            printf("00000");

        }else if(!strcmp("api", p_token_array[opc_token_i]))
        {
            printf("00101");

        }else if(!strcmp("pc", p_token_array[opc_token_i]))
        {
            printf("00110");

        }else if(!strcmp("ap", p_token_array[opc_token_i]))
        {
            printf("00111");

        }else if(!strcmp("a", p_token_array[opc_token_i]))
        {
            printf("00100");

        }else if(!strcmp("r0", p_token_array[opc_token_i]))
        {
            printf("01000");

        }else if(!strcmp("r1", p_token_array[opc_token_i]))
        {
            printf("01001");

        }else if(!strcmp("r2", p_token_array[opc_token_i]))
        {
            printf("01010");

        }else if(!strcmp("r3", p_token_array[opc_token_i]))
        {
            printf("01011");

        }else if(!strcmp("spci", p_token_array[opc_token_i]))
        {
            printf("01100");

        }else if(!strcmp("sapi", p_token_array[opc_token_i]))
        {
            printf("01101");

        }else if(!strcmp("spc", p_token_array[opc_token_i]))
        {
            printf("01110");

        }else if(!strcmp("sap", p_token_array[opc_token_i]))
        {
            printf("01111");

        }else if(!strcmp("jcneg", p_token_array[opc_token_i]))
        {
            printf("10000");

        }else if(!strcmp("jcco", p_token_array[opc_token_i]))
        {
            printf("10001");

        }else if(!strcmp("jcequ", p_token_array[opc_token_i]))
        {
            printf("10010");

        }else if(!strcmp("jcintm", p_token_array[opc_token_i]))
        {
            printf("10011");

        }else if(!strcmp("dvrl", p_token_array[opc_token_i]))
        {
            printf("10101");

        }else if(!strcmp("dvrh", p_token_array[opc_token_i]))
        {
            printf("10110");

        }else if(!strcmp("dvr", p_token_array[opc_token_i]))
        {
            printf("10111");

        }else if(!strcmp("adrd", p_token_array[opc_token_i]))
        {
            printf("11000");

        }else if(!strcmp("intset", p_token_array[opc_token_i]))
        {
            printf("11110");

        }else if(!strcmp("intrst", p_token_array[opc_token_i]))
        {
            printf("11101");
        }


        if(!strcmp("pci", p_token_array[src_token_i]))
        {
            printf("00000");

        }else if(!strcmp("api", p_token_array[src_token_i]))
        {
            printf("00101");

        }else if(!strcmp("pc", p_token_array[src_token_i]))
        {
            printf("00110");

        }else if(!strcmp("ap", p_token_array[src_token_i]))
        {
            printf("00111");

        }else if(!strcmp("r0", p_token_array[src_token_i]))
        {
            printf("01000");

        }else if(!strcmp("r1", p_token_array[src_token_i]))
        {
            printf("01001");

        }else if(!strcmp("r2", p_token_array[src_token_i]))
        {
            printf("01010");

        }else if(!strcmp("r3", p_token_array[src_token_i]))
        {
            printf("01011");

        }else if(!strcmp("spci", p_token_array[src_token_i]))
        {
            printf("01100");

        }else if(!strcmp("sapi", p_token_array[src_token_i]))
        {
            printf("01101");

        }else if(!strcmp("spc", p_token_array[src_token_i]))
        {
            printf("01110");

        }else if(!strcmp("sap", p_token_array[src_token_i]))
        {
            printf("01111");

        }else if(!strcmp("dvr", p_token_array[src_token_i]))
        {
            printf("10100");

        }else if(!strcmp("adrd", p_token_array[src_token_i]))
        {
            printf("11000");

        }else if(!strcmp("intb", p_token_array[src_token_i]))
        {
            printf("11100");
        }


        if(!strcmp("a+cin", p_token_array[opc_token_i]))
        {
            printf("");

        }else if(!strcmp("a||b+cin", p_token_array[opc_token_i]))
        {
            printf("00101");

        }else if(!strcmp("pc", p_token_array[opc_token_i]))
        {
            printf("00110");

        }else if(!strcmp("ap", p_token_array[opc_token_i]))
        {
            printf("00111");

        }else if(!strcmp("a", p_token_array[opc_token_i]))
        {
            printf("00100");

        }else if(!strcmp("r0", p_token_array[opc_token_i]))
        {
            printf("01000");

        }else if(!strcmp("r1", p_token_array[opc_token_i]))
        {
            printf("01001");

        }else if(!strcmp("r2", p_token_array[opc_token_i]))
        {
            printf("01010");

        }else if(!strcmp("r3", p_token_array[opc_token_i]))
        {
            printf("01011");

        }else if(!strcmp("spci", p_token_array[opc_token_i]))
        {
            printf("01100");

        }else if(!strcmp("sapi", p_token_array[opc_token_i]))
        {
            printf("01101");

        }else if(!strcmp("spc", p_token_array[opc_token_i]))
        {
            printf("01110");

        }else if(!strcmp("sap", p_token_array[opc_token_i]))
        {
            printf("01111");

        }else if(!strcmp("jcneg", p_token_array[opc_token_i]))
        {
            printf("10000");

        }else if(!strcmp("jcco", p_token_array[opc_token_i]))
        {
            printf("10001");

        }else if(!strcmp("jcequ", p_token_array[opc_token_i]))
        {
            printf("10010");

        }else if(!strcmp("jcintm", p_token_array[opc_token_i]))
        {
            printf("10011");

        }else if(!strcmp("dvrl", p_token_array[opc_token_i]))
        {
            printf("10101");

        }else if(!strcmp("dvrh", p_token_array[opc_token_i]))
        {
            printf("10110");

        }else if(!strcmp("dvr", p_token_array[opc_token_i]))
        {
            printf("10111");

        }else if(!strcmp("adrd", p_token_array[opc_token_i]))
        {
            printf("11000");

        }else if(!strcmp("intset", p_token_array[opc_token_i]))
        {
            printf("11110");

        }else if(!strcmp("intrst", p_token_array[opc_token_i]))
        {
            printf("11101");
        }
    }

    return;
}

