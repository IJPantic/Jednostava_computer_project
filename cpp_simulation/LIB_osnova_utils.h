//Osnova c++ utilities library header file, GPL-V3
//Created by Ivan Jonjic (IJPantic on github)

/*
UPDATED ON:
22. september, 2026.
23. september, 2026.
25. september, 2026.
25. september, 2026.
1. October, 2026.
2. October, 2026.
3. October, 2026.
4. October, 2026.
5. October, 2026.
6. October, 2026.
*/

#include <cstdint>
#include <math.h>

#ifndef LIB_osnova_utils
#define LIB_osnova_utils

//DATA TYPES

using byte_t = uint8_t; //CPU's word, 8-bit
using pnt_t = int; //Pointer, 20-bit

//UNIVERSAL CONSTANTS

extern byte_t MAX_BV; //Max byte value
extern pnt_t MAX_PV; //Max pointer value

extern const byte_t LOW_AREA; //Low area mask of a byte
extern const byte_t HIGH_AREA; //High area mask of a byte

extern const pnt_t ADR_AREA; //Address area mask of ADD address space
extern const pnt_t SCT_AREA; //Sector area mask of ADD address

//FUNCTIONS

//Trims off all bits in a byte except selected part
byte_t trim(byte_t value, int start, int end);

//Trims off all bits in a pointer except selected part
pnt_t trim(pnt_t value, int start, int end);

//Argument decompile table (Turns argument number into argument's name)
const char *dcmp_arg(byte_t opc, byte_t arg);

//Opcode decompile table (Turns opcode number into opcode's name)
const char *dcmp_opc(byte_t opc);

//ALU operations decompile table (Turns ALU op number into argument's name)
const char *dcmp_op(int op, bool mode, int cin);

#endif

