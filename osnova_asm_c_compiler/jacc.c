//Jednostava assembly C compiler, version beta, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
7. November, 2026.
8. November, 2026.
*/

/*
TODO

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//MAIN FUNCTIONS DECLARATION

//Checks correctnes of the user input
void user_input(int argc, char** argv, char** p_assembly_filepath, char** p_output_filepath);

//Creates text buffer from the assembly input file
void input_file_manipulation(char* p_assembly_filepath, char** p_assembly_text);

//Split assembly text into tokens
void prelexer(char* assembly_text, char*** p_token_list, int* p_token_num);

//Groups tokens into instructions, ignores comment tokens
void lexer(char** token_list, int* p_token_num, char**** p_instruction_list, int* p_instruction_num, int** p_instruction_len_list);

//Removes empty instructions, inserts macros and includes
void preparser(char**** p_instruction_list, int instruction_num, int** p_instruction_len_list);

void base_parser(char**** p_instruction_list, int instruction_num, int** p_instruction_len_list);

void directive_parser();

void variable_parser();

void operation_parser();

void optimiser();

void generator();

void output_file_manipulation();

//MAIN VARIABLES

char* assembly_filepath;
char* output_filepath;
char* flag;

char* assembly_text;

char** token_list;
int token_num;

char*** instruction_list;
int instruction_num;
int* instruction_len_list;

//MAIN

int main(int argc, char* argv[])
{
    user_input(argc, argv, &assembly_filepath, &output_filepath);

    input_file_manipulation(assembly_filepath, &assembly_text);

    prelexer(assembly_text, &token_list, &token_num);

    lexer(token_list, &token_num, &instruction_list, &instruction_num, &instruction_len_list);

    preparser(&instruction_list, instruction_num, &instruction_len_list);

    base_parser(&instruction_list, instruction_num, &instruction_len_list);


    /*
    for(int i=0; i<instruction_num; i++)
    {
        printf("instruction %d: ", i);
        for(int j=0; j<instruction_len_list[i]; j++)
            printf("'%s' ", instruction_list[i][j]);
        printf("\n");
    }
    */
}

//MAIN FUNCTIONS DECLARATION

void user_input(int argc, char** argv, char** p_assembly_filepath, char** p_output_filepath)
{
    //Number of args check
    int arg_num = argc-1; //-1 because first token is just a command (jacc)

    if(arg_num>3) //Too many arguments
    {
        printf("ERROR: There are too many arguments! (%d)\n\n", arg_num);
        exit(1);

    }else if(!arg_num) //No arguments
    {
        printf("ERROR: There are too few arguments! (%d)\n\n", arg_num);
        exit(1);

    }else if(argc==1) //Only one argument
    {
        //Acceptable only if the argument is 'help'
        if(!strcmp(argv[1], "help")) //Prints how to use jacc
        {
            printf("jacc [Assembly_input_filepath] [Output_filepath] [Flag (Optional)]\n\n");
            exit(0);

        }else
        {
            printf("ERROR: There are too few arguments! (%d)\n\n", arg_num);
            exit(1);
        }
    }

    //Jacc flag check
    if(argc==4)
    {
        char* jacc_flag = argv[3]; //There is a flag

    }else
    {
        char* jacc_flag = NULL; //There isn't a flag
    }

    //Assembly filepath check
    char* input_filepath = argv[1];
    int input_filepath_len = strlen(input_filepath);

    if(input_filepath_len<5) //.asm can't even fit in here then
    {
        printf("ERROR: Assembly input filepath is too short! (%d)\n\n", input_filepath_len);
        exit(1);
    }

    //Checking assembly input file's extension
    char input_filepath_extension[5];
    memcpy(input_filepath_extension, input_filepath + input_filepath_len-4, 5); //Extracting extesnion

    if(strcmp(input_filepath_extension, ".asm")) //Comparing extension
    {
        printf("ERROR: Assembly input file's extesnsion is wrong! (%s)\n\n", input_filepath_extension);
        exit(1);
    }

    *p_assembly_filepath = input_filepath; //Passing filepath

    return;
}

void input_file_manipulation(char* assembly_filepath, char** p_assembly_text)
{
    //Openning file
    FILE* assembly_file = fopen(assembly_filepath, "r");

    //Checking if the file exists
    if(assembly_file==NULL)
    {
        printf("ERROR: Assembly input file does not exist! (%s)\n\n", assembly_filepath);
        exit(1);
    }

    //Reading input file
    fseek(assembly_file, 0, SEEK_END); //Setting file's pointer to the end of a file
    int assembly_text_len = ftell(assembly_file); //Extracting file size

    //Checking if the input file is empty
    if(assembly_text_len==0)
    {
        printf("ERROR: Assembly input file is empty! (%s)\n\n", assembly_filepath);
        exit(1);
    }

    //Creating assembly file buffer
    *p_assembly_text = (char*)malloc(assembly_text_len+1); //+1 for null terminator

    fseek(assembly_file, 0, SEEK_SET); //Setting file's pointer to the staart of a file
    fread(*p_assembly_text, 1, assembly_text_len, assembly_file); //Writing from a file to the text buffer
    (*p_assembly_text)[assembly_text_len] = '\0'; //Adding null terminator

    fclose(assembly_file); //Closing file

    return;
}

void prelexer(char* assembly_text, char*** p_token_list, int* p_token_num)
{
    int text_len = strlen(assembly_text);

    char tmp_token[256];

    int token_start_pnt;
    int token_end_pnt;
    int token_len;

    int token_i;
    *p_token_num = 0;

    char current_char;
    int delimiter;
    char last_delimiter;

    int round = 0;
    for(round; round<2; round++)
    {
        if(round) *p_token_list = malloc(sizeof(char*) * (*p_token_num));

        //Extracting tokens
        token_i = 0;
        token_start_pnt = 0;
        token_end_pnt = 0;

        for(int char_i=0; char_i<text_len; char_i++)
        {
            current_char = assembly_text[char_i];

            switch(current_char)
            {
                case '+': delimiter = 1; break;
                case '-': delimiter = 1; break;
                case '*': delimiter = 1; break;
                case '/': delimiter = 1; break;
                case '!': delimiter = 1; break;
                case '&': delimiter = 1; break;
                case '|': delimiter = 1; break;
                case '^': delimiter = 1; break;
                case '(': delimiter = 1; break;
                case ')': delimiter = 1; break;
                case '[': delimiter = 1; break;
                case ']': delimiter = 1; break;
                case '.': delimiter = 1; break;
                case ',': delimiter = 1; break;
                case ':': delimiter = 1; break;
                case ';': delimiter = 1; break;
                case ' ': delimiter = 1; break;
                case '\t': delimiter = 1; break;
                case '\n': delimiter = 1; break;
                case '$': delimiter = 1; break;
                case '#': delimiter = 1; break;
                case '%': delimiter = 1; break;

                default: delimiter = 0;
            }

            //Count tokens
            if(delimiter)
            {
                token_len = token_end_pnt - token_start_pnt;

                if(token_len) //word token
                {
                    if(round)
                    {
                        memcpy(tmp_token, assembly_text + token_start_pnt, token_len); //Extracting token
                        tmp_token[token_len] = '\0';

                        (*p_token_list)[token_i] = (char*)malloc(token_len);
                        strcpy((*p_token_list)[token_i], tmp_token);

                    }else
                    {
                        (*p_token_num)++;
                    }

                    token_i++;
                    last_delimiter = 'x'; //Ponovo moze doci bilo koji delimiter
                }

                if(current_char!=last_delimiter) //delimiter token
                {
                    if(round)
                    {
                        tmp_token[0] = current_char;
                        tmp_token[1] = '\0';

                        (*p_token_list)[token_i] = (char*)malloc(token_len);
                        strcpy((*p_token_list)[token_i], tmp_token);

                    }else
                    {
                        (*p_token_num)++;
                    }

                    token_i++;
                    last_delimiter = current_char; //nemnoj ponavljat isti delimiter
                }

                token_start_pnt = token_end_pnt+1;
            }

            token_end_pnt++;
        }
    }
}

void lexer(char** token_list, int* p_token_num, char**** p_instruction_list, int* p_instruction_num, int** p_instruction_len_list)
{
    int instruction_start_pnt;
    int instruction_end_pnt;
    int instruction_len;

    int instruction_i;
    *p_instruction_num = 0;

    int token_i;

    char* current_token;

    int round = 0;
    int comment_mode = 0;

    for(round; round<2; round++)
    {
        if(round)
        {
            *p_instruction_list = malloc(sizeof(char*) * (*p_instruction_num));
            *p_instruction_len_list = malloc(sizeof(int) * (*p_instruction_num));
        }

        //Extracting tokens
        token_i = 0;

        instruction_start_pnt = 0;
        instruction_end_pnt = 0;
        instruction_i = 0;

        for(int token_i=0; token_i<(*p_token_num); token_i++)
        {
            current_token = token_list[token_i];

            //Count tokens
            if(!strcmp(current_token, "\n") || !strcmp(current_token, ";"))
            {
                instruction_len = instruction_end_pnt - instruction_start_pnt;

                if(instruction_len) //word token
                {
                    if(round)
                    {
                        (*p_instruction_list)[instruction_i] = (char**)malloc(sizeof(char**) * instruction_len);
                        (*p_instruction_len_list)[instruction_i] = instruction_len;

                        if(!comment_mode)
                        {
                            for(int i=0; i<instruction_len; i++)
                                (*p_instruction_list)[instruction_i][i] = token_list[instruction_start_pnt + i];
                        }

                    }else
                    {
                        (*p_instruction_num)++;
                    }

                    instruction_i++;
                }

                instruction_start_pnt = instruction_end_pnt+1;
            }

            instruction_end_pnt++;

            if(!strcmp(current_token, "\n")) comment_mode = 0;
            if(!strcmp(current_token, ";")) comment_mode = 1;
        }
    }
}

void preparser(char**** p_instruction_list, int instruction_num, int** p_instruction_len_list) //TODO prerskoci za sada
{
//naidjes naa include ->umetnes njegove lexer tokene ispred glavnog filea
//naijdes na macrodef ->stvoris listu s njegovim sadrzjima
//naaidjes na macro ->umetnes iz liste njegove sadrzaaje
//naidjes na prazne instrukcije, maknes ih (null)
    //instruction_list

    //int instruction_i = 0;
    //for(instruction_i; instruction_i<instruction_num; instruction_i++)
    //{

    //}
}

/*
1.common bases (base)
0b, 0x -> [DECIMAL]

2.secondary compiler directives (directives)
stack
heap
switch define with its values

3.switch pointers with variables (variable)

4.solve by order (operation)
()
[]
!
| & ^
* /
+ -
[SPACE]
*/

void base_parser(char**** p_instruction_list, int instruction_num, int** p_instruction_len_list)
{
    int instruction_i;
    int instruction_len;
    int token_i;
    char* token;

    for(instruction_i=0; instruction_i<instruction_num; instruction_i++)
    {
        instruction_len = (*p_instruction_len_list)[instruction_i];

        for(token_i=0; token_i<instruction_len; token_i++)
        {
            token = (*p_instruction_list)[instruction_i][token_i];
            printf("%s\n", token);
        }
    }
}

void directive_parser()
{

}

void variable_parser()
{

}

void operation_parser()
{

}

void optimiser() //TODO prerskoci za sada
{

}

void generator()
{

}

void output_file_manipulation()
{

}

