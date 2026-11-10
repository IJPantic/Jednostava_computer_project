//Jednostava assembly C compiler, version beta, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
7. November, 2026.
8. November, 2026.
10. November, 2026.
*/

/*
TODO's

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//GLOBAL VARIABLES
char* input_file_path;
char* output_file_path;
char* input_flag;

char* input_text_buffer;

char** token_list;
int token_num;

char*** instruction_list;
int* instruction_len_list;
int instruction_num;

//UTILITY FUNCTIONS DECLARATION
void insert_instruction(int start_index, char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num);
void delete_instruction(int start_index, char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num);

void insert_token(int start_index, char**** p_instruction_list, int** p_instruction_len_list);
void delete_token(int start_index, char**** p_instruction_list, int** p_instruction_len_list);

//COMPILING FUNCTIONS DECLARATION
void user_input(int argc, char** argv, char** p_input_file_path, char** p_output_file_path, char** p_input_flag);

void input_file_manipulation(char* input_file_path, char** p_input_text_buffer);

void tokenizer(char* input_text_buffer, char*** p_token_list, int* p_token_num);

void lexer(char** token_list, int token_num, char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num);

void parser(char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num);

void generator(char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num);

void output_file_manipulation(char* output_file_path, char* input_flag);

//PARSER FUNCTIONS DECLARATION
void comment_parser();

void stack_parser();

void heap_parser();

void include_parser();

void macrodef_parser();

void macro_parser();

void define_parser();

void label_parser();

void variable_parser();

void base_parser();

void operation_parser();

//MAIN
int main(int argc, char** argv)
{
    user_input(argc, argv, &input_file_path, &output_file_path, &input_flag);

    input_file_manipulation(input_file_path, &input_text_buffer);

    tokenizer(input_text_buffer, &token_list, &token_num);

    lexer(token_list, token_num, &instruction_list, &instruction_len_list, &instruction_num);

    //parser(&instruction_list, &instruction_len_list, &instruction_num);

    //generator(&instruction_list, &instruction_len_list, &instruction_num);

    //output_file_manipulation(output_file_path, input_flag);

    printf("TESTS (user_input):\ninput_file_path: %s\noutput_file_path: %s\ninput_flag:\n%s\n\n", input_file_path, output_file_path, input_flag);

    printf("TESTS (input_file_manipulation):\ninput_text_buffer: %s\n\n", input_text_buffer);

    printf("TESTS (tokenizer):\ntoken_num: %d\n", token_num);
    for(int i=0; i<token_num; i++) printf("token_list (index %d): %s\n", i, token_list[i]);
    printf("\n");

    printf("TESTS 1 (lexer):\ninstruction_num: %d\n", instruction_num);
    for(int i=0; i<instruction_num; i++)
    {
        printf("instruction_list (index %d): ", i);

        for(int j=0; j<instruction_len_list[i]; j++) printf("(%d):%s ", j, instruction_list[i][j]);
        printf("\n");
    }
    printf("\n\n");

    insert_instruction(7, &instruction_list, &instruction_len_list, &instruction_num);

    printf("TESTS 2 (lexer):\ninstruction_num: %d\n", instruction_num);
    for(int i=0; i<instruction_num; i++)
    {
        printf("instruction_list (index %d): ", i);

        for(int j=0; j<instruction_len_list[i]; j++) printf("(%d):%s ", j, instruction_list[i][j]);
        printf("\n");
    }
    printf("\n\n");

    return 0;
}

//UTILITY FUNCTIONS DEFINITIONS
void insert_instruction(int start_index, char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num)
{
    //Creating temporay memory block
    char*** tmp_instruction_list = malloc(sizeof(char**) * ((*p_instruction_num)+1));
    int* tmp_instruction_len_list = malloc(sizeof(int) * ((*p_instruction_num)+1));

    //Copying from current to temporay
    int instruction_i = 0;
    int tmp_instruction_i = 0;
    
    for(instruction_i; instruction_i<(*p_instruction_num); instruction_i++)
    {
        if(instruction_i==start_index) tmp_instruction_i++; //shift everthing by 1

        tmp_instruction_list[tmp_instruction_i] = (*p_instruction_list)[instruction_i];
        tmp_instruction_len_list[tmp_instruction_i] = (*p_instruction_len_list)[instruction_i];

        tmp_instruction_i++;
    }

    //Cleaning up old memory block
    free(*p_instruction_list);
    free(*p_instruction_len_list);

    (*p_instruction_num)++;

    //Creating new memory block
    (*p_instruction_list) =  malloc(sizeof(char**) * (*p_instruction_num));
    memcpy((*p_instruction_list), tmp_instruction_list, sizeof(char**) * (*p_instruction_num));

    (*p_instruction_len_list) = malloc(sizeof(int) * (*p_instruction_num));
    memcpy((*p_instruction_len_list), tmp_instruction_len_list, sizeof(int) * (*p_instruction_num));
}

void delete_instruction(int start_index, char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num)
{
    //Creating temporay memory block
    char*** tmp_instruction_list = malloc(sizeof(char**) * ((*p_instruction_num)-1));
    int* tmp_instruction_len_list = malloc(sizeof(int) * ((*p_instruction_num)-1));

    //Copying from current to temporay
    int instruction_i = 0;
    int tmp_instruction_i = 0;
    
    for(instruction_i; instruction_i<(*p_instruction_num); instruction_i++)
    {
        if(instruction_i!=start_index)
        {
            tmp_instruction_list[tmp_instruction_i] = (*p_instruction_list)[instruction_i];
            tmp_instruction_len_list[tmp_instruction_i] = (*p_instruction_len_list)[instruction_i];

            tmp_instruction_i++;

        }else //free unused tokens
        {
            for(int i=0; i<(*p_instruction_len_list)[instruction_i]; i++) free((*p_instruction_list)[instruction_i][i]);
        }
    }

    //Cleaning up old memory block
    free(*p_instruction_list);
    free(*p_instruction_len_list);

    (*p_instruction_num)--;

    //Creating new memory block
    (*p_instruction_list) =  malloc(sizeof(char**) * (*p_instruction_num));
    memcpy((*p_instruction_list), tmp_instruction_list, sizeof(char**) * (*p_instruction_num));

    (*p_instruction_len_list) = malloc(sizeof(int) * (*p_instruction_num));
    memcpy((*p_instruction_len_list), tmp_instruction_len_list, sizeof(int) * (*p_instruction_num));
}

void insert_token(int start_index, char**** p_instruction_list, int** p_instruction_len_list)
{

}

void delete_token(int start_index, char**** p_instruction_list, int** p_instruction_len_list)
{

}

//COMPILING FUNCTIONS DEFINITIONS
void user_input(int argc, char** argv, char** p_input_file_path, char** p_output_file_path, char** p_input_flag)
{
    int arg_num = --argc; //Number of arguments, -1 because first token is just a command (jacc)

    //Number of args check
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

        }else //Too many arguments
        {
            printf("ERROR: There are too few arguments! (%d)\n\n", arg_num);
            exit(1);
        }
    }

    //Jacc flag check
    if(arg_num==3)
    {
        *p_input_flag = argv[3]; //There is a flag

    }else
    {
        *p_input_flag = NULL; //There isn't a flag
    }

    //Assembly file path check
    char* input_file_path = argv[1];
    int input_file_path_len = strlen(input_file_path);

    if(input_file_path_len<5) //.asm can't even fit in this string then
    {
        printf("ERROR: Assembly input filepath is too short! (%d)\n\n", input_file_path_len);
        exit(1);
    }

    //Checking assembly input file's extension
    char input_file_path_extension[5];
    memcpy(input_file_path_extension, input_file_path + input_file_path_len-4, 5); //Extracting extesnion

    if(strcmp(input_file_path_extension, ".asm")) //Comparing extension
    {
        printf("ERROR: Assembly input file's extesnsion is wrong! (%s)\n\n", input_file_path_extension);
        exit(1);
    }

    *p_input_file_path = input_file_path; //Passing input file path
    *p_output_file_path = argv[2]; //Passing output file path

    return;
}

void input_file_manipulation(char* input_file_path, char** p_input_text_buffer)
{
    //Opening file
    FILE* input_file = fopen(input_file_path, "r");

    //Checking if the file exists
    if(input_file==NULL)
    {
        printf("ERROR: Assembly input file does not exist! (%s)\n\n", input_file_path);
        exit(1);
    }

    //Reading input file
    fseek(input_file, 0, SEEK_END); //Setting file's pointer to the end of a file
    int input_text_len = ftell(input_file); //Extracting file size

    //Checking if the input file is empty
    if(input_text_len==0)
    {
        printf("ERROR: Assembly input file is empty! (%s)\n\n", input_file_path);
        exit(1);
    }

    //Creating assembly file buffer
    *p_input_text_buffer = malloc(input_text_len+1); //+1 for null terminator

    fseek(input_file, 0, SEEK_SET); //Setting file's pointer to the start of a file
    fread(*p_input_text_buffer, 1, input_text_len, input_file); //Writing from a file to the text buffer
    (*p_input_text_buffer)[input_text_len] = '\0'; //Adding null terminator

    fclose(input_file); //Closing file

    return;
}

void tokenizer(char* input_text_buffer, char*** p_token_list, int* p_token_num)
{
    int text_len = strlen(input_text_buffer);

    char tmp_token[256];

    int token_start_pnt;
    int token_end_pnt;
    int token_len;

    int token_i;
    *p_token_num = 0;

    char current_char;
    int delimiter;
    char last_delimiter = 'x';

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
            current_char = input_text_buffer[char_i];

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

                if(token_len) //Word Token
                {
                    if(round)
                    {
                        memcpy(tmp_token, input_text_buffer + token_start_pnt, token_len); //Extracting token
                        tmp_token[token_len] = '\0';

                        (*p_token_list)[token_i] = (char*)malloc(token_len); //Adding token
                        strcpy((*p_token_list)[token_i], tmp_token);

                    }else
                    {
                        (*p_token_num)++;
                    }

                    token_i++;
                    last_delimiter = 'x'; //Reset delimiter
                }

                if(current_char!=last_delimiter) //Delimiter token
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
                    last_delimiter = current_char;
                }

                token_start_pnt = token_end_pnt+1;
            }

            token_end_pnt++;
        }
    }
}

void lexer(char** token_list, int token_num, char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num)
{
    int instruction_start_pnt;
    int instruction_end_pnt;
    int instruction_len;

    int instruction_i;
    *p_instruction_num = 0;

    int token_i;

    char* current_token;

    int round = 0;

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

        for(int token_i=0; token_i<token_num; token_i++)
        {
            current_token = token_list[token_i];

            //Count tokens
            if(!strcmp(current_token, "\n") || !strcmp(current_token, ";"))
            {
                instruction_len = instruction_end_pnt - instruction_start_pnt +1;

                //Adding delimiter token

                if(instruction_len)
                {
                    //Adding word token
                    if(round)
                    {
                        (*p_instruction_list)[instruction_i] = (char**)malloc(sizeof(char*) * instruction_len);
                        (*p_instruction_len_list)[instruction_i] = instruction_len;

                        for(int i=0; i<instruction_len; i++)
                            (*p_instruction_list)[instruction_i][i] = token_list[instruction_start_pnt + i];

                    }else
                    {
                        (*p_instruction_num)++; //Count word token
                    }

                    instruction_i++;
                }

                instruction_start_pnt = instruction_end_pnt+1;
            }

            instruction_end_pnt++;
        }
    }
}

void parser(char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num)
{
    comment_parser();

    stack_parser();

    heap_parser();

    include_parser();

    macrodef_parser();

    macro_parser();

    define_parser();

    label_parser();

    variable_parser();

    base_parser();

    operation_parser();

    return;
}

void generator(char**** p_instruction_list, int** p_instruction_len_list, int* p_instruction_num)
{

}

void output_file_manipulation(char* output_file_path, char* input_flag)
{

}

//PARSER FUNCTIONS DEFINITIONS
void comment_parser()
{

}

void stack_parser()
{

}

void heap_parser()
{

}

void include_parser()
{

}

void macrodef_parser()
{

}

void macro_parser()
{

}

void define_parser()
{

}

void label_parser()
{

}

void variable_parser()
{

}

void base_parser()
{

}

void operation_parser()
{

}

