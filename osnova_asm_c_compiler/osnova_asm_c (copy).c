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
*/

/*
TODO
nisi pokrio inlucde??? include rii odma naa staartu, zaa svaaki inlude pokreni lexer funkciju
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

//Delimiter characters check
int delimiter_check(const char check_char);

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

/* PARSER
Replaces declarations and macros with their contents,
filteres comments out,
creates a label and a variable list,
calculates aarithmetic aand logic operations,
converts everything to a hex value
*/
//void parser(char (**inst_p_array)[16], char (*token_p_array)[32], int *token_num);

/* CODE GENERATOR
TODO
*/
void code_generator();

//MAIN

char asm_fpath[256]; //Assembly file path
char* asm_fbuf = NULL;
int asm_fsize;

char bin_fpath[256]; //Binary file path
FILE* fbin = NULL;

char **token_array = NULL;
int token_num = 0;

char **inst_p_array = NULL;
int inst_num = 0;

int main()
{

    user_input(&asm_fpath, &bin_fpath);

    printf("\nPART 1, USER INPUT:\n");
    printf("Assembly input file path: %s\n", asm_fpath);
    printf("Binary output file path: %s\n", bin_fpath);

    file_manipulation(asm_fpath, bin_fpath, &asm_fbuf, &asm_fsize, &fbin);

    printf("\nPART 2, FILE MANIPULATION:\n");
    printf("Assembly file text:\n%s\n", asm_fbuf);

    lexer(&token_num, &token_array, asm_fsize, asm_fbuf);

    printf("\nPART 3, LEXER:\n");
    printf("Number of tokens: %d\n", token_num);
    printf("Tokens:\n");
    for(int i = 0; i < token_num; i++)
        printf("%.2d: %s \n", i, token_array[i]);

    exit(0);

    //parser(&inst_p_array, token_p_array, &token_num);

    //code_generator();

    // --> TEST <--
/*
    for(int j = 0; j < token_num; j++)
    {
        printf("%d: %s\n", j, token_p_array[j]);
    }
*/
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

//Delimiter character check
int delimiter_check(const char check_char)
{
    switch(check_char)
    {
        case '%':  return 1; break; //Compiler directive
        case '#':  return 1; break; //Binary word join operator
        case '\"': return 1; break; //String bracket
        case '\'': return 1; break; //Character bracket
        case ';':  return 1; break; //Comment
        case ',':  return 1; break; //Argument/parameter seperator
        case '.':  return 1; break; //Line label
        case ' ':  return 1; break; //word separator (space)
        case '\t': return 1; break; //word separator (tab)
        case '\n': return 1; break; //Instruction end
        case '+':  return 1; break; //Addition operator
        case '-':  return 1; break; //Subtraction operator
        case '*':  return 1; break; //multiplication operator
        case '/':  return 1; break; //Integer division operator
        case '&':  return 1; break; //Logic and operator
        case '|':  return 1; break; //Logic or operator
        case '!':  return 1; break; //Logic invertor operator
        case '(':  return 1; break; //Open bracket for operation priority
        case ')':  return 1; break; //Closed bracket for operation priority
        case '[':  return 1; break; //Open bracket for part selection
        case ']':  return 1; break; //Closed bracket for part selection
        case ':':  return 1; break; //TODO nema svrhe, cisto da korisnik zna staa je isto kao i _ za start: funkciju
        case '$':  return 1; break; //TODO
        case '^':  return 1; break; //TODO

        default: return 0;
    }
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
    int first_p;
    int last_p;

    char current_char;
    char token_build[256];

    int i;

    //FIRST RUN (Calculates the number of tokens)

    first_p = 0;
    last_p = 0;

    for(last_p; last_p < asm_fsize; last_p++)
    {
        current_char = asm_fbuf[last_p];

        i = first_p;

        if(last_p-first_p > 32) //Max token size check
        {
            printf("\nERROR: Words excede max token limit (31)!\n");
            exit(1);
        }

        if(delimiter_check(current_char)) //Delimiter is a sign that word is done
        {
            if(current_char == '\n') //Include delimiter also
            {
                if(first_p != last_p) *p_token_num +=2;

            }else
            {
                if(current_char == ' ' || current_char == '\t')
                {
                    if(first_p != last_p) *p_token_num +=1;

                }else
                {
                    if(first_p == last_p)
                        *p_token_num +=1;
                    else
                       *p_token_num +=2;
                }
            }

            first_p = last_p +1;
        }
    }

    *p_token_array = malloc(*p_token_num * sizeof(char*)); //Token pointer array (points to all tokens in the aassembly code, max token size is 31 characters)

    //SECOND RUN (Extracts tokens from a text buffer)

    first_p = 0;
    last_p = 0;

    int token_i = 0;

    void add_built_token()
    {
        for(i; i < last_p; i++) token_build[i -first_p] = asm_fbuf[i]; //slozi ric
        token_build[i -first_p] = '\0';

        (*p_token_array)[token_i] = malloc(last_p-first_p +1);

        strcpy((*p_token_array)[token_i], token_build); //prenesi ric
        token_i++;
    }

    void add_built_delimiter()
    {
        token_build[0] = current_char; //slozi ric (delimiter)
        token_build[1] = '\0';

        (*p_token_array)[token_i] = malloc(2);

        strcpy((*p_token_array)[token_i], token_build); //prenesi ric
        token_i++;
    }

    void add_newline_char()
    {
        strcpy(token_build, "NEWLINECHAR"); //slozi ric (new line character)

        (*p_token_array)[token_i] = malloc(12);

        strcpy((*p_token_array)[token_i], token_build); //prenesi ric
        token_i++;
    }

    for(last_p; last_p < asm_fsize; last_p++)
    {
        current_char = asm_fbuf[last_p];

        i = first_p;

        if(delimiter_check(current_char)) //Delimiter is a sign that word is done
        {
            if(current_char == '\n') //Include delimiter also
            {
                if(first_p != last_p)
                {
                    add_built_token();
                    add_newline_char();
                }

            }else
            {
                if(current_char == ' ' || current_char == '\t')
                {
                    if(first_p != last_p) add_built_token();

                }else
                {
                    add_built_delimiter();
                    if(first_p != last_p) add_built_token();
                }
            }

            first_p = last_p +1;
        }
    }

    return;
}

/* PARSER TODO Prvi run podjeli na instrukcijske a tek unda naaa 2 runu sredi te instrukcije
Replaces declarations and macros with their contents,
filteres comments out,
creates a label and a variable list,
calculates aarithmetic aand logic operations,
converts everything to a hex value
*/

void parser(char ***inst_p_array, int *inst_num, char ***token_p_array, int *token_num)
{
    

    /*
    int cmplr_directive_m = 0;
    int comment_m = 0;


    for(token_i; token_i < *token_num; token_i++)
    {
        token = token_p_array[token_i];

        //if(strcmp(token, "%"))
        
    }
    */

    return;
}

/* CODE GENERATOR
TODO
*/

/*
void code_generator()
{


    return;
}
*/
