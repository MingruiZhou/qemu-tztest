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
#define EL1_ABT_STACK_BASE EL1_NS_ABT_STACK_BASE
#define EL1_IRQ_STACK_BASE EL1_NS_IRQ_STACK_BASE
#define EL1_UND_STACK_BASE EL1_NS_UND_STACK_BASE
#define EL1_SVC_STACK_BASE EL1_NS_SVC_STACK_BASE
#define EL1_PGTBL_BASE EL1_NS_PGTBL_BASE
#define EL1_PGTBL_SIZE EL1_NS_PGTBL_SIZE
#define EL1_INIT_STACK EL1_NS_INIT_STACK
#define EL1_PA_POOL_BASE EL1_NS_PA_POOL_BASE
#define EL1_PTE_POOL_BASE EL1_NS_PTE_POOL_BASE
#define EL1_BASE_VA EL1_NS_BASE_VA
#define EL1_VA_HEAP_BASE EL1_NS_VA_HEAP_BASE

#define SEC_STATE_STR "non-secure"
#define SEC_STATE 1

#endif
