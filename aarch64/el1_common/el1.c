#include "platform.h"
#include "common_svc.h"
#include "common_defs.h"
#include "el1_loader.h"
#include "string.h"
#include "el1.h"

typedef union {
    struct {
        uint32_t dfsc : 6;
        uint32_t wnr : 1;
        uint32_t s1ptw : 1;
        uint32_t cm : 1;
        uint32_t ea : 1;
        uint32_t fnv  : 1;
        uint32_t res0 : 3;
        uint32_t ar : 1;
        uint32_t sf : 1;
        uint32_t srt : 5;
        uint32_t sse: 1;
        uint32_t sas: 2;
        uint32_t isv: 1;
    };
    uint32_t raw;
} armv8_data_abort_iss_t;

typedef union {
    struct {
        uint32_t ifsc : 6;
        uint32_t res0_0 : 1;
        uint32_t s1ptw : 1;
        uint32_t res0_1 : 1;
        uint32_t ea : 1;
        uint32_t fnv  : 1;
        uint32_t res0_2 : 3;
    };
    uint32_t raw;
} armv8_inst_abort_iss_t;

typedef union {
    struct {
        uint64_t type : 2;
        uint64_t attridx : 3;
        uint64_t ns : 1;
        uint64_t ap2_1 : 2;
        uint64_t sh1_0 : 2;
        uint64_t af : 1;
        uint64_t ng : 1;
        uint64_t pa : 36;
        uint64_t res0 : 4;
        uint64_t contig : 1;
        uint64_t pxn : 1;
        uint64_t xn : 1;
    };
    uint64_t raw;
} armv8_4k_pg_pte_t;

typedef union {
    struct {
        uint64_t type : 2;
        uint64_t ignored0 : 10;
        uint64_t pa : 36;
        uint64_t res0 : 4;
        uint64_t ignored1 : 7;
        uint64_t pxn : 1;
        uint64_t xn : 1;
        uint64_t ap : 2;
        uint64_t ns : 1;
    };
    uint64_t raw;
} armv8_4k_tbl_pte_t;

uint64_t el1_next_pa = 0;
uint64_t el1_allocate_pa() {
    uint64_t next = el1_next_pa;
    el1_next_pa += 0x1000;
    return next;
}

void el1_map_va(uintptr_t addr)
{
    uint64_t pa = EL1_S_PGTBL_BASE;
    uint32_t i;
    armv8_4k_tbl_pte_t *pte;
    armv8_4k_pg_pte_t *l3pte;

    for (i = 0; i < 4; i++) {
        /* Each successive level uses the next lower 9 VA bits in a 48-bit
         * address, hence the i*9.
         */
        uint64_t off = ((addr >> (39-(i*9))) & 0x1FF) << 3;
        pte = (armv8_4k_tbl_pte_t *)(pa | off);
        if (!pte->type) {
            pa = el1_allocate_pa();
            pte->pa = pa >> 12;
            pte->type = 3;
        } else {
            pa = pte->pa << 12;
        }
    }

    l3pte = (armv8_4k_pg_pte_t *)pte;
    l3pte->af = 1;
}

void el1_handle_exception(uint64_t ec, uint64_t iss, uint64_t addr)
{
    armv8_data_abort_iss_t dai = {.raw = iss};
//    armv8_inst_abort_iss_t iai = {.raw = iss};
    switch (ec) {
    case 0x17:      /* SMC from aarch64 */
        switch (iss) {
            /*
        case SMC_YIELD:
            DEBUG_MSG("took an SMC(SMC_YIELD) exception\n");
            monitor_switch();
            break;
        case SMC_DISPATCH_MONITOR:
            DEBUG_MSG("took an SMC(SMC_DSPATCH_MONITOR) exception\n");
            el1_dispatch(NULL);
            break;
        case SMC_NOOP:
            DEBUG_MSG("took an SMC(SMC_NOOP) exception\n");
            break;
            */
        default:
            printf("Unrecognized AArch64 SMC opcode: iss = %d\n", iss);
        }
        break;
    case 0x20:
        printf("Instruction abort at lower level: address = %0lx\n",
               addr);
        break;
    case 0x21:
        printf("Instruction abort at current level (EL3): address = %0lx\n",
               addr);
        el1_map_va(addr);
        break;
    case 0x24:
        printf("Data abort (%s) at lower level: address = %0lx\n",
               dai.wnr ? "write" : "read", addr);
        break;
    case 0x25:
        printf("Data abort (%s) at current level (EL3): address = %0lx\n",
               dai.wnr ? "write" : "read", addr);
        el1_map_va(addr);
        break;

    default:
        printf("Unhandled EL3 exception: EC = %d  ISS = %d\n", ec, iss);
        break;
    }
}

#define __smc(_op)      \
    asm volatile (      \
        "smc #2 \n"  \
    )

void el1_start()
{
    int i = 0;

    while (1) {
        printf("%d: Entered %s el1_start\n", i, SECURE_STATE);

        __smc(SMC_YIELD);
        i++;
    }

    return;
}
