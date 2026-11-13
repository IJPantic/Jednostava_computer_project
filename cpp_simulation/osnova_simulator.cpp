//Osnova computer c++ simulator, version 1, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
23. september, 2026.
24. september, 2026.
25. september, 2026.
26. september, 2026.
28. september, 2026.
29. september, 2026.
30. september, 2026.
2. October, 2026.
3. October, 2026.
4. October, 2026.
5. October, 2026.
6. October, 2026.
*/

#include <stdio.h>
#include <cstring>
#include <thread>
#include <chrono>
#include "LIB_osnova_utils.h"

#include "computer_def.h"

/*
HOW TO USE:

1. Create and link "computer_def.h" and "computer_def.cpp" file when compiling with osnova simulator code
2. In "computer_def.cpp" create hardware objects, declare wires/buses together with their starting values and set number of cpus and mems
3. Create "update" void function, write update sequence there
4. Create "wire" void function, declare which wires/buses are connected to each other
*/

int stage = 0; //CPU's exectuion cycle stage

int mems_size[mems_num]; //Array of sizes of available memories
byte_t *mems[mems_num]; //Array of available memories
CPU *cpus[cpus_num]; //Array of available CPUs

//SIMULATION THREAD

std::thread sim_thread; //Simulation thread

bool sim_auto; //Auto-tick simulation control
int freq = 1; //Thread freqency (In Hz, max 1000Hz)

//Every auto clock tick
void update_thread(int freq)
{
    int period;

    if(freq > 1000) //Checks if freq is exceeding it's max value
        period = 1; //Sets to a max possible (To a 1KHz)
    else
        period = 1000 / freq;

    while(sim_auto)
    {
        update();

        stage++;
        if(stage > 7) stage = 0; //Bounds it's value between 0 and 7

        std::this_thread::sleep_for(std::chrono::milliseconds(period)); //Controls simulation's execution rate
    }
}

//SIMULATOR COMMANDS DECLARATION

//Exits program
void cmd_done();

//Does one clock tick
void cmd_step();

//Does clock ticks until it finishes current cycle (Goes to the next multiple of 8)
void cmd_cycle();

//Commands to a computer simulation
void cmd_sim();

//takes decimal value, prints hexadecimal value
void cmd_dec();

//takes hexadecimal value, prints decimal value
void cmd_hex();

//Lists selected tables
void cmd_list();

//Prints all commands and how to use them
void cmd_help();

//Prints all CPU states
void cmd_snap();

//Prints a value at a certain memory address
void cmd_peek();

//Changes a value at a certain memory address
void cmd_poke();

//Prints (Dumps) memory contents to a terminal or a bin file
void cmd_dump();

//Inserts string of values into memory at a certain memory address
void cmd_insert();

//SIMULATOR

char cmd[16]; //Command (Simulator command)

int arg1, arg2, arg3; //Command's number arguments
char str1[16], str2[16]; //Command's word arguments

int main()
{
    wire(); //External user-defined function that sets wiring of the computer

    //Short program's notice
    printf("Osnova c++ simulator version 1, type 'help' to list all commands and their arguments\n");
    printf("Created by Ivan Jonjic (IJPantic on github), GPL-V3\n \n");

    //SIMULATOR CONTROLS

    while(true)
    {
        //Read terminal command
        printf(" > ");
        scanf("%15s", cmd);

        //Command check
        if(!strcmp(cmd, "done")) cmd_done(); //Exits program

        else if(!strcmp(cmd, "step")) cmd_step(); //Does one clock tick

        else if(!strcmp(cmd, "cycle")) cmd_cycle(); //Does clock ticks until it finishes current cycle (Goes to the next multiple of 8)

        else if(!strcmp(cmd, "sim")) cmd_sim(); //Commands to a computer simulation

        else if(!strcmp(cmd, "dec")) cmd_dec(); //takes decimal value, prints hexadecimal value

        else if(!strcmp(cmd, "hex")) cmd_hex(); //takes hexadecimal value, prints decimal value

        else if(!strcmp(cmd, "list")) cmd_list(); //Lists selected tables

        else if(!strcmp(cmd, "help")) cmd_help(); //Prints all commands and how to use them

        else if(!strcmp(cmd, "snap")) cmd_snap(); //Prints all CPU states

        else if(!strcmp(cmd, "peek")) cmd_peek(); //Prints a value at a certain memory address

        else if(!strcmp(cmd, "poke")) cmd_poke(); //Changes a value at a certain memory address

        else if(!strcmp(cmd, "dump")) cmd_dump(); //Prints (Dumps) memory contents to a terminal or a bin file

        else if(!strcmp(cmd, "insert")) cmd_insert(); //Inserts string of values into memory at a certain memory address

        else printf("ERROR: That command does exist\n");
    }
}

//SIMULATOR COMMANDS

//Exits program
void cmd_done()
{
    exit(0);
}

//Does one clock tick
void cmd_step()
{
    update(); //Ticks clock

    stage++;
    if(stage > 7) stage = 0; //Bounds it's value between 0 and 7
}

//Does clock ticks until it finishes current cycle (Goes to the next multiple of 8)
void cmd_cycle()
{
    scanf("%d", &arg1); //Reads argument 'TIMES'

    for(int times = 0; times < arg1; times++)
    {
        for(stage; stage < 8; stage++)
            update(); //Ticks clock

        stage = 0;  
    }
}

//Commands to a computer simulation
void cmd_sim()
{
    scanf("%15s", str1); //Reads argument ACT

    if(!strcmp(str1, "auto")) //Sets simulation's clock to auto-tick
    {
        sim_auto = 1;
        sim_thread = std::thread(update_thread, freq);
        sim_thread.detach();

    }else if(!strcmp(str1, "manual")) //Sets simulation's clock to manual-tick
    {
        sim_auto = 0;

    }else if(!strcmp(str1, "freq")) //Sets simulation's clock frequency
    {
        scanf("%d", &arg1); //Reads argument F

        freq = arg1;
    }
}

//takes decimal value, prints hexadecimal value
void cmd_dec()
{
    scanf("%x", &arg1); //Reads argument VAL

    printf("%d \n", arg1);
}

//takes hexadecimal value, prints decimal value
void cmd_hex()
{
    scanf("%d", &arg1); //Reads argument VAL

    printf("0x%02x \n", arg1);
}

//Lists selected tables
void cmd_list()
{
    scanf("%15s", str1); //Reads argument TABLE

    if(!strcmp(str1, "opc")) //List opcodes
    {
        for(byte_t i = 0; i < 16; i++)
            printf("0x%1xX: %s \n", (byte_t)i, dcmp_opc(i));

        printf("\n");

    }else if(!strcmp(str1, "argsrc")) //List argument data sources
    {
        for(byte_t i = 0; i < 16; i++)
            printf("0xX%1x: %s \n", i, dcmp_arg(0, i));
        printf("\n");

    }else if(!strcmp(str1, "argcnd")) //List argument jump (branch) conditions
    {
        for(byte_t i = 0; i < 16; i++)
            printf("0xX%1x: %s \n", i, dcmp_arg(0, i));

        printf("\n");

    }else if(!strcmp(str1, "aluop")) //List ALU operations
    {
        printf("Sel; Mode:0, Cin:0           Mode:0, Cin:1,          Mode:1, Cin:X\n");

        for(byte_t i = 0; i < 16; i++)

        {
            printf("0x%1x: ", i);

            printf("%s", dcmp_op(i, 0, 0));
            printf("%s", dcmp_op(i, 0, 1));
            printf("%s", dcmp_op(i, 1, 0));

            printf("\n");
        }

        printf("\n");
    }
}

//Prints all commands and how to use them
void cmd_help()
{
    printf("done - - - - - - - -> Exits this program\n");
    printf("step - - - - - - - -> Ticks clock once\n");
    printf("cycle TIMES- - - - -> Finishes whole CPU execution cycle TIMES times\n");
    printf("sim ACT F- - - - - -> ACT 'auto' sets clock to auto, 'manual' sets clock to manual and 'freq' has additional argument 'F' which sets clock's frequency\n");
    printf("dec VAL- - - - - - -> Prints value VAL in a decimal base\n");
    printf("hex VAL- - - - - - -> Prints value VAL in a hexadecimal base\n");
    printf("list TABLE - - - - -> Lists TABLE table, where TABLE can be opc (Opcodes), argsrc (Data sources), argcnd (Jump conditions), aluop (ALU operations)\n");
    printf("help - - - - - - - -> Prints help list of all commnds (This is help command)\n");
    printf("snap CPU - - - - - -> Prints all current states CPU cpu states\n");
    printf("poke MEM ADR VAL - -> Writes hexadecimal VAL value to a memory MEM at address ADR\n");
    printf("peek MEM ADR - - - -> Prints a memory value from memory MEM at address ADR\n");
    printf("dump MEM LOC FILE- -> Prints all contents of a MEM memory to a LOC location, LOC can be 'terminal' or 'file', where file takes another argument FILE\n");
    printf("insert MEM ADR FILE-> Writes a string of values to a memory MEM at address ADR from '.bin' file FILE\n");

    printf("\n");
}

//Prints all CPU states
void cmd_snap()
{
    scanf("%d", &arg1); //Reads argument CPU
    CPU cpu = *cpus[arg1];

    printf("REG;   RA: 0x%02x    RB: 0x%02x   RC: 0x%02x    RESR: 0x%02x  DVR: 0x%02x \n", cpu.ra, cpu.rb, cpu.rc, cpu.resr, cpu.dvr);
    printf("INST;  Stage: %1d    IR: 0x%02x   Opc: %-5s  Arg: %-10s                   \n", stage, cpu.ir, dcmp_opc(cpu.opc), dcmp_arg(cpu.opc, cpu.arg));
    printf("PC;    SF: 0x%02x    PFH: 0x%02x  PFL: 0x%02x                             \n", trim(cpu.pc, 16, 20), trim(cpu.pc, 8, 16), trim(cpu.pc, 0, 8));
    printf("AP;    SF: 0x%02x    PFH: 0x%02x  PFL: 0x%02x                             \n", trim(cpu.ap, 16, 20), trim(cpu.ap, 8, 16), trim(cpu.ap, 0, 8));
    printf("PCI;   SF: 0x%02x    PFH: 0x%02x  PFL: 0x%02x                             \n", trim(cpu.pci, 16, 20), trim(cpu.pci, 8, 16), trim(cpu.pci, 0, 8));
    printf("API;   SF: 0x%02x    PFH: 0x%02x  PFL: 0x%02x                             \n", trim(cpu.api, 16, 20), trim(cpu.api, 8, 16), trim(cpu.api, 0, 8));
    printf("PS;    SFPS: 0x%02x  PS: 0x%02x   PRA: 0x%02x   PWA: 0x%02x               \n", cpu.sfps, cpu.ps, cpu.pra, cpu.pwa);
    printf("ALU;   ALU: 0x%02x   Op: %-24s                                            \n", cpu.alu, dcmp_op(cpu.op, cpu.mode, cpu.cin));
    printf("FLG;   EQU: %1d      FLGXN: %1d   INTMN: %1d    NEG: %1d      ODD: %1d    \n", cpu.equ, cpu.flgxn, cpu.intmn, cpu.neg, cpu.odd);

    printf("\n");
}

//Prints a value at a certain memory address
void cmd_peek()
{
    scanf("%d", &arg1); //Reads argument MEM
    scanf("%d", &arg2); //Reads argument ADR

    printf("0x%02x \n", mems[arg1][arg2]); //Print from a memory address
}

//Changes a value at a certain memory address
void cmd_poke()
{
    scanf("%d", &arg1); //Reads argument MEM
    scanf("%d", &arg2); //Reads argument ADR
    scanf("%x", &arg3); //Reads argument VAL

    mems[arg1][arg2] = arg3; //Write to a memory address
}

//Prints (Dumps) memory contents to a terminal or a bin file
void cmd_dump()
{
    scanf("%d", &arg1); //Reads argument MEM
    scanf("%15s", &str1); //Reads argument LOC

    byte_t *memory = mems[arg1]; //Selected memory to dump

    if(mems_size[arg1] > 65536) return; //Stops command for executing if memory size is too big

    if(!strcmp(str1, "terminal")) //Prints to a terminal
    {
        for(int y = 0; y < mems_size[arg1]; y += 32) //Print rows
        {
            printf("0xXX%04x: ", (pnt_t)y);

            //Print rows
            for(int x = 0; x < 32; x++)
            {
                byte_t value = memory[y+x]; //Value at a certain address in memory

                printf("0x%02x ", value);
            }

            printf("\n");
        }

    }else if(!strcmp(str1, "file")) //Prints to a selected file
    {
        scanf("%15s", &str2); //Reads argument FILE

        //Retriving values from a bin file
        FILE *p_file = fopen(str2, "wb");

        byte_t *tmp_mem = (byte_t*)malloc(arg3); //Creating temporary memory

        fwrite(mems[arg1], 1, mems_size[arg1], p_file); //Write to a memory address

        free(tmp_mem);
        fclose(p_file);
    }
}

//Inserts string of values into memory at a certain memory address
void cmd_insert()
{
    scanf("%d", &arg1); //Reads argument MEM
    scanf("%d", &arg2); //Reads argument ADR
    scanf("%15s", &str1); //Reads argument FILE
    scanf("%d", &arg3); //Reads argument SIZE

    FILE *p_file = fopen(str1, "rb"); //Pointer to a selected file

    if(p_file == NULL) //Wrong file name check
    {
        printf("Can't open this file");
        return;
    }

    byte_t *tmp_mem = (byte_t*)malloc(arg3); //Creating temporary memory

    fread(tmp_mem, 1, arg3, p_file); //Writing from file to a temporary memory

    //Writing from temporary memory to a selected memory address
    for(int times = 0; times < arg3; times++)
        mems[arg1][times +arg2] = tmp_mem[times];

    free(tmp_mem);
    fclose(p_file);
}

