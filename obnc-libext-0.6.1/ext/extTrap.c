/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/extTrap.h"
#include <obnc/OBNC.h>

extTrap__Handler_ extTrap__handle_;

void extTrap__SetHandler_(extTrap__Handler_ h)
{
	OBNC_C_ASSERT(h != NULL);

	OBNC_handleTrap = h;
	extTrap__handle_ = h;
}


void extTrap__Init(void)
{
	extTrap__handle_ = OBNC_handleTrap;
}
