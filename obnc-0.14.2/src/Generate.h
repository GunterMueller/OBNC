/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This file is part of OBNC.

OBNC is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OBNC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OBNC.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef GENERATE_H
#define GENERATE_H

#include "Trees.h"

void Generate_Init(void);

void Generate_Open(const char moduleName[], int isEntryPoint);

void Generate_ModuleHeading(void);

void Generate_ImportList(Trees_Node importList);

void Generate_ConstDeclaration(Trees_Node constIdent);

void Generate_TypeDeclaration(Trees_Node typeIdent);

void Generate_VariableDeclaration(Trees_Node varIdentList);

void Generate_ProcedureHeading(Trees_Node procIdent);

void Generate_ProcedureStatements(Trees_Node stmtSeq);

void Generate_ReturnClause(Trees_Node exp);

void Generate_ProcedureEnd(Trees_Node procIdent);

void Generate_ModuleStatements(Trees_Node stmtSeq);

void Generate_ModuleEnd(void);

void Generate_Close(void);

#endif
