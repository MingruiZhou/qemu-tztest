/*
 * Copyright (c) 2015 Linaro Limited
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _PLATFORM_H
#define _PLATFORM_H

/* NORFLASH0 on Vexpress aseries (a15) is mapped from 0x08000000 to 0x0A000000
 * which is 32MB.  It is also aliased to 0x0 (to 0x2000000).
 * It is acceptable to have the text here as it is RO.
 */
#define FLASH_BASE 0x00000000
#define FLASH_SIZE (64 * 1024 * 1024)

#define RAM_BASE 0x60000000
#define RAM_SIZE (1*1024*1024*1024)

#define UART0_BASE 0x10009000

/* Taken from Linux vexpress support */
#define SYSREG_BASE 0x10000000
#define SYSREG_CFGCTRL 0xa4
#define SYS_CFGCTRL_FUNC(n) (((n) & 0x3f) << 20)
#define SYS_CFGFUNC_RESET 5
#define SYS_CFGFUNC_SHUTDOWN 8
#define SYS_CFGCTRL_START       (1 << 31)
#define SYS_CFGCTRL_WRITE       (1 << 30)
#define SYS_SHUTDOWN    (SYS_CFGCTRL_FUNC(SYS_CFGFUNC_SHUTDOWN) | \
                         SYS_CFGCTRL_START | SYS_CFGCTRL_WRITE)
#define SYS_RESET       (SYS_CFGCTRL_FUNC(SYS_CFGFUNC_SHUTDOWN) | \
                         SYS_CFGCTRL_START | SYS_CFGCTRL_WRITE)

#endif
