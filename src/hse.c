/*
** Copyright (C) 2022-2023 Arseny Vakhrushev <arseny.vakhrushev@me.com>
**
** This firmware is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This firmware is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this firmware. If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"

void inithse(void) {
	CLK_SWCR = 0x02; // Enable switch
	CLK_SWR = 0xb4; // Enable HSE clock
	while (!(CLK_SWCR & 0x08)); // SWIF=0 (switch in progress)
	CLK_ICKR = 0x00; // Disable HSI clock
	CLK_SWCR = 0x00; // Disable switch
}
