/*
 * Copyright (C) 2021 Koen Zandberg <koen@bergzand.net>
 *               2021 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     cpu_cortexm_common_pip
 * @{
 *
 * @file
 * @brief       Implementation of the kernels thread interface
 *
 * @author      Koen Zandberg <koen@bergzand.net>
 */

#ifndef THREAD_ARCH_H
#define THREAD_ARCH_H

#ifdef __cplusplus
extern "C" {
#endif

extern void *riotPartDesc;

#define THREAD_API_INLINED

#ifndef DOXYGEN /* Doxygen is in core/include/thread.h */

static inline __attribute__((always_inline)) void thread_yield_higher(void)
{
    /* Trigger the PENDSV interrupt to run scheduler and schedule new thread if
     * applicable. Do not save current context by passing the index 0 of the VIDT
     * containing a NULL pointer to the yield function. */
    Pip_yield(riotPartDesc, 14, 9, 1, 1);
}

#endif /* DOXYGEN */

#ifdef __cplusplus
}
#endif

#endif /* THREAD_ARCH_H */
/** @} */
