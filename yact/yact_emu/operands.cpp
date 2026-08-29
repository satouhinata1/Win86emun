#include "stdafx.h"
#include "yact_emu.h"
#include "../inc/x86emul.h"

DWORD ReadReg(PCPU_STATE cpu, int reg_idx) {
    switch (reg_idx) {
        case 0: return cpu->eax; case 1: return cpu->ecx;
        case 2: return cpu->edx; case 3: return cpu->ebx;
        case 4: return cpu->esp; case 5: return cpu->ebp;
        case 6: return cpu->esi; case 7: return cpu->edi;
        default: return 0;
    }
}
void WriteReg(PCPU_STATE cpu, int reg_idx, DWORD value) {
    switch (reg_idx) {
        case 0: cpu->eax = value; break; case 1: cpu->ecx = value; break;
        case 2: cpu->edx = value; break; case 3: cpu->ebx = value; break;
        case 4: cpu->esp = value; break; case 5: cpu->ebp = value; break;
        case 6: cpu->esi = value; break; case 7: cpu->edi = value; break;
    }
}
DWORD ReadMem32(DWORD addr) { return *(DWORD*)addr; }
void WriteMem32(DWORD addr, DWORD value) { *(DWORD*)addr = value; }

int ResolveOperand(x86opcode *Op, int operand_index, DWORD *out_addr, BOOL *is_memory) {
    BYTE modrm = Op->ModRM;
    BYTE mod = (modrm >> 6) & 0x03;
    BYTE reg = (modrm >> 3) & 0x07;
    BYTE rm = modrm & 0x07;

    if (operand_index == 1) {
        *is_memory = FALSE;
        return reg;
    }

    if (mod == 3) {
        *is_memory = FALSE;
        return rm;
    }

    DWORD base = 0, index = 0, scale = 1, disp = 0;
    if (mod != 3 && rm == 4) {
        BYTE sib = Op->SIB;
        BYTE base_val = sib & 0x07;
        BYTE index_val = (sib >> 3) & 0x07;
        BYTE scale_val = (sib >> 6) & 0x03;
        scale = (1 << scale_val);
        if (index_val != 4) index = ReadReg(Op->Cpu, index_val);
        if (base_val == 5 && mod == 0) base = 0;
        else base = ReadReg(Op->Cpu, base_val);
    } else {
        static const int map[8] = {0,1,2,3,4,5,6,7};
        if (mod == 0 && rm == 5) base = 0;
        else base = ReadReg(Op->Cpu, map[rm]);
    }

    if (mod == 0 && rm == 5) disp = Op->Imm32;
    else if (mod == 1) { disp = (BYTE)Op->Imm8; if (disp & 0x80) disp |= 0xFFFFFF00; }
    else if (mod == 2) disp = Op->Imm32;
    else if (mod == 0 && rm == 4) disp = Op->Imm32;

    *out_addr = base + (index * scale) + disp;
    *is_memory = TRUE;
    return 0xFFFFFFFF;
}
