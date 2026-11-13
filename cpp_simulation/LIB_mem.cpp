//Osnova c++ memory simulation library file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
25. september, 2026.
30. september, 2026.
2. October, 2026.
5. October, 2026.
*/

#include "LIB_mem.h" //Header file of this library

//Memory constructor
MEM::MEM(int size, const char type[3])
{
    //SETTING PROPERTIES

    mem_size = size; //Memory size (In bytes)

    //Memory type ('rw' read/write, '-w' write only, 'r-' read only)
    mem_type[0] = type[0]; //Read?
    mem_type[1] = type[1]; //Write?
    mem_type[2] = '\0';

    memory = (byte_t*)malloc(mem_size); //Creating memory

    //INPUTS AND OUTPUTS

    //Buses
    *io_adrb; //Memory storage address bus
    *io_db; //Memory data bus

    //Control lines
    *io_we; //Write enable (Inverted)
    *io_re; //Read enable (Inverted)
    *io_ce; //Chip enable (Inverted)

    //MEMORY ACTIONS

    void update(); //Memory value update

    void end(); //Free allocated memory from this memory emulation
}

//Memory value update
void MEM::update()
{
    if(!*io_ce) //Is chip selected?
    {
        pnt_t valid_adr = trim(*io_adrb, 0, log2(mem_size)); //Containing 20-bit address within actual memory address space

        if(mem_type[0] == 'r' && !*io_re && *io_we) //Reading from memory to data bus
            *io_db = memory[valid_adr];

        else if(mem_type[1] == 'w' && *io_re && !*io_we) //writing from data bus to memory
            memory[valid_adr] = *io_db;
    }
}

//Free allocated memory from this memory emulation
void MEM::end()
{
    free(memory);
}

