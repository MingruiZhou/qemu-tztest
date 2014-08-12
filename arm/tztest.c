#include "tztest.h"
#include "tztest_builtins.h"
#include "libcflat.h"

#define CALL(_f)  __svc(0, _f)
#define RETURN(_r)  __svc(0,(_r))

#define EXEC_SECURE(_op, _func, _ret)           \
    do {                                        \
        tztest_svc_desc_t _desc;                \
        _desc.secure_dispatch.func = (_func);   \
        __svc((_op), &_desc);                   \
        (_ret) = _desc.secure_dispatch.ret;     \
    } while(0)

#define EXEC_SECURE_USR(_func, _ret)            \
        EXEC_SECURE(SVC_DISPATCH_SECURE_USR, (_func), (_ret))
#define EXEC_SECURE_SVC(_func, _ret)            \
        EXEC_SECURE(SVC_DISPATCH_SECURE_SVC, (_func), (_ret))
#define EXEC_MONITOR(_func, _ret)            \
        EXEC_SECURE(SVC_DISPATCH_MONITOR, (_func), (_ret))

/* Make the below globals volatile as  found that the compiler uses the
 * register value ratherh than the memory value making it look like the writes
 * actually happened.
 */
extern uint32_t _shared_memory_heap_base;

void P0_nonsecure_check_smc()
{
    validate_state(CPSR_MODE_USR, TZTEST_STATE_NONSECURE);
    printf("\nValidating non-secure P0 smc behavior:\n");
    printf("\tUnprivileged P0 smc call ... ");
    TEST_EXCP_COND(asm volatile (".arch_extension sec\n" "smc #0\n"),
                   CPSR_MODE_UND, ==);
}

void P0_nonsecure_check_register_access()
{
    validate_state(CPSR_MODE_USR, TZTEST_STATE_NONSECURE);

    /* Set things to non-secure P1 and attempt accesses */
    printf("\nValidating non-secure P0 restricted register access:\n");

    printf("\tNonsecure P0 SCR read ... ");
    TEST_EXCP_COND(_read_scr(), CPSR_MODE_UND, ==);

    printf("\tNonsecure P0 SCR write ... ");
    TEST_EXCP_COND(_write_scr(0), CPSR_MODE_UND, ==);

    printf("\tNonsecure P0 SDER read ... ");
    TEST_EXCP_COND(_read_sder(), CPSR_MODE_UND, ==);

    printf("\tNonsecure P0 SDER write ... ");
    TEST_EXCP_COND(_write_sder(0), CPSR_MODE_UND, ==);

    printf("\tNonsecure P0 MVBAR read ... ");
    TEST_EXCP_COND(_read_mvbar(), CPSR_MODE_UND, ==);

    printf("\tNonsecure P0 MVBAR write ... ");
    TEST_EXCP_COND(_write_mvbar(0), CPSR_MODE_UND, ==);

    printf("\tNonsecure P0 NSACR write ... ");
    TEST_EXCP_COND(_write_nsacr(0), CPSR_MODE_UND, ==);
}

int P0_secure_check_register_access()
{
    validate_state(CPSR_MODE_USR, TZTEST_STATE_SECURE);

    /* Set things to non-secure P1 and attempt accesses */
    printf("\nValidating secure P0 restricted register access:\n");

    printf("\tSecure P0 SCR read ... ");
    TEST_EXCP_COND(_read_scr(), CPSR_MODE_UND, ==);

    printf("\tSecure P0 SCR write ... ");
    TEST_EXCP_COND(_write_scr(0), CPSR_MODE_UND, ==);

    printf("\tSecure P0 SDER read ... ");
    TEST_EXCP_COND(_read_sder(), CPSR_MODE_UND, ==);

    printf("\tSecure P0 SDER write ... ");
    TEST_EXCP_COND(_write_sder(0), CPSR_MODE_UND, ==);

    printf("\tSecure P0 MVBAR read ... ");
    TEST_EXCP_COND(_read_mvbar(), CPSR_MODE_UND, ==);

    printf("\tSecure P0 MVBAR write ... ");
    TEST_EXCP_COND(_write_mvbar(0), CPSR_MODE_UND, ==);

    printf("\tSecure P0 NSACR write ... ");
    TEST_EXCP_COND(_write_nsacr(0), CPSR_MODE_UND, ==);

    RETURN(0);
}

void P0_nonsecure_check_memory_access()
{
    validate_state(CPSR_MODE_USR, TZTEST_STATE_NONSECURE);

    tztest_svc_desc_t desc;
    printf("\nValidating non-secure P0 restricted memory access:\n");

    DEBUG_MSG("Writing %p\n", &_shared_memory_heap_base);
    *(uint32_t *)&_shared_memory_heap_base = 42;
}

#define TZTEST_SECURE_USR_PATTERN 0x87654321
#define TZTEST_SECURE_SVC_PATTERN 0x12345678
int tztest_secure_usr_test1()
{
    DEBUG_MSG("Entered\n");
    __svc(1,0);
    DEBUG_MSG("Exiting\n");
    RETURN(TZTEST_SECURE_USR_PATTERN);
}

int tztest_secure_svc_test1()
{
    DEBUG_MSG("Entered\n");
    DEBUG_MSG("Exiting\n");
    return TZTEST_SECURE_SVC_PATTERN;
}

void tztest_check_secure_usr_handshake()
{
    uint32_t ret = 0;
    EXEC_SECURE_USR(tztest_secure_usr_test1, ret);

    if (TZTEST_SECURE_USR_PATTERN != ret) {
        DEBUG_MSG("\n***** Failed secure usr handshake *****\n");
        assert(TZTEST_SECURE_USR_PATTERN == ret);
    }
}

void tztest_check_secure_svc_handshake()
{
    uint32_t ret = 0;
    EXEC_SECURE_SVC(tztest_secure_svc_test1, ret);

    if (TZTEST_SECURE_SVC_PATTERN != ret) {
        DEBUG_MSG("\n***** Failed secure svc handshake *****\n");
        assert(TZTEST_SECURE_SVC_PATTERN == ret);
    }
}
int mon_noop()
{
    return 0;
}

int MON_check_state()
{
    printf("\nValidating monitor mode:\n");

    printf("\tChecking for monitor mode... ");
    if (CPSR_MODE_MON != (_read_cpsr() & CPSR_MODE_MASK)) {
        printf("FAILED\n");
        INC_FAIL_COUNT();
    } else {
        printf("PASSED\n");
    }
    INC_TEST_COUNT();

    int scr = _read_scr();
    int ret = 0;

    /* B1-1211: SMC exceptions from monitor mode cause transition to secure
     * state.
     */

    /* Set our security state to secure */
    _write_scr(scr & ~SCR_NS);
    EXEC_MONITOR(mon_noop, ret);
    printf("\tChecking for secure state after secure monitor smc... ");
    if (!SCR_NS != (_read_scr() & SCR_NS)) {
        printf("FAILED\n");
        INC_FAIL_COUNT();
    } else {
        printf("PASSED\n");
    }
    INC_TEST_COUNT();

    /* Set our security state to nonsecure */
    _write_scr(scr | SCR_NS);
    EXEC_MONITOR(mon_noop, ret);
    printf("\tChecking for secure state after nonsecure monitor smc... ");
    if (!SCR_NS != (_read_scr() & SCR_NS)) {
        printf("FAILED\n");
        INC_FAIL_COUNT();
    } else {
        printf("PASSED\n");
    }
    INC_TEST_COUNT();

    /* Restore the original SCR */
    _write_scr(scr);

}

#define TZTEST_SVAL 0xaaaaaaaa
#define TZTEST_NSVAL ~TZTEST_SVAL
#define TZTEST_GET_REG_SECURE_BANK(_reg, _val)      \
    do {                                            \
        _write_scr(scr & ~SCR_NS);                  \
        (_val) = _read_##_reg();                    \
    } while(0)

#define TZTEST_GET_REG_NONSECURE_BANK(_reg, _val)   \
    do {                                            \
        _write_scr(scr | SCR_NS);                   \
        (_val) = _read_##_reg();                    \
    } while(0)

#define TZTEST_SET_REG_SECURE_BANK(_reg, _val)      \
    do {                                            \
        _write_scr(scr & ~SCR_NS);                  \
        _write_##_reg(_val);                        \
    } while(0)

#define TZTEST_SET_REG_NONSECURE_BANK(_reg, _val)   \
    do {                                            \
        _write_scr(scr | SCR_NS);                   \
        _write_##_reg(_val);                        \
    } while(0)

#define TZTEST_GET_REG_BANKS(_reg, _sval, _nsval)   \
    do {                                            \
        TZTEST_GET_REG_SECURE_BANK(_reg, _sval);    \
        TZTEST_GET_REG_NONSECURE_BANK(_reg, _nsval);\
    } while(0)

#define TZTEST_SET_REG_BANKS(_reg, _sval, _nsval)   \
    do {                                            \
        TZTEST_SET_REG_SECURE_BANK(_reg, _sval);    \
        TZTEST_SET_REG_NONSECURE_BANK(_reg, _nsval);\
    } while(0)

#define VERIFY_REGISTER_CUSTOM(_reg, _mask, _sval, _nsval)              \
    do {                                                                \
        unsigned int sval = 0, nsval = 0;                               \
        unsigned int _reg[2] = {0,0};                                   \
        printf("\tChecking %s banks...", #_reg);                        \
        TZTEST_GET_REG_BANKS(_reg, _reg[!SCR_NS], _reg[SCR_NS]);        \
        TZTEST_SET_REG_BANKS(_reg, (_sval), (_nsval));                  \
        TZTEST_GET_REG_SECURE_BANK(_reg, sval);                         \
        TZTEST_GET_REG_NONSECURE_BANK(_reg, nsval);                     \
        if ((sval & (_mask)) == (nsval & (_mask)) ||                    \
            ((_sval)& (_mask)) != (sval & (_mask)) ||              \
            ((_nsval)& (_mask)) != (nsval & (_mask))) {            \
            printf("FAILED\n");                                         \
            DEBUG_MSG("(%s) sval = 0x%x  nsval = 0x%x\n",               \
                      #_reg, sval, nsval);                              \
            INC_FAIL_COUNT();                                           \
        } else {                                                        \
            printf("PASSED\n");                                         \
        }                                                               \
        INC_TEST_COUNT();                                               \
        TZTEST_SET_REG_BANKS(_reg, _reg[!SCR_NS], _reg[SCR_NS]);        \
    } while(0)

#define VERIFY_REGISTER(_reg)    \
    VERIFY_REGISTER_CUSTOM(_reg, 0xFFFFFFFF, TZTEST_SVAL, TZTEST_NSVAL)

extern int nsec_l1_page_table;
extern int nonsecure_vectors;
int MON_check_banked_regs()
{
    int scr = _read_scr();
    uint32_t val = 0;

    printf("\nValidate secure state access to banked registers:\n");

    VERIFY_REGISTER_CUSTOM(csselr, 0xF, TZTEST_SVAL, TZTEST_NSVAL);
    /* Modifying SCTLR is highly disruptive, so the test is heavily restricted
     * to avoid complications.  We only flip the V-bit for comparison which is
     * safe unless we take an exception which should be low risk.
     */
    val = _read_sctlr();
    VERIFY_REGISTER_CUSTOM(sctlr, (1 << 13), val, (val| (1 << 13)));

    /* ACTLR is banked but not supported on Vexpress */

    /* For testing purposes we switch the secure world to the nonsecure page
     * table, so any translations can still be made.  Since we are only working
     * out of the non-secure mappings we should be safe.  This assumption could
     * be wrong.
     */
    VERIFY_REGISTER_CUSTOM(ttbr0, 0xFFF00000,
                           (int)&nsec_l1_page_table, TZTEST_NSVAL);
    VERIFY_REGISTER(ttbr1);

    /* Modifying TTBCR is highly disruptive, so the test is heavily restricted
     * to avoid complications.  We only use the PD1-bit for comparison as we
     * are not using ttbr1 at this time.
     */
    val = _read_ttbcr();

    VERIFY_REGISTER_CUSTOM(ttbcr, (1 << 5), val, (val | (1 << 5)));
    /* Leave the bottom 4 bits alone as they will disrupt address translation
     * otherwise.
     */
    VERIFY_REGISTER_CUSTOM(dacr, 0xFFFFFFF0, 0x55555555, 0xAAAAAAA5);

    VERIFY_REGISTER(dfsr);
    VERIFY_REGISTER(ifsr);
    VERIFY_REGISTER(dfar);
    VERIFY_REGISTER(ifar);
    VERIFY_REGISTER_CUSTOM(par, 0xFFFFF000, TZTEST_SVAL, TZTEST_NSVAL);
    VERIFY_REGISTER(prrr);
    VERIFY_REGISTER(nmrr);

    /* The bottome 5 bits are SBZ */
    VERIFY_REGISTER_CUSTOM(vbar, 0xFFFFFFE0, TZTEST_SVAL, TZTEST_NSVAL);

    VERIFY_REGISTER(fcseidr);
    VERIFY_REGISTER(contextidr);
    VERIFY_REGISTER(tpidrurw);
    VERIFY_REGISTER(tpidruro);
    VERIFY_REGISTER(tpidrprw);

    /* Restore the SCR to it's original value */
    _write_scr(scr);
}

void tztest_nonsecure_usr_main()
{
    int ret = 0;

    DEBUG_MSG("Entered\n");
    *tztest_test_count = 0;
    *tztest_fail_count = 0;

    P0_nonsecure_check_smc();
    P0_nonsecure_check_register_access();
//    P0_nonsecure_check_memory_access();

#ifdef DEBUG
    tztest_check_secure_usr_handshake();
    tztest_check_secure_svc_handshake();
#endif

    EXEC_SECURE_USR(P0_secure_check_register_access, ret);

    EXEC_MONITOR(MON_check_state, ret);
    EXEC_MONITOR(MON_check_banked_regs, ret);

    printf("\nValidation complete.  Passed %d of %d tests\n",
           *tztest_test_count-*tztest_fail_count, *tztest_test_count);

    DEBUG_MSG("Exiting\n");
}

#ifdef MMU_ENABLED
    // Test: Access to secure memory only allowed from secure mode
    //      pg. B1-1156
#endif
#ifdef VIRT_ENABLED
    // Test: PL2 and secure not combinable
    //      pg. B1-1157
    //      "hyp mode is only available when NS=1"
    // Test: An axception cannot be taken fromsecure mode to nonsecure (2->1)
    //      pg  B1-1138
#endif
#ifdef SIMD_ENABLED
    // Test: Check that vector ops are undefined if CPACR/NSACR
#endif
    // Test: Check that monitor mode has access to secure resource despite NS
    //      pg. B1-1140
    // Test: Check that access to banked regs from mon mode depend on NS
    // Test: Check that code residing in secure memory can't be exec from ns
    //      pg. B1-1147
    // Test: Check that CPSR.M unpredictable values don't cause entry to sec
    //      pg. B1-1150
    //      NS state -> mon mode
    //      NSACR.RFR=1 + NS state -> FIQ
    // Test: SCR.FW/AW protects access to CPSR.F/Aa when nonsecure
    //      pg. B1-1151
    //      table B1-2
    // Test: Check that non-banked (shared) regs match between sec and ns
    //      pg. B1-1157
    // Test?: Should e check that SC.NS is deprecated in any mode but mon?
    //      pg. B1-1157
    // Test: Check that mon mode is only avail if sec ext. present
    //      pg. B1-1157
    // Test: Check that smc is only aval if sec. ext. present
    //      pg. B1-1157
    // Test: Check that the SCTLR.V bit is banked allowing mix of vectors
    //      pg. B1-1158
    // Test: Check that the low exception vecotr base address is banked
    //      pg. B1-1158
    // Test: Check that using SCR aborts/irqs/fiqs are routed to mon mode
    //      pg. B1-1158
    // Test: Check that using SCR aborts/irqs/fiqs can be masked
    //      pg. B1-1158
    // Test: Check that SCR.SIF/SCD are not restrictive when EL2 not present
    //      pg. B1-1158
    // Test: Check that distinct vec tables possible for mon/sPL1/nsPL1
    //      pg. B1-1165
    // Test: Check that unused vec tables are not used
    //      table B1-3
    // Test: Check that irq/fiq interrupts routed to mon mode use mvbar entry
    //      pg. B1-1168
    // Test: Check that an exception from mon mode, NS cleared to 0
    //      pg. B1-1170
    // Test: Check that an exception taken from sec state is taken to sec state
    //      in the default mode for the excp.
    //      pg. B1-1173
    // Test: Check that an exception taken from ns state is taken to ns state
    //      in the default mode for the excp.
    //      pg. B1-1173
    // Test: Check that undef exceptions are routed properly
    //      figure: B1-3
    // Test: Check that svc exceptions are routed properly
    //      figure: B1-4
    // Test: Check that prefetch abort exceptions are routed properly
    //      figure: B1-6
    // Test: Check that data abort exceptions are routed properly
    //      figure: B1-7
    // Test: Check that IRQ exceptions are routed properly
    //      figure: B1-8
    // Test: Check that FIQ exceptions are routed properly
    //      figure: B1-9
    // Test: Check that CPSR.A/I.F are set to 1 on exception to mon mode
    //      pg. B1-1182
    // Test: Check that on reset if sec et present, starts in sec state
    //      pg. B1-1204
    // Note: Unaligned access can cause abort in PMSA
    //
