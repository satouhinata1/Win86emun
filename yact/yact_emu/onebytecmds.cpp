#include "stdafx.h"
#include "yact_emu.h"

// Forward declarations from operands.cpp
extern DWORD ReadReg(PCPU_STATE cpu, int reg_idx);
extern void WriteReg(PCPU_STATE cpu, int reg_idx, DWORD value);
extern DWORD ReadMem32(DWORD addr);
extern void WriteMem32(DWORD addr, DWORD value);
extern int ResolveOperand(x86opcode *Op, int operand_index, DWORD *out_addr, BOOL *is_memory);

static bool FASTCALL Cmd00(x86opcode *Op)
{
    Op->Mnemonics = "add";
    FetchModRM(Op);
    return false;
}

// ADD r/m, reg (Cmd01)
static bool FASTCALL Cmd01(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    DWORD res = op1 + op2;
    if (is_mem) WriteMem32(addr, res);
    else WriteReg(Op->Cpu, rm_idx, res);
    UpdateFlags_Add(Op->Cpu, res, op1, op2);
    Op->Cpu->eip += 2;
    return false;
}

static bool FASTCALL Cmd02(x86opcode *Op)
{
    Op->Mnemonics = "add";
    return false;
}

static bool FASTCALL Cmd03(x86opcode *Op)
{
    Op->Mnemonics = "add";
    return false;
}

static bool FASTCALL Cmd04(x86opcode *Op)
{
    Op->Mnemonics = "add";
    return false;
}

static bool FASTCALL Cmd05(x86opcode *Op)
{
    Op->Mnemonics = "add";
    return false;
}

static bool FASTCALL Cmd06(x86opcode *Op) // PUSH ES
{
    Op->Cpu->esp -= 4;
    WriteMem32(Op->Cpu->esp, (DWORD)Op->Cpu->es);
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd07(x86opcode *Op) // POP ES
{
    Op->Cpu->es = (WORD)ReadMem32(Op->Cpu->esp);
    Op->Cpu->esp += 4;
    Op->Cpu->eip += 1;
    return false;
}

// AND r/m, reg (Cmd20) ～ AND AL, imm8 (Cmd24)
static bool FASTCALL Cmd20(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); DWORD res = op1 & op2; if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd21(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); DWORD res = op1 & op2; if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd22(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); DWORD res = op1 & op2; if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd23(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); DWORD res = op1 & op2; WriteReg(Op->Cpu, reg_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd24(x86opcode *Op) { BYTE imm8 = FetchImm8(Op); DWORD op1 = Op->Cpu->eax & 0xFF; DWORD op2 = imm8 & 0xFF; DWORD res = op1 & op2; Op->Cpu->eax = (Op->Cpu->eax & ~0xFF) | res; UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd25(x86opcode *Op) { DWORD imm32 = FetchImm32(Op); DWORD res = Op->Cpu->eax & imm32; Op->Cpu->eax = res; UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 5; return false; }

// OR r/m, reg (Cmd08) ～ OR AL, imm8 (Cmd0C)
static bool FASTCALL Cmd08(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); DWORD res = op1 | op2; if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd09(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); DWORD res = op1 | op2; if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd0A(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); DWORD res = op1 | op2; if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd0B(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); DWORD res = op1 | op2; WriteReg(Op->Cpu, reg_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd0C(x86opcode *Op) { BYTE imm8 = FetchImm8(Op); DWORD res = (Op->Cpu->eax & ~0xFF) | ((Op->Cpu->eax & 0xFF) | imm8); Op->Cpu->eax = res; UpdateFlags_Logical(Op->Cpu, res & 0xFF); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd0D(x86opcode *Op) { DWORD imm32 = FetchImm32(Op); DWORD res = Op->Cpu->eax | imm32; Op->Cpu->eax = res; UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 5; return false; }

// XOR r/m, reg (Cmd30) ～ XOR AL, imm8 (Cmd34)
static bool FASTCALL Cmd30(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); DWORD res = op1 ^ op2; if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd31(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); DWORD res = op1 ^ op2; if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd32(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); DWORD res = op1 ^ op2; if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd33(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); DWORD res = op1 ^ op2; WriteReg(Op->Cpu, reg_idx, res); UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd34(x86opcode *Op) { BYTE imm8 = FetchImm8(Op); DWORD res = (Op->Cpu->eax & ~0xFF) | ((Op->Cpu->eax & 0xFF) ^ imm8); Op->Cpu->eax = res; UpdateFlags_Logical(Op->Cpu, res & 0xFF); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd35(x86opcode *Op) { DWORD imm32 = FetchImm32(Op); DWORD res = Op->Cpu->eax ^ imm32; Op->Cpu->eax = res; UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 5; return false; }

// TEST r/m, reg (Cmd84) ～ TEST EAX, imm32 (Cmd85)
static bool FASTCALL Cmd84(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); DWORD res = op1 & op2; UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd85(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); DWORD res = op1 & op2; UpdateFlags_Logical(Op->Cpu, res); Op->Cpu->eip += 2; return false; }

static bool FASTCALL Cmd0E(x86opcode *Op) // PUSH CS
{
    Op->Cpu->esp -= 4;
    WriteMem32(Op->Cpu->esp, (DWORD)Op->Cpu->cs);
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd0F(x86opcode *Op)
{
    Op->Current = FetchB(Op);
    return (TwoByteCmds[Op->Current])(Op);;
}


// ADC r/m, reg (Cmd10) ～ ADC EAX, imm32 (Cmd15)
static bool FASTCALL Cmd10(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    BOOL carry_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 + op2 + (carry_in ? 1 : 0);
    if (is_mem) WriteMem32(addr, res);
    else WriteReg(Op->Cpu, rm_idx, res);
    UpdateFlags_Adc(Op->Cpu, res, op1, op2, carry_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd11(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    BOOL carry_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 + op2 + (carry_in ? 1 : 0);
    if (is_mem) WriteMem32(addr, res);
    else WriteReg(Op->Cpu, rm_idx, res);
    UpdateFlags_Adc(Op->Cpu, res, op1, op2, carry_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd12(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    BOOL carry_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 + op2 + (carry_in ? 1 : 0);
    if (is_mem) WriteMem32(addr, res);
    else WriteReg(Op->Cpu, rm_idx, res);
    UpdateFlags_Adc(Op->Cpu, res, op1, op2, carry_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd13(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    BOOL carry_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 + op2 + (carry_in ? 1 : 0);
    WriteReg(Op->Cpu, reg_idx, res);
    UpdateFlags_Adc(Op->Cpu, res, op1, op2, carry_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd14(x86opcode *Op) {
    BYTE imm8 = FetchImm8(Op);
    DWORD op1 = Op->Cpu->eax & 0xFF;
    DWORD op2 = imm8 & 0xFF;
    BOOL carry_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 + op2 + (carry_in ? 1 : 0);
    Op->Cpu->eax = (Op->Cpu->eax & ~0xFF) | (res & 0xFF);
    UpdateFlags_Adc(Op->Cpu, res & 0xFF, op1, op2, carry_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd15(x86opcode *Op) {
    DWORD imm32 = FetchImm32(Op);
    DWORD op1 = Op->Cpu->eax;
    DWORD op2 = imm32;
    BOOL carry_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 + op2 + (carry_in ? 1 : 0);
    Op->Cpu->eax = res;
    UpdateFlags_Adc(Op->Cpu, res, op1, op2, carry_in);
    Op->Cpu->eip += 5;
    return false;
}

static bool FASTCALL Cmd16(x86opcode *Op) // PUSH SS
{
    Op->Cpu->esp -= 4;
    WriteMem32(Op->Cpu->esp, (DWORD)Op->Cpu->ss);
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd17(x86opcode *Op) // POP SS
{
    Op->Cpu->ss = (WORD)ReadMem32(Op->Cpu->esp);
    Op->Cpu->esp += 4;
    Op->Cpu->eip += 1;
    return false;
}

// SBB r/m, reg (Cmd18) ～ SBB EAX, imm32 (Cmd1D)
static bool FASTCALL Cmd18(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    BOOL borrow_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 - op2 - (borrow_in ? 1 : 0);
    if (is_mem) WriteMem32(addr, res);
    else WriteReg(Op->Cpu, rm_idx, res);
    UpdateFlags_Sbb(Op->Cpu, res, op1, op2, borrow_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd19(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    BOOL borrow_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 - op2 - (borrow_in ? 1 : 0);
    if (is_mem) WriteMem32(addr, res);
    else WriteReg(Op->Cpu, rm_idx, res);
    UpdateFlags_Sbb(Op->Cpu, res, op1, op2, borrow_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd1A(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    BOOL borrow_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 - op2 - (borrow_in ? 1 : 0);
    if (is_mem) WriteMem32(addr, res);
    else WriteReg(Op->Cpu, rm_idx, res);
    UpdateFlags_Sbb(Op->Cpu, res, op1, op2, borrow_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd1B(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    BOOL borrow_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 - op2 - (borrow_in ? 1 : 0);
    WriteReg(Op->Cpu, reg_idx, res);
    UpdateFlags_Sbb(Op->Cpu, res, op1, op2, borrow_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd1C(x86opcode *Op) {
    BYTE imm8 = FetchImm8(Op);
    DWORD op1 = Op->Cpu->eax & 0xFF;
    DWORD op2 = imm8 & 0xFF;
    BOOL borrow_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 - op2 - (borrow_in ? 1 : 0);
    Op->Cpu->eax = (Op->Cpu->eax & ~0xFF) | (res & 0xFF);
    UpdateFlags_Sbb(Op->Cpu, res & 0xFF, op1, op2, borrow_in);
    Op->Cpu->eip += 2;
    return false;
}
static bool FASTCALL Cmd1D(x86opcode *Op) {
    DWORD imm32 = FetchImm32(Op);
    DWORD op1 = Op->Cpu->eax;
    DWORD op2 = imm32;
    BOOL borrow_in = (Op->Cpu->eflags & EFLAGS_CF) ? TRUE : FALSE;
    DWORD res = op1 - op2 - (borrow_in ? 1 : 0);
    Op->Cpu->eax = res;
    UpdateFlags_Sbb(Op->Cpu, res, op1, op2, borrow_in);
    Op->Cpu->eip += 5;
    return false;
}

static bool FASTCALL Cmd1E(x86opcode *Op) // PUSH DS
{
    Op->Cpu->esp -= 4;
    WriteMem32(Op->Cpu->esp, (DWORD)Op->Cpu->ds);
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd1F(x86opcode *Op) // POP DS
{
    Op->Cpu->ds = (WORD)ReadMem32(Op->Cpu->esp);
    Op->Cpu->esp += 4;
    Op->Cpu->eip += 1;
    return false;
}

// Removed duplicate definitions - Cmd20-Cmd25 already defined above at lines 75-80

static bool FASTCALL Cmd26(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd27(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd28(x86opcode *Op)
{
    return false;
}

// SUB r/m, reg (Cmd29)
static bool FASTCALL Cmd29(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    DWORD res = op1 - op2;
    if (is_mem) WriteMem32(addr, res);
    else WriteReg(Op->Cpu, rm_idx, res);
    UpdateFlags_Sub(Op->Cpu, res, op1, op2);
    Op->Cpu->eip += 2;
    return false;
}

static bool FASTCALL Cmd2A(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd2B(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd2C(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd2D(x86opcode *Op)
{
    return false;
}

// SegCS
static bool FASTCALL Cmd2E(x86opcode *Op)
{
    return true;
}

static bool FASTCALL Cmd2F(x86opcode *Op)
{
    return false;
}

// Removed duplicate definitions - Cmd30-Cmd35 already defined above at lines 91-96

static bool FASTCALL Cmd36(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd37(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd38(x86opcode *Op)
{
    return false;
}

// CMP r/m, reg (Cmd39)
static bool FASTCALL Cmd39(x86opcode *Op) {
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem);
    DWORD op2 = ReadReg(Op->Cpu, reg_idx);
    DWORD res = op1 - op2;
    UpdateFlags_Sub(Op->Cpu, res, op1, op2);
    Op->Cpu->eip += 2;
    return false;
}

static bool FASTCALL Cmd3A(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd3B(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd3C(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd3D(x86opcode *Op)
{
    return false;
}

// SegDS
static bool FASTCALL Cmd3E(x86opcode *Op)
{
    return true;
}

static bool FASTCALL Cmd3F(x86opcode *Op)
{
    return false;
}

// INC EAX (Cmd40) ～ INC EDI (Cmd47)
static bool FASTCALL Cmd40(x86opcode *Op) { DWORD v = Op->Cpu->eax + 1; Op->Cpu->eax = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->eax & 0xF) == 0) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd41(x86opcode *Op) { DWORD v = Op->Cpu->ecx + 1; Op->Cpu->ecx = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->ecx & 0xF) == 0) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd42(x86opcode *Op) { DWORD v = Op->Cpu->edx + 1; Op->Cpu->edx = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->edx & 0xF) == 0) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd43(x86opcode *Op) { DWORD v = Op->Cpu->ebx + 1; Op->Cpu->ebx = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->ebx & 0xF) == 0) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd44(x86opcode *Op) { DWORD v = Op->Cpu->esp + 1; Op->Cpu->esp = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->esp & 0xF) == 0) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd45(x86opcode *Op) { DWORD v = Op->Cpu->ebp + 1; Op->Cpu->ebp = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->ebp & 0xF) == 0) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd46(x86opcode *Op) { DWORD v = Op->Cpu->esi + 1; Op->Cpu->esi = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->esi & 0xF) == 0) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd47(x86opcode *Op) { DWORD v = Op->Cpu->edi + 1; Op->Cpu->edi = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->edi & 0xF) == 0) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }

// DEC EAX (Cmd48) ～ DEC EDI (Cmd4F)
static bool FASTCALL Cmd48(x86opcode *Op) { DWORD v = Op->Cpu->eax - 1; Op->Cpu->eax = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x7FFFFFFF) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->eax & 0xF) == 0xF) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd49(x86opcode *Op) { DWORD v = Op->Cpu->ecx - 1; Op->Cpu->ecx = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x7FFFFFFF) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->ecx & 0xF) == 0xF) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd4A(x86opcode *Op) { DWORD v = Op->Cpu->edx - 1; Op->Cpu->edx = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x7FFFFFFF) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->edx & 0xF) == 0xF) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd4B(x86opcode *Op) { DWORD v = Op->Cpu->ebx - 1; Op->Cpu->ebx = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x7FFFFFFF) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->ebx & 0xF) == 0xF) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd4C(x86opcode *Op) { DWORD v = Op->Cpu->esp - 1; Op->Cpu->esp = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x7FFFFFFF) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->esp & 0xF) == 0xF) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd4D(x86opcode *Op) { DWORD v = Op->Cpu->ebp - 1; Op->Cpu->ebp = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x7FFFFFFF) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->ebp & 0xF) == 0xF) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd4E(x86opcode *Op) { DWORD v = Op->Cpu->esi - 1; Op->Cpu->esi = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x7FFFFFFF) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->esi & 0xF) == 0xF) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd4F(x86opcode *Op) { DWORD v = Op->Cpu->edi - 1; Op->Cpu->edi = v; Op->Cpu->eflags &= ~(EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF); if (v == 0x7FFFFFFF) Op->Cpu->eflags |= EFLAGS_OF; if (v & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; if (v == 0) Op->Cpu->eflags |= EFLAGS_ZF; if ((Op->Cpu->edi & 0xF) == 0xF) Op->Cpu->eflags |= EFLAGS_AF; if (CalcParity(v)) Op->Cpu->eflags |= EFLAGS_PF; Op->Cpu->eip += 1; return false; }

// PUSH EAX (Cmd50) ～ PUSH EDI (Cmd57)
static bool FASTCALL Cmd50(x86opcode *Op) { Op->Cpu->esp -= 4; WriteMem32(Op->Cpu->esp, Op->Cpu->eax); Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd51(x86opcode *Op) { Op->Cpu->esp -= 4; WriteMem32(Op->Cpu->esp, Op->Cpu->ecx); Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd52(x86opcode *Op) { Op->Cpu->esp -= 4; WriteMem32(Op->Cpu->esp, Op->Cpu->edx); Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd53(x86opcode *Op) { Op->Cpu->esp -= 4; WriteMem32(Op->Cpu->esp, Op->Cpu->ebx); Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd54(x86opcode *Op) { Op->Cpu->esp -= 4; WriteMem32(Op->Cpu->esp, Op->Cpu->esp + 4); Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd55(x86opcode *Op) { Op->Cpu->esp -= 4; WriteMem32(Op->Cpu->esp, Op->Cpu->ebp); Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd56(x86opcode *Op) { Op->Cpu->esp -= 4; WriteMem32(Op->Cpu->esp, Op->Cpu->esi); Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd57(x86opcode *Op) { Op->Cpu->esp -= 4; WriteMem32(Op->Cpu->esp, Op->Cpu->edi); Op->Cpu->eip += 1; return false; }

// POP EAX (Cmd58) ～ POP EDI (Cmd5F)
static bool FASTCALL Cmd58(x86opcode *Op) { Op->Cpu->eax = ReadMem32(Op->Cpu->esp); Op->Cpu->esp += 4; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd59(x86opcode *Op) { Op->Cpu->ecx = ReadMem32(Op->Cpu->esp); Op->Cpu->esp += 4; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd5A(x86opcode *Op) { Op->Cpu->edx = ReadMem32(Op->Cpu->esp); Op->Cpu->esp += 4; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd5B(x86opcode *Op) { Op->Cpu->ebx = ReadMem32(Op->Cpu->esp); Op->Cpu->esp += 4; Op->Cpu->eip += 1; return false; }

static bool FASTCALL Cmd5C(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd5D(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd5E(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd5F(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd60(x86opcode *Op) // PUSHA - Push All General Purpose Registers
{
    DWORD esp_temp = Op->Cpu->esp;
    esp_temp -= 4; WriteMem32(esp_temp, Op->Cpu->eax);
    esp_temp -= 4; WriteMem32(esp_temp, Op->Cpu->ecx);
    esp_temp -= 4; WriteMem32(esp_temp, Op->Cpu->edx);
    esp_temp -= 4; WriteMem32(esp_temp, Op->Cpu->ebx);
    esp_temp -= 4; WriteMem32(esp_temp, Op->Cpu->esp); // Original ESP
    esp_temp -= 4; WriteMem32(esp_temp, Op->Cpu->ebp);
    esp_temp -= 4; WriteMem32(esp_temp, Op->Cpu->esi);
    esp_temp -= 4; WriteMem32(esp_temp, Op->Cpu->edi);
    Op->Cpu->esp = esp_temp;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd61(x86opcode *Op) // POPA - Pop All General Purpose Registers
{
    DWORD esp_temp = Op->Cpu->esp;
    Op->Cpu->edi = ReadMem32(esp_temp); esp_temp += 4;
    Op->Cpu->esi = ReadMem32(esp_temp); esp_temp += 4;
    Op->Cpu->ebp = ReadMem32(esp_temp); esp_temp += 4;
    esp_temp += 4; // Skip ESP (popped value is discarded)
    Op->Cpu->ebx = ReadMem32(esp_temp); esp_temp += 4;
    Op->Cpu->edx = ReadMem32(esp_temp); esp_temp += 4;
    Op->Cpu->ecx = ReadMem32(esp_temp); esp_temp += 4;
    Op->Cpu->eax = ReadMem32(esp_temp); esp_temp += 4;
    Op->Cpu->esp = esp_temp;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd62(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd63(x86opcode *Op)
{
    return false;
}

// SegFS
static bool FASTCALL Cmd64(x86opcode *Op)
{
    Op->SegFS = true;
    return true;
}

// SegGS
static bool FASTCALL Cmd65(x86opcode *Op)
{
    return true;
}

// OpSize
static bool FASTCALL Cmd66(x86opcode *Op)
{
    Op->Opsize = true;
    return true;
}

// AddrSize
static bool FASTCALL Cmd67(x86opcode *Op)
{
    Op->Addrsize = true;
    return true;
}

static bool FASTCALL Cmd68(x86opcode *Op) // PUSH imm32
{
    DWORD imm32 = FetchImm32(Op);
    Op->Cpu->esp -= 4;
    WriteMem32(Op->Cpu->esp, imm32);
    Op->Cpu->eip += 5;
    return false;
}

static bool FASTCALL Cmd69(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd6A(x86opcode *Op) // PUSH imm8
{
    BYTE imm8 = FetchImm8(Op);
    Op->Cpu->esp -= 4;
    WriteMem32(Op->Cpu->esp, (DWORD)(signed char)imm8); // Sign-extend
    Op->Cpu->eip += 2;
    return false;
}

static bool FASTCALL Cmd6B(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd6C(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd6D(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd6E(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd6F(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd70(x86opcode *Op) // JO - Jump if Overflow
{
    signed char imm8 = (signed char)FetchB(Op);
    if (Op->Cpu->eflags & EFLAGS_OF)
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd71(x86opcode *Op) // JNO - Jump if Not Overflow
{
    signed char imm8 = (signed char)FetchB(Op);
    if (!(Op->Cpu->eflags & EFLAGS_OF))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd72(x86opcode *Op) // JB/JNAE - Jump if Below / Not Above or Equal
{
    signed char imm8 = (signed char)FetchB(Op);
    if (Op->Cpu->eflags & EFLAGS_CF)
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd73(x86opcode *Op) // JAE/JNB - Jump if Above or Equal / Not Below
{
    signed char imm8 = (signed char)FetchB(Op);
    if (!(Op->Cpu->eflags & EFLAGS_CF))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd74(x86opcode *Op) // JZ/JE - Jump if Zero / Equal
{
    signed char imm8 = (signed char)FetchB(Op);
    if (Op->Cpu->eflags & EFLAGS_ZF)
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd75(x86opcode *Op) // JNZ/JNE - Jump if Not Zero / Not Equal
{
    signed char imm8 = (signed char)FetchB(Op);
    if (!(Op->Cpu->eflags & EFLAGS_ZF))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd76(x86opcode *Op) // JBE/JNA - Jump if Below or Equal / Not Above
{
    signed char imm8 = (signed char)FetchB(Op);
    if (Op->Cpu->eflags & (EFLAGS_CF | EFLAGS_ZF))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd77(x86opcode *Op) // JA/JNBE - Jump if Above / Not Below or Equal
{
    signed char imm8 = (signed char)FetchB(Op);
    if (!(Op->Cpu->eflags & (EFLAGS_CF | EFLAGS_ZF)))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd78(x86opcode *Op) // JS - Jump if Sign
{
    signed char imm8 = (signed char)FetchB(Op);
    if (Op->Cpu->eflags & EFLAGS_SF)
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd79(x86opcode *Op) // JNS - Jump if Not Sign
{
    signed char imm8 = (signed char)FetchB(Op);
    if (!(Op->Cpu->eflags & EFLAGS_SF))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd7A(x86opcode *Op) // JP/JPE - Jump if Parity / Parity Even
{
    signed char imm8 = (signed char)FetchB(Op);
    if (Op->Cpu->eflags & EFLAGS_PF)
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd7B(x86opcode *Op) // JNP/JPO - Jump if Not Parity / Parity Odd
{
    signed char imm8 = (signed char)FetchB(Op);
    if (!(Op->Cpu->eflags & EFLAGS_PF))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd7C(x86opcode *Op) // JL/JNGE - Jump if Less / Not Greater or Equal
{
    signed char imm8 = (signed char)FetchB(Op);
    BOOL sf = (Op->Cpu->eflags & EFLAGS_SF) != 0;
    BOOL of = (Op->Cpu->eflags & EFLAGS_OF) != 0;
    if (sf != of)
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd7D(x86opcode *Op) // JGE/JNL - Jump if Greater or Equal / Not Less
{
    signed char imm8 = (signed char)FetchB(Op);
    BOOL sf = (Op->Cpu->eflags & EFLAGS_SF) != 0;
    BOOL of = (Op->Cpu->eflags & EFLAGS_OF) != 0;
    if (sf == of)
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd7E(x86opcode *Op) // JLE/JNG - Jump if Less or Equal / Not Greater
{
    signed char imm8 = (signed char)FetchB(Op);
    BOOL sf = (Op->Cpu->eflags & EFLAGS_SF) != 0;
    BOOL of = (Op->Cpu->eflags & EFLAGS_OF) != 0;
    BOOL zf = (Op->Cpu->eflags & EFLAGS_ZF) != 0;
    if (zf || (sf != of))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd7F(x86opcode *Op) // JG/JNLE - Jump if Greater / Not Less or Equal
{
    signed char imm8 = (signed char)FetchB(Op);
    BOOL sf = (Op->Cpu->eflags & EFLAGS_SF) != 0;
    BOOL of = (Op->Cpu->eflags & EFLAGS_OF) != 0;
    BOOL zf = (Op->Cpu->eflags & EFLAGS_ZF) != 0;
    if (!zf && (sf == of))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL Cmd80(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd81(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd82(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd83(x86opcode *Op)
{
    return false;
}

// Removed - already defined above at line 88-89

// XCHG r/m, reg (Cmd86/Cmd87) and XCHG EAX, reg (Cmd90-Cmd97)
static bool FASTCALL Cmd86(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); BYTE op1 = is_mem ? *(BYTE*)addr : (BYTE)ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); BYTE op2 = (BYTE)ReadReg(Op->Cpu, reg_idx); if (is_mem) WriteMem32(addr, op2); else WriteReg(Op->Cpu, rm_idx, op2); WriteReg(Op->Cpu, reg_idx, op1); Op->Cpu->eip += 2; return false; }
static bool FASTCALL Cmd87(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem); DWORD op1 = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx); int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); DWORD op2 = ReadReg(Op->Cpu, reg_idx); if (is_mem) WriteMem32(addr, op2); else WriteReg(Op->Cpu, rm_idx, op2); WriteReg(Op->Cpu, reg_idx, op1); Op->Cpu->eip += 2; return false; }

static bool FASTCALL Cmd88(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd89(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd8A(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd8B(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd8C(x86opcode *Op)
{
    return false;
}

// LEA reg, r/m (Cmd8D) - Load Effective Address
static bool FASTCALL Cmd8D(x86opcode *Op) { FetchModRM(Op); DWORD addr; BOOL is_mem; int reg_idx = ResolveOperand(Op, 1, &addr, &is_mem); ResolveOperand(Op, 0, &addr, &is_mem); WriteReg(Op->Cpu, reg_idx, addr); Op->Cpu->eip += 2; return false; }

// MOV r/m16, Sreg (Cmd8C) and MOV Sreg, r/m16 (Cmd8E), POP r/m (Cmd8F)

static bool FASTCALL Cmd8E(x86opcode *Op) // MOV Sreg, r/m16
{
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    WORD val = is_mem ? (WORD)ReadMem32(addr) : (WORD)ReadReg(Op->Cpu, rm_idx);
    int reg = Op->Reg;
    switch(reg) {
        case 0: Op->Cpu->es = val; break;
        case 1: /* CS is not allowed */ break;
        case 2: Op->Cpu->ss = val; break;
        case 3: Op->Cpu->ds = val; break;
        case 4: Op->Cpu->fs = val; break;
        case 5: Op->Cpu->gs = val; break;
    }
    Op->Cpu->eip += 2;
    return false;
}

static bool FASTCALL Cmd8F(x86opcode *Op) // POP r/m
{
    FetchModRM(Op);
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD val = ReadMem32(Op->Cpu->esp);
    Op->Cpu->esp += 4;
    if (is_mem) WriteMem32(addr, val);
    else WriteReg(Op->Cpu, rm_idx, val);
    Op->Cpu->eip += 2;
    return false;
}

// XCHG EAX, reg (Cmd90-Cmd97) - NOP when reg=EAX
static bool FASTCALL Cmd90(x86opcode *Op) { /* XCHG EAX, EAX = NOP */ Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd91(x86opcode *Op) { DWORD t = Op->Cpu->eax; Op->Cpu->eax = Op->Cpu->ecx; Op->Cpu->ecx = t; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd92(x86opcode *Op) { DWORD t = Op->Cpu->eax; Op->Cpu->eax = Op->Cpu->edx; Op->Cpu->edx = t; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd93(x86opcode *Op) { DWORD t = Op->Cpu->eax; Op->Cpu->eax = Op->Cpu->ebx; Op->Cpu->ebx = t; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd94(x86opcode *Op) { DWORD t = Op->Cpu->eax; Op->Cpu->eax = Op->Cpu->esp; Op->Cpu->esp = t; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd95(x86opcode *Op) { DWORD t = Op->Cpu->eax; Op->Cpu->eax = Op->Cpu->ebp; Op->Cpu->ebp = t; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd96(x86opcode *Op) { DWORD t = Op->Cpu->eax; Op->Cpu->eax = Op->Cpu->esi; Op->Cpu->esi = t; Op->Cpu->eip += 1; return false; }
static bool FASTCALL Cmd97(x86opcode *Op) { DWORD t = Op->Cpu->eax; Op->Cpu->eax = Op->Cpu->edi; Op->Cpu->edi = t; Op->Cpu->eip += 1; return false; }

static bool FASTCALL Cmd98(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd99(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd9A(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd9B(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd9C(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd9D(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd9E(x86opcode *Op)
{
    return false;
}

static bool FASTCALL Cmd9F(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA0(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA1(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA2(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA3(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA4(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA5(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA6(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA7(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA8(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdA9(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdAA(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdAB(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdAC(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdAD(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdAE(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdAF(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB0(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB1(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB2(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB3(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB4(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB5(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB6(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB7(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB8(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdB9(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdBA(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdBB(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdBC(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdBD(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdBE(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdBF(x86opcode *Op)
{
    return false;
}

// SHL/SHR/ROL/ROR etc r/m8, imm8 (CmdC0)
static bool FASTCALL CmdC0(x86opcode *Op) {
    FetchModRM(Op);
    BYTE modrm = Op->ModRM;
    BYTE reg = (modrm >> 3) & 0x07;
    BYTE rm = modrm & 0x07;
    BYTE mod = (modrm >> 6) & 0x03;
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    BYTE val = is_mem ? (BYTE)ReadMem32(addr) : (BYTE)ReadReg(Op->Cpu, rm_idx);
    BYTE count = FetchImm8(Op) & 0x1F;
    BYTE res = 0;
    if (count == 0) { Op->Cpu->eip += 3; return false; }
    
    switch (reg) {
        case 0: // SHL
            res = val << (count & 7);
            if (count == 1 && ((val >> 7) & 1)) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            if (count > 0 && ((val >> (8 - count)) & 1)) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 1: // SHR
            res = val >> (count & 7);
            if (count == 1 && (val & 0x80)) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            if (count > 0 && ((val >> (count - 1)) & 1)) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 2: // RCL (simplified)
        case 3: // RCR (simplified)
        case 4: // ROL
            res = (val << (count & 7)) | (val >> (8 - (count & 7)));
            if (count > 0) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 5: // ROR
            res = (val >> (count & 7)) | (val << (8 - (count & 7)));
            if (count > 0) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 6: // SAR
            res = ((signed char)val) >> (count & 7);
            if (count > 0 && ((val >> (count - 1)) & 1)) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 7: // SAL (same as SHL)
            res = val << (count & 7);
            if (count == 1 && ((val >> 7) & 1)) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            if (count > 0 && ((val >> (8 - count)) & 1)) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
    }
    if (res == 0) Op->Cpu->eflags |= EFLAGS_ZF; else Op->Cpu->eflags &= ~EFLAGS_ZF;
    if (res & 0x80) Op->Cpu->eflags |= EFLAGS_SF; else Op->Cpu->eflags &= ~EFLAGS_SF;
    if (CalcParity(res)) Op->Cpu->eflags |= EFLAGS_PF; else Op->Cpu->eflags &= ~EFLAGS_PF;
    if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res);
    Op->Cpu->eip += 3;
    return false;
}

// SHL/SHR/ROL/ROR etc r/m32, imm8 (CmdC1)
static bool FASTCALL CmdC1(x86opcode *Op) {
    FetchModRM(Op);
    BYTE modrm = Op->ModRM;
    BYTE reg = (modrm >> 3) & 0x07;
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD val = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    BYTE count = FetchImm8(Op) & 0x1F;
    DWORD res = 0;
    if (count == 0) { Op->Cpu->eip += 3; return false; }
    
    switch (reg) {
        case 0: // SHL
            res = val << count;
            if (count == 1 && ((val >> 31) & 1)) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            if (count > 0 && ((val >> (32 - count)) & 1)) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 1: // SHR
            res = val >> count;
            if (count == 1 && (val & 0x80000000)) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            if (count > 0 && ((val >> (count - 1)) & 1)) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 4: // ROL
            res = (val << count) | (val >> (32 - count));
            if (count > 0) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 5: // ROR
            res = (val >> count) | (val << (32 - count));
            if (count > 0) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 6: // SAR
            res = ((signed long)val) >> count;
            if (count > 0 && ((val >> (count - 1)) & 1)) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        case 7: // SAL (same as SHL)
            res = val << count;
            if (count == 1 && ((val >> 31) & 1)) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            if (count > 0 && ((val >> (32 - count)) & 1)) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            break;
        default:
            res = val;
            break;
    }
    if (res == 0) Op->Cpu->eflags |= EFLAGS_ZF; else Op->Cpu->eflags &= ~EFLAGS_ZF;
    if (res & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; else Op->Cpu->eflags &= ~EFLAGS_SF;
    if (CalcParity(res)) Op->Cpu->eflags |= EFLAGS_PF; else Op->Cpu->eflags &= ~EFLAGS_PF;
    if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res);
    Op->Cpu->eip += 3;
    return false;
}

static bool FASTCALL CmdC2(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdC3(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdC4(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdC5(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdC6(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdC7(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdC8(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdC9(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdCA(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdCB(x86opcode *Op)
{
    return false;
}

// Int3
static bool FASTCALL CmdCC(x86opcode *Op)
{
    Op->Finished = true;
    return false;
}

// Int X
static bool FASTCALL CmdCD(x86opcode *Op)
{
    Op->Finished = true;
    return false;
}

static bool FASTCALL CmdCE(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdCF(x86opcode *Op)
{
    return false;
}

// D0-D3: Shift/Rotate by 1 or by CL
static bool FASTCALL CmdD0(x86opcode *Op) {
    FetchModRM(Op);
    BYTE modrm = Op->ModRM;
    BYTE reg = (modrm >> 3) & 0x07;
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    BYTE val = is_mem ? (BYTE)ReadMem32(addr) : (BYTE)ReadReg(Op->Cpu, rm_idx);
    BYTE count = 1;
    BYTE res = 0;
    
    switch (reg) {
        case 0: // SHL
            res = val << 1;
            if ((val >> 7) & 1) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            if ((val ^ res) & 0x80) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        case 1: // SHR
            res = val >> 1;
            if (val & 1) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            if (val & 0x80) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        case 4: // ROL
            res = (val << 1) | (val >> 7);
            if (val & 0x80) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            if ((val ^ res) & 0x80) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        case 5: // ROR
            res = (val >> 1) | (val << 7);
            if (val & 1) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            if ((val >> 7) ^ (val >> 6)) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        case 6: // SAR
            res = ((signed char)val) >> 1;
            if (val & 1) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        default:
            res = val;
            break;
    }
    if (res == 0) Op->Cpu->eflags |= EFLAGS_ZF; else Op->Cpu->eflags &= ~EFLAGS_ZF;
    if (res & 0x80) Op->Cpu->eflags |= EFLAGS_SF; else Op->Cpu->eflags &= ~EFLAGS_SF;
    if (CalcParity(res)) Op->Cpu->eflags |= EFLAGS_PF; else Op->Cpu->eflags &= ~EFLAGS_PF;
    if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res);
    Op->Cpu->eip += 2;
    return false;
}

static bool FASTCALL CmdD1(x86opcode *Op) {
    FetchModRM(Op);
    BYTE modrm = Op->ModRM;
    BYTE reg = (modrm >> 3) & 0x07;
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD val = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    BYTE count = 1;
    DWORD res = 0;
    
    switch (reg) {
        case 0: // SHL
            res = val << 1;
            if ((val >> 31) & 1) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            if ((val ^ res) & 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        case 1: // SHR
            res = val >> 1;
            if (val & 1) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            if (val & 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        case 4: // ROL
            res = (val << 1) | (val >> 31);
            if ((val >> 31) & 1) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            if ((val ^ res) & 0x80000000) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        case 5: // ROR
            res = (val >> 1) | (val << 31);
            if (val & 1) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            if ((val >> 31) ^ ((val >> 30) & 1)) Op->Cpu->eflags |= EFLAGS_OF; else Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        case 6: // SAR
            res = ((signed long)val) >> 1;
            if (val & 1) Op->Cpu->eflags |= EFLAGS_CF; else Op->Cpu->eflags &= ~EFLAGS_CF;
            Op->Cpu->eflags &= ~EFLAGS_OF;
            break;
        default:
            res = val;
            break;
    }
    if (res == 0) Op->Cpu->eflags |= EFLAGS_ZF; else Op->Cpu->eflags &= ~EFLAGS_ZF;
    if (res & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; else Op->Cpu->eflags &= ~EFLAGS_SF;
    if (CalcParity(res)) Op->Cpu->eflags |= EFLAGS_PF; else Op->Cpu->eflags &= ~EFLAGS_PF;
    if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res);
    Op->Cpu->eip += 2;
    return false;
}

static bool FASTCALL CmdD2(x86opcode *Op) {
    FetchModRM(Op);
    BYTE modrm = Op->ModRM;
    BYTE reg = (modrm >> 3) & 0x07;
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    BYTE val = is_mem ? (BYTE)ReadMem32(addr) : (BYTE)ReadReg(Op->Cpu, rm_idx);
    BYTE count = Op->Cpu->ecx & 0x1F;
    BYTE res = 0;
    if (count == 0) { Op->Cpu->eip += 2; return false; }
    
    switch (reg) {
        case 0: res = val << count; break;
        case 1: res = val >> count; break;
        case 4: res = (val << count) | (val >> (8 - count)); break;
        case 5: res = (val >> count) | (val << (8 - count)); break;
        case 6: res = ((signed char)val) >> count; break;
        default: res = val; break;
    }
    if (res == 0) Op->Cpu->eflags |= EFLAGS_ZF; else Op->Cpu->eflags &= ~EFLAGS_ZF;
    if (res & 0x80) Op->Cpu->eflags |= EFLAGS_SF; else Op->Cpu->eflags &= ~EFLAGS_SF;
    if (CalcParity(res)) Op->Cpu->eflags |= EFLAGS_PF; else Op->Cpu->eflags &= ~EFLAGS_PF;
    if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res);
    Op->Cpu->eip += 2;
    return false;
}

static bool FASTCALL CmdD3(x86opcode *Op) {
    FetchModRM(Op);
    BYTE modrm = Op->ModRM;
    BYTE reg = (modrm >> 3) & 0x07;
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD val = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    BYTE count = Op->Cpu->ecx & 0x1F;
    DWORD res = 0;
    if (count == 0) { Op->Cpu->eip += 2; return false; }
    
    switch (reg) {
        case 0: res = val << count; break;
        case 1: res = val >> count; break;
        case 4: res = (val << count) | (val >> (32 - count)); break;
        case 5: res = (val >> count) | (val << (32 - count)); break;
        case 6: res = ((signed long)val) >> count; break;
        default: res = val; break;
    }
    if (res == 0) Op->Cpu->eflags |= EFLAGS_ZF; else Op->Cpu->eflags &= ~EFLAGS_ZF;
    if (res & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF; else Op->Cpu->eflags &= ~EFLAGS_SF;
    if (CalcParity(res)) Op->Cpu->eflags |= EFLAGS_PF; else Op->Cpu->eflags &= ~EFLAGS_PF;
    if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res);
    Op->Cpu->eip += 2;
    return false;
}

static bool FASTCALL CmdD4(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdD5(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdD6(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdD7(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdD8(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdD9(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdDA(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdDB(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdDC(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdDD(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdDE(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdDF(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdE0(x86opcode *Op) // LOOPNE/LOOPNZ - Loop While Not Equal/Zero
{
    signed char imm8 = (signed char)FetchB(Op);
    Op->Cpu->ecx--;
    if (Op->Cpu->ecx != 0 && !(Op->Cpu->eflags & EFLAGS_ZF))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL CmdE1(x86opcode *Op) // LOOPE/LOOPZ - Loop While Equal/Zero
{
    signed char imm8 = (signed char)FetchB(Op);
    Op->Cpu->ecx--;
    if (Op->Cpu->ecx != 0 && (Op->Cpu->eflags & EFLAGS_ZF))
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL CmdE2(x86opcode *Op) // LOOP - Loop While ECX != 0
{
    signed char imm8 = (signed char)FetchB(Op);
    Op->Cpu->ecx--;
    if (Op->Cpu->ecx != 0)
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL CmdE3(x86opcode *Op) // JCXZ/JECXZ - Jump if ECX is Zero
{
    signed char imm8 = (signed char)FetchB(Op);
    if (Op->Cpu->ecx == 0)
        Op->Cpu->eip += (int)imm8;
    Op->Cpu->eip += 1;
    return false;
}

static bool FASTCALL CmdE4(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdE5(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdE6(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdE7(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdE8(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdE9(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdEA(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdEB(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdEC(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdED(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdEE(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdEF(x86opcode *Op)
{
    return false;
}

// Lock
static bool FASTCALL CmdF0(x86opcode *Op)
{
    Op->Lock = true;
    return true;
}

// ICEBP
static bool FASTCALL CmdF1(x86opcode *Op)
{
    Op->Finished = true;
    return false;
}

// RepNE
static bool FASTCALL CmdF2(x86opcode *Op)
{
    Op->Rep = repne;
    return true;
}

// RepE
static bool FASTCALL CmdF3(x86opcode *Op)
{
    Op->Rep = repe;
    return true;
}

// Hlt
static bool FASTCALL CmdF4(x86opcode *Op)
{
    Op->Finished = true;
    return false;
}

static bool FASTCALL CmdF5(x86opcode *Op)
{
    return false;
}

// F6/F7: TEST, NOT, NEG, MUL, IMUL, DIV, IDIV (group 3)
static bool FASTCALL CmdF6(x86opcode *Op) {
    FetchModRM(Op);
    BYTE modrm = Op->ModRM;
    BYTE reg = (modrm >> 3) & 0x07;
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    BYTE val = is_mem ? (BYTE)ReadMem32(addr) : (BYTE)ReadReg(Op->Cpu, rm_idx);
    
    switch (reg) {
        case 0: // TEST r/m8, imm8
        case 1: // TEST r/m8, imm8 (unused)
            {
                BYTE imm = FetchImm8(Op);
                BYTE res = val & imm;
                UpdateFlags_Logical(Op->Cpu, res);
                Op->Cpu->eip += 3;
            }
            break;
        case 2: // NOT r/m8
            val = ~val;
            if (is_mem) WriteMem32(addr, val); else WriteReg(Op->Cpu, rm_idx, val);
            Op->Cpu->eip += 2;
            break;
        case 3: // NEG r/m8
            {
                BYTE res = -val;
                Op->Cpu->eflags &= ~(EFLAGS_CF | EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF);
                if (res == 0) Op->Cpu->eflags |= EFLAGS_ZF;
                if (res & 0x80) Op->Cpu->eflags |= EFLAGS_SF;
                if (CalcParity(res)) Op->Cpu->eflags |= EFLAGS_PF;
                if (val != 0) Op->Cpu->eflags |= EFLAGS_CF;
                if ((val ^ res) & 0x80) Op->Cpu->eflags |= EFLAGS_OF;
                if ((val & 0xF) + (res & 0xF) > 0xF) Op->Cpu->eflags |= EFLAGS_AF;
                if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res);
            }
            Op->Cpu->eip += 2;
            break;
        case 4: // MUL AL, r/m8
            {
                WORD res = (WORD)Op->Cpu->eax & 0xFF * val;
                Op->Cpu->eax = (Op->Cpu->eax & ~0xFFFF) | res;
                Op->Cpu->eflags &= ~(EFLAGS_CF | EFLAGS_OF);
                if (res > 0xFF) { Op->Cpu->eflags |= EFLAGS_CF; Op->Cpu->eflags |= EFLAGS_OF; }
            }
            Op->Cpu->eip += 2;
            break;
        case 5: // IMUL AL, r/m8 (simplified)
        case 6: // DIV AX, r/m8
        case 7: // IDIV AX, r/m8
        default:
            Op->Cpu->eip += 2;
            break;
    }
    return false;
}

static bool FASTCALL CmdF7(x86opcode *Op) {
    FetchModRM(Op);
    BYTE modrm = Op->ModRM;
    BYTE reg = (modrm >> 3) & 0x07;
    DWORD addr; BOOL is_mem;
    int rm_idx = ResolveOperand(Op, 0, &addr, &is_mem);
    DWORD val = is_mem ? ReadMem32(addr) : ReadReg(Op->Cpu, rm_idx);
    
    switch (reg) {
        case 0: // TEST r/m32, imm32
        case 1: // TEST r/m32, imm32 (unused)
            {
                DWORD imm = FetchImm32(Op);
                DWORD res = val & imm;
                UpdateFlags_Logical(Op->Cpu, res);
                Op->Cpu->eip += 6;
            }
            break;
        case 2: // NOT r/m32
            val = ~val;
            if (is_mem) WriteMem32(addr, val); else WriteReg(Op->Cpu, rm_idx, val);
            Op->Cpu->eip += 2;
            break;
        case 3: // NEG r/m32
            {
                DWORD res = -val;
                Op->Cpu->eflags &= ~(EFLAGS_CF | EFLAGS_OF | EFLAGS_SF | EFLAGS_ZF | EFLAGS_AF | EFLAGS_PF);
                if (res == 0) Op->Cpu->eflags |= EFLAGS_ZF;
                if (res & 0x80000000) Op->Cpu->eflags |= EFLAGS_SF;
                if (CalcParity(res)) Op->Cpu->eflags |= EFLAGS_PF;
                if (val != 0) Op->Cpu->eflags |= EFLAGS_CF;
                if ((val ^ res) & 0x80000000) Op->Cpu->eflags |= EFLAGS_OF;
                if ((val & 0xF) + (res & 0xF) > 0xF) Op->Cpu->eflags |= EFLAGS_AF;
                if (is_mem) WriteMem32(addr, res); else WriteReg(Op->Cpu, rm_idx, res);
            }
            Op->Cpu->eip += 2;
            break;
        case 4: // MUL EAX, r/m32
            {
                DWORD op = Op->Cpu->eax;
                DWORD lo = op * val;
                DWORD hi = (DWORD)(((unsigned long long)op * val) >> 32);
                Op->Cpu->eax = lo;
                Op->Cpu->edx = hi;
                Op->Cpu->eflags &= ~(EFLAGS_CF | EFLAGS_OF);
                if (hi != 0) { Op->Cpu->eflags |= EFLAGS_CF; Op->Cpu->eflags |= EFLAGS_OF; }
            }
            Op->Cpu->eip += 2;
            break;
        case 5: // IMUL EAX, r/m32 (simplified)
        case 6: // DIV EDX:EAX, r/m32
        case 7: // IDIV EDX:EAX, r/m32
        default:
            Op->Cpu->eip += 2;
            break;
    }
    return false;
}

static bool FASTCALL CmdF8(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdF9(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdFA(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdFB(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdFC(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdFD(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdFE(x86opcode *Op)
{
    return false;
}

static bool FASTCALL CmdFF(x86opcode *Op)
{
    return false;
}

COMMAND *OneByteCmds[256] =
{
    Cmd00, Cmd01, Cmd02, Cmd03, Cmd04, Cmd05, Cmd06, Cmd07, Cmd08, Cmd09, Cmd0A, Cmd0B, Cmd0C, Cmd0D, Cmd0E, Cmd0F,
    Cmd10, Cmd11, Cmd12, Cmd13, Cmd14, Cmd15, Cmd16, Cmd17, Cmd18, Cmd19, Cmd1A, Cmd1B, Cmd1C, Cmd1D, Cmd1E, Cmd1F,
    Cmd20, Cmd21, Cmd22, Cmd23, Cmd24, Cmd25, Cmd26, Cmd27, Cmd28, Cmd29, Cmd2A, Cmd2B, Cmd2C, Cmd2D, Cmd2E, Cmd2F,
    Cmd30, Cmd31, Cmd32, Cmd33, Cmd34, Cmd35, Cmd36, Cmd37, Cmd38, Cmd39, Cmd3A, Cmd3B, Cmd3C, Cmd3D, Cmd3E, Cmd3F,
    Cmd40, Cmd41, Cmd42, Cmd43, Cmd44, Cmd45, Cmd46, Cmd47, Cmd48, Cmd49, Cmd4A, Cmd4B, Cmd4C, Cmd4D, Cmd4E, Cmd4F,
    Cmd50, Cmd51, Cmd52, Cmd53, Cmd54, Cmd55, Cmd56, Cmd57, Cmd58, Cmd59, Cmd5A, Cmd5B, Cmd5C, Cmd5D, Cmd5E, Cmd5F,
    Cmd60, Cmd61, Cmd62, Cmd63, Cmd64, Cmd65, Cmd66, Cmd67, Cmd68, Cmd69, Cmd6A, Cmd6B, Cmd6C, Cmd6D, Cmd6E, Cmd6F,
    Cmd70, Cmd71, Cmd72, Cmd73, Cmd74, Cmd75, Cmd76, Cmd77, Cmd78, Cmd79, Cmd7A, Cmd7B, Cmd7C, Cmd7D, Cmd7E, Cmd7F,
    Cmd80, Cmd81, Cmd82, Cmd83, Cmd84, Cmd85, Cmd86, Cmd87, Cmd88, Cmd89, Cmd8A, Cmd8B, Cmd8C, Cmd8D, Cmd8E, Cmd8F,
    Cmd90, Cmd91, Cmd92, Cmd93, Cmd94, Cmd95, Cmd96, Cmd97, Cmd98, Cmd99, Cmd9A, Cmd9B, Cmd9C, Cmd9D, Cmd9E, Cmd9F,
    CmdA0, CmdA1, CmdA2, CmdA3, CmdA4, CmdA5, CmdA6, CmdA7, CmdA8, CmdA9, CmdAA, CmdAB, CmdAC, CmdAD, CmdAE, CmdAF,
    CmdB0, CmdB1, CmdB2, CmdB3, CmdB4, CmdB5, CmdB6, CmdB7, CmdB8, CmdB9, CmdBA, CmdBB, CmdBC, CmdBD, CmdBE, CmdBF,
    CmdC0, CmdC1, CmdC2, CmdC3, CmdC4, CmdC5, CmdC6, CmdC7, CmdC8, CmdC9, CmdCA, CmdCB, CmdCC, CmdCD, CmdCE, CmdCF,
    CmdD0, CmdD1, CmdD2, CmdD3, CmdD4, CmdD5, CmdD6, CmdD7, CmdD8, CmdD9, CmdDA, CmdDB, CmdDC, CmdDD, CmdDE, CmdDF,
    CmdE0, CmdE1, CmdE2, CmdE3, CmdE4, CmdE5, CmdE6, CmdE7, CmdE8, CmdE9, CmdEA, CmdEB, CmdEC, CmdED, CmdEE, CmdEF,
    CmdF0, CmdF1, CmdF2, CmdF3, CmdF4, CmdF5, CmdF6, CmdF7, CmdF8, CmdF9, CmdFA, CmdFB, CmdFC, CmdFD, CmdFE, CmdFF,
};
