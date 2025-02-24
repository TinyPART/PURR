/*
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_common_nrf52_pip
 * @{
 *
 * @file
 * @brief       Default timer configuration for nRF52 based boards
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 */

#ifndef CFG_TIMER_DEFAULT_H
#define CFG_TIMER_DEFAULT_H

#include "kernel_defines.h"
#include "periph_cpu.h"
#include "register.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Timer configuration
 * @{
 */
/**
 * @brief   Configuration of the exposed timers
 *
 * @warning The timer `NRF802154_TIMER`, which by default is `TIMER_DEV(1)`,
 *          is used for the IEEE 802.15.4 driver
 */
static const timer_conf_t timer_config[] = {
    {
        .dev      = PIP_NRF_TIMER_TIMER1_BASE,
        .channels = 3,
        .bitmode  = TIMER_BITMODE_BITMODE_32Bit,
        .irqn     = TIMER1_IRQn
    },
    {
        /* BEWARE: This timer is allocated to the nRF52 IEEE 802.15.4 driver.
         *         Do not use this timer (unless you do not use IEEE 802.15.4
         *         networking)!
         */
        .dev      = PIP_NRF_TIMER_TIMER2_BASE,
        .channels = 3,
        .bitmode  = TIMER_BITMODE_BITMODE_32Bit,
        .irqn     = TIMER2_IRQn
    },
    /* The later timers are only present on the larger NRF52 CPUs like NRF52840
     * or NRF52833, but not small ones like NRF52810. They do have 2 channels
     * more (CC registers [0..5] instead of CC registers [0..3]). */
#ifdef NRF_TIMER3
    {
        .dev      = PIP_NRF_TIMER_TIMER3_BASE,
        .channels = 5,
        .bitmode  = TIMER_BITMODE_BITMODE_32Bit,
        .irqn     = TIMER3_IRQn
    },
#endif
#ifdef NRF_TIMER4
    {
        .dev      = PIP_NRF_TIMER_TIMER4_BASE,
        .channels = 5,
        .bitmode  = TIMER_BITMODE_BITMODE_32Bit,
        .irqn     = TIMER4_IRQn
    }
#endif
};

#define TIMER_0_ISR         isr_timer1
#define TIMER_1_ISR         isr_timer2
#define TIMER_2_ISR         isr_timer3
#define TIMER_3_ISR         isr_timer4

#define TIMER_NUMOF         ARRAY_SIZE(timer_config)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CFG_TIMER_DEFAULT_H */
/** @} */
