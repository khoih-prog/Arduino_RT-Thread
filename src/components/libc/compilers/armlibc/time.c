/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include "include/rtthread.h"

/**
 * Returns the current time.
 *
 * @param time_t * timep the timestamp pointer, if not used, keep NULL.
 *
 * @return time_t return timestamp current.
 *
 */
/* for IAR 6.2 later Compiler */
#if defined (__IAR_SYSTEMS_ICC__) &&  (__VER__) >= 6020000
# pragma module_name = "?time"
time_t (__time32)(time_t *timep)    /* Only supports 32-bit timestamp */
#else
time_t time(time_t *timep)
#endif
{
    time_t time_now = 0;

    #ifdef RT_USING_RTC
        static rt_device_t device = RT_NULL;

        /* optimization: find rtc device only first. */
        if (device == RT_NULL) {
            device = rt_device_find("RTC");
        }

        /* read timestamp from RTC device. */
        if (device) {
            if (RT_EOK == rt_device_open(device, 0)) {
                rt_device_control(device, RT_DEVICE_CTRL_RTC_GET_TIME,
                    &time_now);
                rt_device_close(device);
            }
        }
    #endif /* RT_USING_RTC */

    /* if timep is not NULL, write timestamp to *timep */
    if (timep) {
        *timep = time_now;
    }

    return time_now;
}

RT_WEAK clock_t clock(void) {
    return rt_tick_get();
}
