#ifndef _TZTEST_BUILTINS_H
#define _TZTEST_BUILTINS_H

#include "sm.h"

/* SMC intrinsic
 * Need to enable the security extension on v7 or the assembler will complain
 */
#define __smc(_op, _data)                       \
    asm volatile (                              \
        ".arch_extension sec\n"                 \
        "mov r0, %[r0]\n"                       \
        "mov r1, %[r1]\n"                       \
        "smc 0\n"                               \
        "mov %[r0], r0\n"                       \
        "mov %[r1], r1\n"                       \
        : [r0] "+r" (_op), [r1] "+r" (_data)    \
    )

#define __svc(_op, _indata)                     \
    asm volatile (                              \
        "mov r0, %[r0]\n"                       \
        "mov r1, %[r1]\n"                       \
        "svc 0\n"                               \
        : : [r0] "r" (_op), [r1] "r" (_indata)  \
    )
#define __cps(_r0) asm volatile ("cps %[r0]\n":: [r0] "X" (_r0))

#define __mrs(_r0, _reg) asm volatile ("mrs %[r0], "#_reg"\n" : [r0] "=r" (_r0))
#define __msr(_reg, _r0) asm volatile ("msr "#_reg", %[r0]\n" :: [r0] "r" (_r0))

#define __srsdb_svc(_mode) asm volatile ("srsdb sp!, #0x13\n")

#define __pop(_r0) \
    asm volatile ("pop {%[r0]}\n" : [r0] "=r" (_r0))

#define __mrc(_cp, _opc1, _r0, _crm, _crn, _opc2)                           \
    asm volatile (                                                          \
        "mrc p"#_cp", "#_opc1", %[r0], c"#_crm", c"#_crn", "#_opc2"\n"      \
        : [r0] "=r" (_r0)                                                   \
    )

#define __mcr(_cp, _opc1, _r0, _crm, _crn, _opc2)                           \
    asm volatile (                                                          \
        "mcr p"#_cp", "#_opc1", %[r0], c"#_crm", c"#_crn", "#_opc2"\n"      \
        : : [r0] "r" (_r0)                                                  \
    )

#define _READCP(_reg, _cp, _opc1, _crm, _crn, _opc2)    \
    static inline uint32_t _read_##_reg() {             \
        volatile int _r0 = -1;                          \
        __mrc(_cp, _opc1, _r0, _crm, _crn, _opc2);      \
        return _r0;                                     \
    }

#define _WRITECP(_reg, _cp, _opc1, _crm, _crn, _opc2)       \
    static inline uint32_t  _write_##_reg(uint32_t _r0) {   \
        __mcr(_cp, _opc1, _r0, _crm, _crn, _opc2);          \
        return 0;                                           \
    }

#define _RWCP(_reg, _cp, _opc1, _crm, _crn, _opc2)  \
    _READCP(_reg, _cp, _opc1, _crm, _crn, _opc2)    \
    _WRITECP(_reg, _cp, _opc1, _crm, _crn, _opc2)

_RWCP(scr, 15, 0, 1, 1, 0)           /* _read/write_scr */
_RWCP(sder, 15, 0, 1, 1, 1)          /* _read/write_sder */
_RWCP(nsacr, 15, 0, 1, 1, 2)         /* _read/write_nsacr */
_RWCP(mvbar, 15, 0, 12, 0, 1)        /* _read/write_mvbar */

/* Banked read/write CP register definitions */
_RWCP(csselr, 15, 2, 0, 0, 0)        /* _read/write_csselr */
_RWCP(sctlr, 15, 0, 1, 0, 0)         /* _read/write_sctlr */
_RWCP(actlr, 15, 0, 1, 0, 1)         /* _read/write_actlr */
_RWCP(ttbr0, 15, 0, 2, 0, 0)         /* _read/write_ttbr0 */
_RWCP(ttbr1, 15, 0, 2, 0, 1)         /* _read/write_ttbr1 */
_RWCP(ttbcr, 15, 0, 2, 0, 2)         /* _read/write_ttbcr */
_RWCP(dacr, 15, 0, 3, 0, 0)          /* _read/write_dacr */
_RWCP(dfsr, 15, 0, 5, 0, 0)          /* _read/write_dfsr */
_RWCP(ifsr, 15, 0, 5, 0, 1)          /* _read/write_ifsr */
_RWCP(dfar, 15, 0, 6, 0, 0)          /* _read/write_dfar */
_RWCP(ifar, 15, 0, 6, 0, 2)          /* _read/write_ifar */
_RWCP(par, 15, 0, 7, 4, 0)           /* _read/write_par */
_RWCP(prrr, 15, 0, 10, 2, 0)         /* _read/write_prrr */
_RWCP(nmrr, 15, 0, 10, 2, 1)         /* _read/write_nmrr */
_RWCP(vbar, 15, 0, 12, 0, 0)         /* _read/write_vbar */
_RWCP(fcseidr, 15, 0, 13, 0, 0)      /* _read/write_fcseidr */
_RWCP(contextidr, 15, 0, 13, 0, 1)   /* _read/write_contextidr*/
_RWCP(tpidrurw, 15, 0, 13, 0, 2)     /* _read/write_tpidrurw */
_RWCP(tpidruro, 15, 0, 13, 0, 3)     /* _read/write_tpidruro */
_RWCP(tpidrprw, 15, 0, 13, 0, 4)     /* _read/write_tpidrprw */

static inline uint32_t _read_cpsr() {
    uint32_t r0 = 0;
    __mrs(r0, cpsr);
    return r0;
}

static inline void _write_cpsr(uint32_t val) {
    __msr(cpsr_fsxc, val);
}
#endif
