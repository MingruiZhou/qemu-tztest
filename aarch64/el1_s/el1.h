#ifndef _EL1_S_H
#define _EL1_S_H

#include "platform.h"

#define _EL1_INIT_BASE _EL1_S_INIT_BASE
#define _EL1_INIT_SIZE _EL1_S_INIT_SIZE
#define _EL1_FLASH_TEXT _EL1_S_FLASH_TEXT
#define _EL1_TEXT_BASE _EL1_S_TEXT_BASE
#define _EL1_TEXT_SIZE _EL1_S_TEXT_SIZE
#define _EL1_FLASH_DATA _EL1_S_FLASH_DATA
#define _EL1_DATA_BASE _EL1_S_DATA_BASE
#define _EL1_DATA_SIZE _EL1_S_DATA_SIZE
#define EL1_STACK_BASE EL1_S_STACK_BASE
#define EL1_PGTBL_BASE EL1_S_PGTBL_BASE
#define EL1_PGTBL_SIZE EL1_S_PGTBL_SIZE

#define SECURE_STATE "secure"
#endif
