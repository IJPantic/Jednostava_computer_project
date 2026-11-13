//Osnova test computer c++ header file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
5. October, 2026.
*/

#include "computer_def.h" //Header file of this library

//Don't touch anything above!

//INITIALIZE HARDWARE
CPU main_cpu;
MEM main_ram(256, "rw");

byte_t data_bus = MAX_BV; //Data bus (floats at the start)
pnt_t adr_bus = MAX_PV; //Address bus (floats at the start)

bool we = 1; //CPU's write enable signal (floats at the start)
bool re = 1; //CPU's read enable signal (floats at the start)

bool int_add = 1; //Interrupt ADD line
bool int_cpu = 1; //Interrupt CPU line (floats at the start)
bool flgx = 1; //CPU's flag x pin
bool ce = 0; //RAM's chip enable signal

//UPDATE HARDWARE
void update()
{
    main_cpu.update();
    main_ram.update();
}

//HARDWARE WIRING
void wire()
{
    //MAIN CPU CONNECTION
    cpus[0] = &main_cpu;

    main_cpu.io_db = &data_bus; //Data bus
    main_cpu.io_adrb = &adr_bus; //Address bus

    main_cpu.io_if = &int_cpu; //Interrupt CPU line
    main_cpu.io_itx = &int_add; //Interrupt ADD line
    main_cpu.io_fx = &flgx; //CPU's flag x pin

    main_cpu.io_addw = &we; //Write enable signal
    main_cpu.io_addr = &re; //Read enable signal

    //MAIN RAM CONNECTION
    mems[0] = main_ram.memory;
    mems_size[0] = 256;

    main_ram.io_db = &data_bus; //Data bus
    main_ram.io_adrb = &adr_bus; //Address bus

    main_ram.io_we = &we; //Write enable signal
    main_ram.io_re = &re; //Read enable signal
    main_ram.io_ce = &ce; //Chip enable signal
}

