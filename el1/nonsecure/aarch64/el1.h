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

#ifndef _EL1_NS_H
#define _EL1_NS_H

#include "memory.h"

#ifndef __ASSEMBLY__
extern uintptr_t _EL1_NS_INIT_BASE;
extern uintptr_t EL1_NS_INIT_BASE;
extern uintptr_t _EL1_NS_INIT_SIZE;
extern uintptr_t EL1_NS_INIT_SIZE;
extern uintptr_t _EL1_NS_FLASH_TEXT;
extern uintptr_t EL1_NS_FLASH_TEXT;
extern uintptr_t _EL1_NS_TEXT_BASE;
extern uintptr_t EL1_NS_TEXT_BASE;
extern uintptr_t _EL1_NS_DATA_BASE;
extern uintptr_t EL1_NS_DATA_BASE;
extern uintptr_t _EL1_NS_TEXT_SIZE;
extern uintptr_t EL1_NS_TEXT_SIZE;
extern uintptr_t _EL1_NS_DATA_SIZE;
extern uintptr_t EL1_NS_DATA_SIZE;
#endif

#define _EL1_INIT_BASE _EL1_NS_INIT_BASE
#define _EL1_INIT_SIZE _EL1_NS_INIT_SIZE
#define _EL1_FLASH_TEXT _EL1_NS_FLASH_TEXT
#define _EL1_TEXT_BASE _EL1_NS_TEXT_BASE
#define _EL1_TEXT_SIZE _EL1_NS_TEXT_SIZE
#define _EL1_FLASH_DATA _EL1_NS_FLASH_DATA
#define _EL1_DATA_BASE _EL1_NS_DATA_BASE
#define _EL1_DATA_SIZE _EL1_NS_DATA_SIZE
#define EL1_STACK_BASE EL1_NS_STACK_BASE
#define EL1_PGTBL_BASE EL1_NS_PGTBL_BASE
#define EL1_PGTBL_SIZE EL1_NS_PGTBL_SIZE
#define EL1_INIT_STACK EL1_NS_INIT_STACK
#define EL1_PA_POOL_BASE EL1_NS_PA_POOL_BASE
#define EL1_PTE_POOL_BASE EL1_NS_PTE_POOL_BASE
#define EL1_BASE_VA EL1_NS_BASE_VA
#define EL1_VA_HEAP_BASE EL1_NS_VA_HEAP_BASE

#define EL0_STACK_BASE EL0_NS_STACK_BASE

#endif
