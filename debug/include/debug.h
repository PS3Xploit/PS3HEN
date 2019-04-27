/*  debug.h - printf message logging via Lv-1 Ethernet

Copyright (C) 2010  Hector Martin "marcan" <hector@marcansoft.com

This code is licensed to you under the terms of the GNU GPL, version 2;
see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef DEBUG_H
#define DEBUG_H

int64_t debug_init(void);
int 	debug_end(void);
void 	debug_install(void);
void	debug_uninstall(void);
int64_t debug_printf(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
int64_t _debug_printf(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
int64_t debug_print(const char* buffer, size_t size);
void debug_print_hex(void *buf, uint64_t size);
void debug_print_hex_c(void *buf, uint64_t size);

void fatal(const char *msg) __attribute__((noreturn));
void abort(void) __attribute__((noreturn));

#endif
