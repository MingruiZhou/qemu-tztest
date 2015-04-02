#ifndef _MEMORY_H
#define _MEMORY_H

#include "platform.h"

#define VA_SIZE 32
#define TnSZ (32-VA_SIZE)
#define STACK_SIZE 0x40000

#define EL3_FLASH_BASE FLASH_BASE
#define EL3_RAM_BASE RAM_BASE
#define EL3_RAM_SIZE (512*1024)
#define EL3_BASE_VA 0x70000000
#define EL3_PGTBL_SIZE 0x40000
#define EL3_PGTBL_BASE EL3_RAM_BASE
#define EL3_STACK_SIZE STACK_SIZE
#define EL3_STACK_BASE 0x7FFF0000
#define EL3_PA_POOL_BASE (EL3_PGTBL_BASE + EL3_PGTBL_SIZE)
#define EL3_PTE_POOL_BASE (EL3_PGTBL_BASE + 0x4000)

#define EL1_S_FLASH_BASE (FLASH_BASE+0x10000)
#define EL1_S_RAM_BASE (RAM_BASE+0xC000000)
#define EL1_S_RAM_SIZE (512*1024)
#define EL1_S_BASE_VA 0x60000000
#define EL1_S_PGTBL_SIZE 0x40000
#define EL1_S_PGTBL_BASE EL1_S_RAM_BASE+EL1_S_RAM_SIZE-EL1_S_PGTBL_SIZE
#define EL1_S_STACK_SIZE STACK_SIZE
#define EL1_S_STACK_BASE 0x6FFF0000

#define EL1_NS_FLASH_BASE (FLASH_BASE+0x20000)
#define EL1_NS_RAM_BASE (RAM_BASE+0x8000000)
#define EL1_NS_RAM_SIZE (512*1024)
#define EL1_NS_BASE_VA 0x80000000
#define EL1_NS_PGTBL_SIZE 0x40000
#define EL1_NS_PGTBL_BASE EL1_NS_RAM_BASE+EL1_NS_RAM_SIZE-EL1_NS_PGTBL_SIZE
#define EL1_NS_STACK_SIZE STACK_SIZE
#define EL1_NS_STACK_BASE 0xFFFFF000

#define EL0_S_FLASH_BASE (FLASH_BASE+0x50000)
#define EL0_S_RAM_BASE (RAM_BASE+0x4000000)
#define EL0_S_RAM_SIZE (512*1024)
#define EL0_S_BASE_VA 0x000C0000
#define EL0_S_STACK_SIZE STACK_SIZE
#define EL0_S_STACK_BASE 0x80000000-0x1000

#define EL0_NS_FLASH_BASE (FLASH_BASE+0x30000)
#define EL0_NS_RAM_BASE (RAM_BASE+0x4000000)
#define EL0_NS_RAM_SIZE (512*1024)
#define EL0_NS_BASE_VA 0x00080000
#define EL0_NS_STACK_SIZE STACK_SIZE
#define EL0_NS_STACK_BASE 0x70000000-0x1000

#define TZTEST_COMMON_HEAP_BASE 0x49000000
#define TZTEST_SHARED_HEAP_BASE 0x4A000000
#endif
