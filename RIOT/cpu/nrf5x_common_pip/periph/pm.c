/*
 * Copyright (C) 2017 Kaspar Schleiser <kaspar@schleiser.de>
 *               2014-2017 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_nrf5x_common_pip
 * @ingroup     drivers_periph_pm
 * @{
 *
 * @file
 * @brief       NRF5x specific power management implementations
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include "cpu.h"
#include "svc.h"

#ifdef NRF_POWER_S
#define NRF_POWER NRF_POWER_S
#endif

/* TODO: implement proper pm_off for nRF9160 */
void pm_off(void)
{
#ifndef CPU_FAM_NRF9160
#ifdef CPU_FAM_NRF51
    NRF_POWER->RAMON = 0;
#else
    for (int i = 0; i < 8; i++) {
        Pip_out(PIP_NRF_POWER_POWER_RAM_0_POWERCLR + i, (POWER_RAM_POWERCLR_S1RETENTION_Msk |
                                      POWER_RAM_POWERCLR_S0RETENTION_Msk));
    }
#endif
    Pip_out(PIP_NRF_POWER_POWER_SYSTEMOFF, 1);
    while (1) {}
#endif /* ndef CPU_FAM_NRF9160 */
}
