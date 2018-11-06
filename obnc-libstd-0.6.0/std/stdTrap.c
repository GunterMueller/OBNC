/*Copyright (C) 2017, 2018 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/stdTrap.h"
#include <obnc/OBNC.h>

stdTrap__Handler_ stdTrap__handle_;

void stdTrap__SetHandler_(stdTrap__Handler_ h)
{
	OBNC_C_ASSERT(h != NULL);

	OBNC_handleTrap = h;
	stdTrap__handle_ = h;
}


void stdTrap__Init(void)
{
	stdTrap__handle_ = OBNC_handleTrap;
}
