//Osnova8 CPU c++ simulation library file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
10. september, 2026.
12. september, 2026.
13. september, 2026.
18. september, 2026.
19. september, 2026.
20. september, 2026.
21. september, 2026.
22. september, 2026.
23. september, 2026.
24. september, 2026.
25. september, 2026.
30. september, 2026.
2. October, 2026.
3. October, 2026.
4. October, 2026.
5. October, 2026.
13. November, 2026.
*/

#include "LIB_osnova_cpu.h" //Header file of this library

//Defining the Osnova8 CPU
CPU::CPU()
{
    //Flags (Conditions)
    neg = 0; //ALU's result is negative (Branch on RA ? RB < 0)
    carry = 0; //ALU's operation results in a CARRY (Branch on RA ? RB > 255)
    equ = 1; //RA equals RB (Branch on RA == RB)
    intm = 1; //CPU's INTterrupt Mode Negated (Branch on IF == 1)

    //Cnd arguments table
    cnd[0] = &neg;
    cnd[1] = &carry;
    cnd[2] = &equ;
    cnd[3] = &intm;


    //Pointers and pointer file
    pci = 0; //Program Counter Interrupt
    api = 0; //Address Pointer Interrupt
    pc = 0; //Program Counter
    ap = 0; //Address Pointer

    //Pointer file
    pf[0] = &pci;
    pf[1] = &api;
    pf[2] = &pc;
    pf[3] = &ap;

    //Sectors and sector file
    spci = 0; //Sector program Counter Interrupt
    sapi = 0; //Sector address Pointer Interrupt
    spc = 0; //Sector program Counter
    sap = 0; //Sector address Pointer

    //Sector file
    sf[0] = &spci;
    sf[1] = &sapi;
    sf[2] = &spc;
    sf[3] = &sap;

    //Genereal purpose registers
    r0 = 0; //Register 0
    r1 = 0; //Register 1
    r2 = 0; //Register 2
    r3 = 0; //Register 3

    //register file
    rf[0] = &r0;
    rf[1] = &r1;
    rf[2] = &r2;
    rf[3] = &r3;

    //Special purpose Register
    dvr = 0; //Direct Value Register

    //Values derived from other states
    alu = 1; //Arithmetic Logic Unit

    adrd = io_db; //ADdress Device (Just mirrors data bus)

    //Instruction Register and derived values
    ir = 0;
    opc = 0; //Opcode, higher 4-bit part of IR
    argo = 0; //Argument, lower 4-bit part of IR
    src = 0; //Opcode, higher 4-bit part of IR
    args = 0; //Argument, lower 4-bit part of IR

    //ALU
    op = 0; //(S0, S1, S2, S3) pins (Selection (Of operation)) 
    mode = 0; //M pin (Mode, 1 is for logic operations, 0 is for arithmetic operations)
    cin = 0; //Cn pin (Carry in (Pin is inverted in ALU))

    stage = 0; //CPU Cycle stage, affected by clock ticks (Clock is ticked by calling "updateCPU" function)
    i = 0; //Don't touch ^^'

    //BUS arguments table (Data sources)
    src[0] = &pf;
    src[1] = &MAX_BV;
    src[2] = &rf;
    src[3] = &sf;
    src[4] = &pfh;
    src[5] = &dvr;
    src[6] = adrd;
    src[7] = &intb;

    //Inputs and outputs
    *io_db; //Main CPU Data Bus
    *io_fx; //Flag X CPU pin (Inverted)
    *io_if; //CPU Interrupt mode Flag (Inverted)
    *io_itx; //Interrupt Trigger External (Triggers interrupt in other devices) (Inverted)
    *io_addw; //Address Device Write signal (Inverted)
    *io_addr; //Address Device Read signal (Inverted)
    *io_adrb; //ADdRess Bus (4-bit sectors, each sector 16-bit address space (1048576B = 1MB memory)
};

//Calculates ALU's operation result, ALU is 8-bit version of 74181 IC (equivalent of two 74181 cascaded)
byte_t CPU::calc_alu(byte_t a, byte_t b, int op, bool mode, int cin)
{
    cin = !cin; //Inverting Cin pin of ALU

    if(mode) //Logic operations
    {
        switch(op)
        {
            case 0:  return !a;        break;
            case 1:  return !(a || b); break;
            case 2:  return (!a) && b; break;
            case 3:  return 0;         break;
            case 4:  return !(a && b); break;
            case 5:  return !b;        break;
            case 6:  return a ^ b;     break;
            case 7:  return a && (!b); break;
            case 8:  return (!a) || b; break;
            case 9:  return !(a ^ b);  break;
            case 10: return b;         break;
            case 11: return a && b;    break;
            case 12: return 1;         break;
            case 13: return a || (!b); break;
            case 14: return a || b;    break;
            case 15: return a;         break;
        }
    }else //Arithmetic operations
    {
        switch(op)
        {
            case 0:  return a +cin;                    break;
            case 1:  return (a || b) +cin;             break;
            case 2:  return (a || (!b)) +cin;          break;
            case 3:  return 255 +cin;                  break;
            case 4:  return a+(a && (!b)) +cin;        break;
            case 5:  return (a || b)+(a && (!b)) +cin; break;
            case 6:  return a-b-1 +cin;                break;
            case 7:  return (a && (!b))-1 +cin;        break;
            case 8:  return a+(a && b) +cin;           break;
            case 9:  return a+b +cin;                  break;
            case 10: return (a || (!b))+(a && b) +cin; break;
            case 11: return (a && b)-1 +cin;           break;
            case 12: return a+a +cin;                  break;
            case 13: return (a || b)+a +cin;           break;
            case 14: return (a || (!b))+a +cin;        break;
            case 15: return a-1 +cin;                  break;
        }
    }

    return 0;
}

//CPU update (execution cycle update)
void CPU::update()
{
    //Exection cycle
    switch(stage)
    {
        //1. INSTRUCTION FETCH
        case 0: //Stage 0
            i = 0; //Interrupt mode check
            if(intmn) i = 2;

            *io_adrb = *pf[i]; //Setting address of a current instruction
            *io_addr = 0; //Releasing PC (Or PCI, depending on interrupt flag state)
        break;

        case 1: //Stage 1
            ir = *io_db; //Fetching instruction into IR
            opc = trim(ir, 4, 8); //Updating opcode value
            arg = trim(ir, 0, 4); //Updating argument value
        break;

        case 2: //Stage 2
            //No changes
        break;

        //2. INSTRUCTION EXECUTE
        case 3: //Stage 3
            *io_addr = 1; //Ending fetch stage

            //Setting address of a current pointer selected by PS
            pra = trim(ps, 4, 6);
            pwa = trim(ps, 6, 8);
            *io_adrb = *pf[pra];

            pfl = trim(*pf[pra], 0, 8); //Updating pointer file low source
            pfh = trim(*pf[pra], 8, 16); //Updating pointer file high source
            sfps = (trim(*pf[pra], 16, 20) <<8 ) + trim(ps, 0, 4); //Updating sector file, pointer selector source

            //Updating ALU source
            op = trim(dvr, 0, 4);
            mode = trim(dvr, 4, 5);
            cin = trim(dvr, 5, 6);
            alu = calc_alu(ra, rb, op, mode, cin);

            //Decoding argument part of instruction
            if(arg == 6) //Index of ADD data source
            {
                *io_db = MAX_BV;
                *io_addr = 0; //Gives signal to address device to output it's value to CPU'a data bus

            }else if(arg > 7) //CPU does not release any value to the data bus
                *io_db = MAX_BV;

            else
                *io_db = *src[arg]; //Releases data sources from within CPU

            //Flags check
            equ = ra == rb; //Are registers A and B equal?
            flgxn = !*io_fx; //Is flag x CPU pin zero?
            intmn = !*io_if; //Is CPU not in interrupt mode?
            carry = ra+rb > MAX_BV; //Is there a carry?
            neg = alu >= 128; //Is MSB one?
            odd = alu%2 == 1; //Is LSB one?
        break;

        case 4: //Stage 4
            switch(opc) //Decoding opcode part of instruction
            {
                //JuMP (Branches on Cnd == 1), copies value from one pointer to another (if the Arg's MSB is 0, transfered pointer is incremented)
                case 0: if(*cnd[arg]) *pf[pra] = (*pf[pwa]) +trim(arg, 3, 4); break;

                case 1: rc = *io_db; break; //Register C (SRC -> RC)

                case 2: sf = *io_db; break; //Sector File (SRC -> selected pointer of SF)

                case 3: pfl = *io_db; break; //Pointer File Low (SRC -> selected pointer of PFL)

                case 4: pfh = *io_db; break; //Pointer File High (SRC -> selected pointer of PFH)

                case 5: resr = *io_db; break; //REServed Register (SRC -> RESR)

                case 6: *io_addw = 0; break; //Address device write (SRC -> selected address of ADD)

                case 7: dvr = 0; break; //Direct Value Register Reset (DVR = 0), sets value to zero

                case 8: dvr += (dvr &HIGH_AREA) +arg; break; //Direct Value Register Low (ARG part of IR -> low part of DVR), overwrites lower part of DVR

                case 9: dvr += (dvr &LOW_AREA) +(arg <<4); break; //Direct Value Register High (ARG part of IR -> high part of DVR), overwrites higher part of DVR

                case 10: ra = *io_db; break; //Register A (SRC -> RA)

                case 11: rb = *io_db; break; //Register B (SRC -> RB)

                case 12: ps = trim(*io_db, 4, 8); break; //Pointer Selector (SRC -> PS), note its size and the way it is connected

                case 13: intmn = 0; break; //Interrupt Set Internal (Inverted Set), switches on CPU's interrupt mode

                case 14: *io_itx = 0; break; //Interrupt Set External (Inverted Set)

                case 15: intmn = 1; break; //Interrupt Enable (Inverted Reset), switches off CPU's interrupt mode
            }
        break;

        case 5: //Stage 5
            *io_itx = 1;
            *io_addw = 1;
        break;

        //3. POINTER INCREMENT
        case 6: //Stage 6
            *io_db = MAX_BV; //Ending execution stage

            i = 0; //Interrupt mode check
            if(intmn) i = 2;

            *io_addr = 0; //Releasing ADD at address PC (Or PCI, depending on interrupt flag state)
        break;

        case 7: //Stage 7
            if(*pf[i] >= ADR_AREA)
                *pf[i] = (*pf[i]) &SCT_AREA; //Spill check (Counting can only occur in address area of ADD, first 16 bits)
            else
                (*pf[i])++; //Incrementing PC (Or PCI, depending on interrupt flag state)

            *io_adrb = *pf[i]; //Setting address of the next instruction
        break;
    }

    stage++;
    if(stage > 7) stage = 0; //Reset cycle stage
}

