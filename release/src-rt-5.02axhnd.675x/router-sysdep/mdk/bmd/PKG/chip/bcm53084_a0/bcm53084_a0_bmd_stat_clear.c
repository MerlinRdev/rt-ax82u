#include <bmd_config.h>
#if CDK_CONFIG_INCLUDE_BCM53084_A0 == 1

/*
 * $Id: bcm53084_a0_bmd_stat_clear.c,v 1.2 Broadcom SDK $
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

#include <cdk/chip/bcm53084_a0_defs.h>

#include "bcm53084_a0_bmd.h"

typedef union {
    TXUNICASTPKTSr_t txunicastpkts;
    TXMULTICASTPKTSr_t txmulticastpkts;
    TXBROADCASTPKTSr_t txbroadcastpkts;
    TXPAUSEPKTSr_t txpausepkts;
    TXOCTETSr_t txoctets;
    TXEXCESSIVECOLLISIONr_t txexcessivecollision;
    TXLATECOLLISIONr_t txlatecollision;
    RXUNICASTPKTSr_t rxunicastpkts;
    RXMULTICASTPKTSr_t rxmulticastpkts;
    RXBROADCASTPKTSr_t rxbroadcastpkts;
    RXPAUSEPKTSr_t rxpausepkts;
    RXOCTETSr_t rxoctets;
    RXJABBERSr_t rxjabbers;
    RXFCSERRORSr_t rxfcserrors;
    RXOVERSIZEPKTSr_t rxoversizepkts;
    RXALIGNMENTERRORSr_t rxalignmenterrors;
    RXSYMBLERRr_t rxsymblerr;
    RXUNDERSIZEPKTSr_t rxundersizepkts;
    RXFRAGMENTSr_t rxfragments;
    RXDROPPKTSr_t rxdroppkts;
    RXDISCARDr_t rxdiscard;
} bcm53084_a0_counter_t;

int 
bcm53084_a0_bmd_stat_clear(int unit, int port, bmd_stat_t stat)
{
    int ioerr = 0;
    bcm53084_a0_counter_t ctr;

    BMD_CHECK_UNIT(unit);
    BMD_CHECK_PORT(unit, port);

    CDK_MEMSET(&ctr, 0, sizeof(ctr));

    switch (stat) {
    case bmdStatTxPackets:
        ioerr += WRITE_TXUNICASTPKTSr(unit, port, ctr.txunicastpkts);
        ioerr += WRITE_TXMULTICASTPKTSr(unit, port, ctr.txmulticastpkts);
        ioerr += WRITE_TXBROADCASTPKTSr(unit, port, ctr.txbroadcastpkts);
        ioerr += WRITE_TXPAUSEPKTSr(unit, port, ctr.txpausepkts);
        break;
    case bmdStatTxBytes:
        ioerr += WRITE_TXOCTETSr(unit, port, ctr.txoctets);
        break;
    case bmdStatTxErrors:
        ioerr += WRITE_TXEXCESSIVECOLLISIONr(unit, 
			port, ctr.txexcessivecollision);
        ioerr += WRITE_TXLATECOLLISIONr(unit, port, ctr.txlatecollision);
        break;
    case bmdStatRxPackets:
        ioerr += WRITE_RXUNICASTPKTSr(unit, port, ctr.rxunicastpkts);
        ioerr += WRITE_RXMULTICASTPKTSr(unit, port, ctr.rxmulticastpkts);
        ioerr += WRITE_RXBROADCASTPKTSr(unit, port, ctr.rxbroadcastpkts);
        ioerr += WRITE_RXPAUSEPKTSr(unit, port, ctr.rxpausepkts);
        break;
    case bmdStatRxBytes:
        ioerr += WRITE_RXOCTETSr(unit, port, ctr.rxoctets);
        break;
    case bmdStatRxErrors:
        ioerr += WRITE_RXJABBERSr(unit, port, ctr.rxjabbers);
        ioerr += WRITE_RXFCSERRORSr(unit, port, ctr.rxfcserrors);
        ioerr += WRITE_RXOVERSIZEPKTSr(unit, port, ctr.rxoversizepkts);
        ioerr += WRITE_RXALIGNMENTERRORSr(unit, port, ctr.rxalignmenterrors);
        ioerr += WRITE_RXSYMBLERRr(unit, port, ctr.rxsymblerr);
        ioerr += WRITE_RXUNDERSIZEPKTSr(unit, port, ctr.rxundersizepkts);
        ioerr += WRITE_RXFRAGMENTSr(unit, port, ctr.rxfragments);
        break;
    case bmdStatRxDrops:
        ioerr += WRITE_RXDROPPKTSr(unit, port, ctr.rxdroppkts);
        ioerr += WRITE_RXDISCARDr(unit, port, ctr.rxdiscard);
        break;
    default:
        break;
    }

    return ioerr ? CDK_E_IO : CDK_E_NONE;
}

#endif /* CDK_CONFIG_INCLUDE_BCM53084_A0 */