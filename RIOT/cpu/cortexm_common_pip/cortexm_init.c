/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_cortexm_common_pip
 * @{
 *
 * @file
 * @brief       Cortex-M specific configuration and initialization options
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include "cpu.h"

#include "svc.h"

/**
 * Interrupt vector base address, defined by the linker
 */
extern const void *_isr_vectors;

#if defined(CPU_CORTEXM_INIT_SUBFUNCTIONS)
#define CORTEXM_STATIC_INLINE /*empty*/
#else
#define CORTEXM_STATIC_INLINE static inline
#endif

CORTEXM_STATIC_INLINE void cortexm_init_isr_priorities(void)
{
#if CPU_CORTEXM_PRIORITY_GROUPING != 0
    /* If defined, initialise priority subgrouping, see cpu_conf_common.h */
    NVIC_SetPriorityGrouping(CPU_CORTEXM_PRIORITY_GROUPING);
#endif

    /* initialize the interrupt priorities */
    /* set pendSV interrupt to its own priority */
    NVIC_SetPriority(PendSV_IRQn, CPU_CORTEXM_PENDSV_IRQ_PRIO);
    /* initialize all vendor specific interrupts with the default priority */
    for (unsigned i = 0; i < CPU_IRQ_NUMOF; i++) {
        NVIC_SetPriority((IRQn_Type) i, CPU_DEFAULT_IRQ_PRIO);
    }
}

CORTEXM_STATIC_INLINE void cortexm_init_misc(void)
{
    /* enable wake up on events for __WFE CPU sleep */
    Pip_out(PIP_ARMV7M_SCS_SCID_SCR,
        Pip_in(PIP_ARMV7M_SCS_SCID_SCR) | SCB_SCR_SEVONPEND_Msk);

    /* for Cortex-M3 r1p0 and up the STKALIGN option was added, but not automatically
     * enabled until revision r2p0. For 64bit function arguments to work properly this
     * needs to be enabled.
     */
#ifdef SCB_CCR_STKALIGN_Msk
    Pip_out(PIP_ARMV7M_SCS_SCID_CCR,
        Pip_in(PIP_ARMV7M_SCS_SCID_CCR) | SCB_CCR_STKALIGN_Msk);
#endif
}

void cortexm_init(void)
{
    cortexm_init_isr_priorities();
    cortexm_init_misc();
}

bool cpu_check_address(volatile const char *address)
{
#if defined(CPU_CORE_CORTEX_M3) || defined(CPU_CORE_CORTEX_M33) || \
    defined(CPU_CORE_CORTEX_M4) || defined(CPU_CORE_CORTEX_M4F) || \
    defined(CPU_CORE_CORTEX_M7)
    static const uint32_t BFARVALID_MASK = (0x80 << SCB_CFSR_BUSFAULTSR_Pos);

    bool is_valid = true;

    /* Clear BFARVALID flag */
    Pip_out(PIP_ARMV7M_SCS_SCID_CFSR,
        Pip_in(PIP_ARMV7M_SCS_SCID_CFSR) | BFARVALID_MASK);

    /* Ignore BusFault by enabling BFHFNMIGN and disabling interrupts */
    uint32_t mask = __get_FAULTMASK();
    __disable_fault_irq();
    Pip_out(PIP_ARMV7M_SCS_SCID_CCR,
        Pip_in(PIP_ARMV7M_SCS_SCID_CCR) | SCB_CCR_BFHFNMIGN_Msk);

    *address;
    /* Check BFARVALID flag */
    if ((Pip_in(PIP_ARMV7M_SCS_SCID_CFSR) & BFARVALID_MASK) != 0) {
        /* Bus Fault occurred reading the address */
        is_valid = false;
    }

    /* Re-enable BusFault by clearing  BFHFNMIGN */
    Pip_out(PIP_ARMV7M_SCS_SCID_CCR,
        Pip_in(PIP_ARMV7M_SCS_SCID_CCR) & ~SCB_CCR_BFHFNMIGN_Msk);
    __set_FAULTMASK(mask);

    return is_valid;
#else
    /* Cortex-M0 doesn't have BusFault so we need to catch HardFault */
    (void)address;

    bool result;

    __asm__ volatile (
        "movs r5, #1            \n" /* R5 will be set to 0 by HardFault handler */
                                    /* to indicate HardFault has occurred */
        "ldr  r1, =0xDEADF00D   \n" /* set magic number     */
        "ldr  r2, =0xCAFEBABE   \n" /* 2nd magic to be sure */
        "ldrb r3, %1            \n" /* probe address        */
        "mov  %0, r5            \n" /* store result */
        : "=r"(result)
        : "m"(*address)
        : "r1", "r2", "r3", "r5", "cc"
     );

    return result;
#endif
}
