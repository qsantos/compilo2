/*\
 *  Just Another Compiler Collection
 *  Copyright (C) 2013 Quentin SANTOS
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
\*/

#ifndef IR_H
#define IR_H

#include <sys/types.h>

typedef unsigned int      ir_aval_t;
typedef unsigned int      ir_label_t;
typedef struct ir_arg_t   ir_arg_t;
typedef struct ir_instr_t ir_instr_t;
typedef struct ir_prgm_t  ir_prgm_t;

typedef enum
{
	O_IMM, O_IMMADDR,
	O_REG, O_REGADDR,
} ir_atype_t;

struct ir_arg_t
{
	ir_atype_t t;
	ir_aval_t  v;
};

typedef enum
{
	I_NOP, I_HLT,
	I_MOV,
	I_NEG, I_NOT,
	I_LOR, I_AND, I_XOR,
	I_ADD, I_SUB, I_MUL, I_DIV, I_MOD,
	I_LBL, I_JMP,
	I_JEQ, I_JNE, I_JGE, I_JGT, I_JLE, I_JLT,
	I_CAL, I_RET,
} ir_opcode_t;

struct ir_instr_t
{
	ir_opcode_t type;
	ir_arg_t op0;
	ir_arg_t op1;
	ir_arg_t op2;
};

struct ir_prgm_t
{
	ir_aval_t    n_regs;
	ir_label_t   n_labels;

	size_t       n_instr;
	size_t       a_instr;
	ir_instr_t* instrs;
};

// constructor, destructor

void ir_init(ir_prgm_t* p);
void ir_del (ir_prgm_t* p);



// registers

// reset registers and allocate 'n' fresh ones
void ir_resreg(ir_prgm_t* p, size_t n);
// get fresh register
ir_aval_t ir_reg(ir_prgm_t* p);



// labels

ir_label_t ir_label(ir_prgm_t* p);



// add new instructions
// ir_pushX() expects X operands

void ir_push0(ir_prgm_t* p, ir_opcode_t o);

void ir_push1(ir_prgm_t* p, ir_opcode_t o,
	ir_atype_t t1, ir_aval_t x1
);

void ir_push2(ir_prgm_t* p, ir_opcode_t o,
	ir_atype_t t1, ir_aval_t x1,
	ir_atype_t t2, ir_aval_t x2
);

void ir_push3(ir_prgm_t* p, ir_opcode_t o,
	ir_atype_t t1, ir_aval_t x1,
	ir_atype_t t2, ir_aval_t x2,
	ir_atype_t t3, ir_aval_t x3
);

#endif
