#include <bmd_config.h>
#if CDK_CONFIG_INCLUDE_BCM53084_A0 == 1

/*
 * $Id: bcm53084_a0_bmd_rx.c,v 1.5 Broadcom SDK $
 * 
 * $Copyright: Copyright 2013 Broadcom Corporation.
 * This program is the proprietary software of Broadcom Corporation
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 */

#include <bmd/bmd.h>
#include <bmd/bmd_dma.h>

#include <cdk/chip/bcm53084_a0_defs.h>
#include <cdk/cdk_debug.h>
#include <cdk/cdk_device.h>

#include "bcm53084_a0_bmd.h"
#include "bcm53084_a0_internal.h"

#if BMD_CONFIG_INCLUDE_DMA == 1

typedef struct robo_rx_dscr_s {
    bmd_pkt_t *pkt;
} robo_rx_dscr_t;

static robo_rx_dscr_t _rx_dscr[BMD_CONFIG_MAX_UNITS];

static int
_cpu_port_enable_set(int unit, int enable)
{
    int ioerr = 0;
    IMP_CTLr_t mii_pctl;
	P5_CTLr_t p5_ctl;

    if (CDK_CHIP_CONFIG(unit) & DCFG_MBUS_SEC_USB) {
        ioerr += READ_IMP_CTLr(unit, &mii_pctl);
        IMP_CTLr_RX_BCST_ENf_SET(mii_pctl, enable);
        IMP_CTLr_RX_MCST_ENf_SET(mii_pctl, enable);
        IMP_CTLr_RX_UCST_ENf_SET(mii_pctl, enable);
        IMP_CTLr_RX_DISf_SET(mii_pctl, 0);
        IMP_CTLr_TX_DISf_SET(mii_pctl, 0);
        ioerr += WRITE_IMP_CTLr(unit, mii_pctl);
    } else {
        ioerr += READ_P5_CTLr(unit, &p5_ctl);
        P5_CTLr_RX_DISf_SET(p5_ctl, 0);
        P5_CTLr_TX_DISf_SET(p5_ctl, 0);
        ioerr += WRITE_P5_CTLr(unit, p5_ctl);
    }

    return ioerr ? CDK_E_IO : CDK_E_NONE;
}
#endif

int
bcm53084_a0_bmd_rx_start(int unit, bmd_pkt_t *pkt)
{
#if BMD_CONFIG_INCLUDE_DMA == 1
    int rv;

    BMD_CHECK_UNIT(unit);

    /* Save packet pointer locally */
    _rx_dscr[unit].pkt = pkt;
    rv = _cpu_port_enable_set(unit, 1);

    return rv; 
#else
    return CDK_E_UNAVAIL;
#endif
}

int
bcm53084_a0_bmd_rx_poll(int unit, bmd_pkt_t **ppkt)
{
#if BMD_CONFIG_INCLUDE_DMA == 1
    int rv = CDK_E_NONE;
    bmd_pkt_t *pkt = _rx_dscr[unit].pkt;
    uint8_t *brcm_hdr;

    BMD_CHECK_UNIT(unit);

    if (pkt == NULL) {
        return CDK_E_DISABLED;
    }

    rv = cdk_dev_read(unit, CDK_DEV_ADDR_ETH, pkt->data, pkt->size);
    if (CDK_FAILURE(rv)) {
        return rv;
    }
    pkt->size = rv;

    brcm_hdr = &pkt->data[12];
    if (pkt->size > (12 + ROBO_BRCM_HDR_SIZE)) {

        CDK_VVERB(("Rx BRCM header = %02x%02x %02x%02x\n",
                   brcm_hdr[0], brcm_hdr[1], brcm_hdr[2], 
                   brcm_hdr[3]));

        /* Extract ingress port from BRCM tag */
        pkt->port = brcm_hdr[3] & 0x1f;

        /* Strip Broadcom header */
        pkt->size -= ROBO_BRCM_HDR_SIZE;
        
        
        CDK_MEMCPY(brcm_hdr, &brcm_hdr[ROBO_BRCM_HDR_SIZE], pkt->size - 12);
    }

    /* Pass packet back to application */
    *ppkt = pkt;

    CDK_MEMSET(&_rx_dscr[unit], 0, sizeof(_rx_dscr[unit]));

    return rv; 
#else
    return CDK_E_UNAVAIL;
#endif
}

int
bcm53084_a0_bmd_rx_stop(int unit)
{
#if BMD_CONFIG_INCLUDE_DMA == 1
    int rv;

    BMD_CHECK_UNIT(unit);

    if (_rx_dscr[unit].pkt == NULL) {
        return CDK_E_DISABLED;
    }

    rv = _cpu_port_enable_set(unit, 0);

    CDK_MEMSET(&_rx_dscr[unit], 0, sizeof(_rx_dscr[unit]));

    return rv; 
#else
    return CDK_E_UNAVAIL;
#endif
}

#endif /* CDK_CONFIG_INCLUDE_BCM53084_A0 */