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

#ifndef PATHS_H
#define PATHS_H

int Paths_Absolute(const char path[]);

char *Paths_Dirname(const char path[]);

char *Paths_Basename(const char path[]);

char *Paths_SansSuffix(const char path[]);

const char *Paths_ShellArg(const char arg[]);

char *Paths_CurrentDir(void);

#endif
