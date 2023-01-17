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
#include "serial.h"

uint16_t chv[14];

void initserial(void) {
	UART_BRR2 = 0x05;
	UART_BRR1 = 0x04; // 115200 baud @ 8Mhz clock
	UART_CR2 = 0x2c; // REN=1, TEN=1, RIEN=1 (enable RX/TX/interrupts)
	NESTED_IRQ(UART_RXIRQ); // Enable nested IRQ
}

int putchar(int c) { // STDOUT -> UART_TX (blocking)
	while (!(UART_SR & 0x80)); // TXE=0 (TX in progress)
	UART_DR = c;
	return 0;
}

void UART_RXNE(void) __interrupt(UART_RXIRQ) {
	static uint8_t a, b, n = 30;
	static uint16_t u;
	a = b;
	b = UART_DR; // Clear RXNE
	if (a == 0x20 && b == 0x40) { // Sync
		n = 0;
		u = 0xff9f;
		return;
	}
	if (n == 30 || ++n & 1) return;
	uint16_t v = a | b << 8;
	if (n == 30) { // End of chunk
		if (u != v) return; // Sync lost
		update();
		return;
	}
	chv[(n >> 1) - 1] = v & 0x0fff;
	u -= a + b;
}
