//Osnova c++ utilities library file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
22. september, 2026.
23. september, 2026.
25. september, 2026.
1. October, 2026.
2. October, 2026.
3. October, 2026.
4. October, 2026.
5. October, 2026.
*/

#include "LIB_osnova_utils.h" //Header file of this library

//UNIVERSAL CONSTANTS

byte_t MAX_BV = 0xff; //Max byte value
pnt_t MAX_PV = 0xffffff; //Max pointer value

const byte_t LOW_AREA = 0x0f; //Low area mask of a byte
const byte_t HIGH_AREA = 0xf0; //High area mask of a byte

const pnt_t ADR_AREA = 0x0ffff; //Address area mask of ADD address space
const pnt_t SCT_AREA = 0xf0000; //Sector area mask of ADD address

//FUNCTIONS

//Trims off all bits in a byte except selected part
byte_t trim(byte_t value, int start, int end)
{
    value = value >>start;

    int mask = 0;
    for(int i=0; i<end-start; i++)
    {
        mask += pow(2, i);
    }

    value = value & mask;

    return value;
}

//Trims off all bits in a pointer except selected part
pnt_t trim(pnt_t value, int start, int end)
{
    value = value >>start;

    int mask = 0;
    for(int i=0; i<end-start; i++)
    {
        mask += pow(2, i);
    }

    value = value & mask;

    return value;
}

//Argument decompile table (Turns argument number into argument's name)
const char *dcmp_arg(byte_t opc, byte_t arg)
{
    if(opc == 8 || opc == 9) //Opcode is "dvrl" or "dvrh" instruction
    {
        switch(arg) //Argument is for a value
        {
            case 0:  return "0x0"; break;
            case 1:  return "0x1"; break;
            case 2:  return "0x2"; break;
            case 3:  return "0x3"; break;
            case 4:  return "0x4"; break;
            case 5:  return "0x5"; break;
            case 6:  return "0x6"; break;
            case 7:  return "0x7"; break;
            case 8:  return "0x8"; break;
            case 9:  return "0x9"; break;
            case 10: return "0xa"; break;
            case 11: return "0xb"; break;
            case 12: return "0xc"; break;
            case 13: return "0xd"; break;
            case 14: return "0xe"; break;
            case 15: return "0xf"; break;
        }

    }else if(opc == 0) //Opcode is "jmp" instruction
    {
        switch(arg) //Argument is for jump (branch) conditions
        {
            case 0:  return "nc, inc";    break;
            case 1:  return "equ, inc";   break;
            case 2:  return "flgxn, inc"; break;
            case 3:  return "intmn, inc"; break;
            case 4:  return "carry, inc"; break;
            case 5:  return "neg, inc";   break;
            case 6:  return "odd, inc";   break;
            case 7:  return "dont, inc";  break;
            case 8:  return "nc";         break;
            case 9:  return "equ";        break;
            case 10: return "flgxn";      break;
            case 11: return "intmn";      break;
            case 12: return "carry";      break;
            case 13: return "neg";        break;
            case 14: return "odd";        break;
            case 15: return "dont";       break;
        }

    }else if(opc == 12) //Opcode is "ps" instruction
    {
        switch(trim(arg, 4, 8)) //From high argument part, first 2 bits are for "pra" and other 2 bits are for "pwa"
        {
            case 0:  return "pc -> pc";   break;
            case 1:  return "ap -> pc";   break;
            case 2:  return "pci -> pc";  break;
            case 3:  return "api -> pc";  break;
            case 4:  return "pc -> ap";   break;
            case 5:  return "ap -> ap";   break;
            case 6:  return "pci -> ap";  break;
            case 7:  return "api -> ap";  break;
            case 8:  return "pc -> pci";  break;
            case 9:  return "ap -> pci";  break;
            case 10: return "pci -> pci"; break;
            case 11: return "api -> pci"; break;
            case 12: return "pc -> api";  break;
            case 13: return "ap -> api";  break;
            case 14: return "pci -> api"; break;
            case 15: return "api -> api"; break;
        }

    }else if(opc == 7 || opc > 12) //Opcode is "dvrr", "intsx", "intsi" or "inte" instruction
    {
        return "x"; //Instruction which doesn't take arguments

    }else
    {
        switch(arg)
        {
            case 0:  return "alu";  break;
            case 1:  return "rc";   break;
            case 2:  return "sfps"; break;
            case 3:  return "pfl";  break;
            case 4:  return "pfh";  break;
            case 5:  return "resr"; break;
            case 6:  return "add";  break;
            case 7:  return "dvr";  break;
            case 8:  return "x";    break;
            case 9:  return "x";    break;
            case 10: return "x";    break;
            case 11: return "x";    break;
            case 12: return "x";    break;
            case 13: return "x";    break;
            case 14: return "x";    break;
            case 15: return "x";    break;
        }
    }

    return "ERROR";
}

//Opcode decompile table (Turns opcode number into opcode's name)
const char *dcmp_opc(byte_t opc)
{
    switch(opc)
    {
        case 0:  return "jmp";   break;
        case 1:  return "rc";    break;
        case 2:  return "sf";    break;
        case 3:  return "pfl";   break;
        case 4:  return "pfh";   break;
        case 5:  return "resr";  break;
        case 6:  return "add";   break;
        case 7:  return "dvrr";  break;
        case 8:  return "dvrl";  break;
        case 9:  return "dvrh";  break;
        case 10: return "ra";    break;
        case 11: return "rb";    break;
        case 12: return "ps";    break;
        case 13: return "intsi"; break;
        case 14: return "intsx"; break;
        case 15: return "inte";  break;
    }

    return "ERROR";
}

//ALU operations decompile table (Turns ALU op number into argument's name)
const char *dcmp_op(int op, bool mode, int cin)
{
    if(mode) //Logic operations
    {
        switch(op)
        {
            case 0:  return "!A";        break;
            case 1:  return "!(A || B)"; break;
            case 2:  return "(!A) && B"; break;
            case 3:  return "0";         break;
            case 4:  return "!(A && B)"; break;
            case 5:  return "!B";        break;
            case 6:  return "A ^ B";     break;
            case 7:  return "A && (!B)"; break;
            case 8:  return "(!A) || B"; break;
            case 9:  return "!(A ^ B)";  break;
            case 10: return "B";         break;
            case 11: return "A && B";    break;
            case 12: return "1";         break;
            case 13: return "A || (!B)"; break;
            case 14: return "A || B";    break;
            case 15: return "A";         break;
        }

    }else //Arithmetic operations
    {
        if(cin) //Without carry in
        {
            switch(op)
            {
                case 0:  return "A";                    break;
                case 1:  return "(A || B)";             break;
                case 2:  return "(A || (!B))";          break;
                case 3:  return "255";                  break;
                case 4:  return "A+(A && (!B))";        break;
                case 5:  return "(A || B)+(A && (!B))"; break;
                case 6:  return "A-B-1";                break;
                case 7:  return "(A && (!B))-1";        break;
                case 8:  return "A+(A && B)";           break;
                case 9:  return "A+B";                  break;
                case 10: return "(A || (!B))+(A && B)"; break;
                case 11: return "(A && B)-1";           break;
                case 12: return "A+A";                  break;
                case 13: return "(A || B)+A";           break;
                case 14: return "(A || (!B))+A";        break;
                case 15: return "A-1";                  break;
            }

        }else //With carry in
        {
            switch(op)
            {
                case 0:  return "A +1";                    break;
                case 1:  return "(A || B) +1";             break;
                case 2:  return "(A || (!B)) +1";          break;
                case 3:  return "0";                       break;
                case 4:  return "A+(A && (!B)) +1";        break;
                case 5:  return "(A || B)+(A && (!B)) +1"; break;
                case 6:  return "A-B";                     break;
                case 7:  return "(A && (!B))";             break;
                case 8:  return "A+(A && B) +1";           break;
                case 9:  return "A+B +1";                  break;
                case 10: return "(A || (!B))+(A && B) +1"; break;
                case 11: return "(A && B)";                break;
                case 12: return "A+A +1";                  break;
                case 13: return "(A || B)+A +1";           break;
                case 14: return "(A || (!B))+A +1";        break;
                case 15: return "A";                       break;
            }
        }
    }

    return "ERROR";
}

