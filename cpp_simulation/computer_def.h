//Osnova test computer c++ file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
5. October, 2026.
*/

#include "LIB_osnova_cpu.h"
#include "LIB_mem.h"

#ifndef computer_def
#define computer_def

//Update hardware
void update();

//Hardware wiring
void wire();

//Don't touch anything above!

//HARDWARE INIT
int constexpr mems_num = 1; //One CPU "main_cpu"
int constexpr cpus_num = 1; //One RAM "main_ram"

//External references, Don't touch this section!
extern int mems_size[mems_num];
extern byte_t *mems[mems_num];
extern CPU *cpus[cpus_num];

//WIRING
extern byte_t data_bus; //Data bus (floats at the start)
extern pnt_t adr_bus; //Address bus (floats at the start)

extern bool we; //CPU's write enable signal (floats at the start)
extern bool re; //CPU's read enable signal (floats at the start)

extern bool int_add; //Interrupt ADD line
extern bool int_cpu; //Interrupt CPU line (floats at the start)
extern bool flgx; //CPU's flag x pin
extern bool ce; //RAM's chip enable signal

#endif

