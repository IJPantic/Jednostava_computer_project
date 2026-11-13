//Osnova c++ memory simulation library header file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
25. september, 2026.
30. september, 2026.
1. October, 2026.
2. October, 2026.
5. October, 2026.
*/

#include "LIB_osnova_utils.h"

#ifndef LIB_mem
#define LIB_mem

//Defining memory
class MEM
{
    public:
        MEM(int size, const char type[3]); //Memory constructor

        //SETTING PROPERTIES

        int mem_size; //Memory size (In bytes)

        char mem_type[3]; //Memory type ('rw' read/write, '-w' write only, 'r-' read only)

        byte_t *memory; //Memory storage

        //INPUTS AND OUTPUTS

        //Buses
        pnt_t *io_adrb; //Memory storage address bus
        byte_t *io_db; //Memory data bus

        //Control lines
        bool *io_we; //Write enable (Inverted)
        bool *io_re; //Read enable (Inverted)
        bool *io_ce; //Chip enable (Inverted)

        //MEMORY ACTIONS

        void update(); //Memory value update

        void end(); //Free allocated memory from this memory emulation
};

#endif

