/*
 * Copyright (C) 2018 UC Berkeley
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file
 * @brief       OpenThread border router application
 *
 * @author      Hyung-Sin Kim <hs.kim@cs.berkeley.edu>
 */

#include <stdio.h>
#include "ot.h"
#include <openthread/udp.h>
#include <openthread/cli.h>
#include <openthread/openthread.h>
#include "sched.h"
#include "periph/gpio.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

#define BLINK_INTERVAL 1000000ul

/* OpenThread network parameters */
uint16_t myRloc = 0;

uint32_t packetReTxArray[11];
uint32_t packetReTxCnt = 0;
uint32_t packetSuccessCnt = 0;
uint32_t packetFailCnt = 0;
uint32_t packetBusyChannelCnt = 0;
uint32_t broadcastCnt = 0;
uint32_t queueOverflowCnt = 0;

uint32_t totalIpv6MsgCnt = 0;
uint32_t Ipv6TxSuccCnt = 0;
uint32_t Ipv6TxFailCnt = 0;
uint32_t Ipv6RxSuccCnt = 0;
uint32_t Ipv6RxFailCnt = 0;

uint16_t nextHopRloc = 0;
uint8_t borderRouterLC = 0;
uint8_t borderRouterRC = 0;
uint32_t borderRouteChangeCnt = 0;
uint32_t routeChangeCnt = 0;

uint32_t pollMsgCnt = 0;
uint32_t mleMsgCnt = 0;

uint32_t mleRouterMsgCnt = 0;
uint32_t addrMsgCnt = 0;
uint32_t netdataMsgCnt = 0;

uint32_t meshcopMsgCnt = 0;
uint32_t tmfMsgCnt = 0;

uint32_t totalSerialMsgCnt = 0;


int main(void)
{
    /* Run wpantund to interact with NCP */
    DEBUG("This a test for OpenThread MTD (sleepy node)\n");
    
    while (1) {
		//Sleep
        xtimer_usleep(BLINK_INTERVAL);
    }
    return 0;
}
