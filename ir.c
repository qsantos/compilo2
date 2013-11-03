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

#include "ir.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

void ir_init(ir_prgm_t* p)
{
	p->n_regs  = 0;
	p->n_instr = 0;
	p->a_instr = 0;
	p->instrs = NULL;
}

void ir_del(ir_prgm_t* p)
{
	free(p->instrs);
}

void ir_resreg(ir_prgm_t* p, size_t n)
{
	p->n_regs = n;
}

ir_aval_t ir_reg(ir_prgm_t* p)
{
	return p->n_regs++;
}

void ir_push0(ir_prgm_t* p, ir_opcode_t o)
{
	ir_push3(p, o, 0, 0, 0, 0, 0, 0);
}

void ir_push1(ir_prgm_t* p, ir_opcode_t o,
	ir_atype_t t1, ir_aval_t x1
)
{
	ir_push3(p, o, t1, x1, 0, 0, 0, 0);
}

void ir_push2(ir_prgm_t* p, ir_opcode_t o,
	ir_atype_t t1, ir_aval_t x1,
	ir_atype_t t2, ir_aval_t x2
)
{
	ir_push3(p, o, t1, x1, t2, x2, 0, 0);
}

void ir_push3(ir_prgm_t* p, ir_opcode_t o,
	ir_atype_t t1, ir_aval_t x1,
	ir_atype_t t2, ir_aval_t x2,
	ir_atype_t t3, ir_aval_t x3
)
{
	if (p->n_instr == p->a_instr)
	{
		p->a_instr = p->a_instr ? 2*p->a_instr : 1;
		p->instrs = MREALLOC(p->instrs, ir_instr_t, p->a_instr);
	}

	ir_instr_t i =
	{
		o,
		{ t1, x1 },
		{ t2, x2 },
		{ t3, x3 },
	};

	memcpy(&p->instrs[p->n_instr++], &i, sizeof(ir_instr_t));
}
