/***********************************************************************
 *
 *  Copyright (c) 2017  Broadcom Corporation
 *  All Rights Reserved
 *
<:label-BRCM:2017:proprietary:standard

 This program is the proprietary software of Broadcom and/or its
 licensors, and may only be used, duplicated, modified or distributed pursuant
 to the terms and conditions of a separate, written license agreement executed
 between you and Broadcom (an "Authorized License").  Except as set forth in
 an Authorized License, Broadcom grants no license (express or implied), right
 to use, or waiver of any kind with respect to the Software, and Broadcom
 expressly reserves all rights in and to the Software and all intellectual
 property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
 NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
 BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.

 Except as expressly set forth in the Authorized License,

 1. This program, including its structure, sequence and organization,
    constitutes the valuable trade secrets of Broadcom, and you shall use
    all reasonable efforts to protect the confidentiality thereof, and to
    use this information only in connection with your use of Broadcom
    integrated circuit products.

 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
    ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
    FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
    COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
    TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
    PERFORMANCE OF THE SOFTWARE.

 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
    ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
    INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
    WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
    OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
    SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
    SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
    LIMITED REMEDY.
:>
*
************************************************************************/

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <net/if.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "os_defs.h"

#include "archer.h"

#define ARCHER_SUCCESS                          0
#define ARCHER_ERROR                            -1

/* Limit values */
#define CMD_NAME_LEN                            16
#define MAX_OPTS                                64
#define MAX_PARMS                               16

/* Argument type values. */
#define ARG_TYPE_COMMAND                        1
#define ARG_TYPE_OPTION                         2
#define ARG_TYPE_PARAMETER                      3

/* Return codes. */
#define ARCHER_GENERAL_ERROR                    100
#define ARCHER_NOT_FOUND                        101
#define ARCHER_ALLOC_ERROR                      102
#define ARCHER_INVALID_COMMAND                  103

#define ARCHER_INVALID_OPTION                   104
#define ARCHER_INVALID_PARAMETER                105
#define ARCHER_INVALID_NUMBER_OF_OPTIONS        106
#define ARCHER_INVALID_NUMBER_OF_PARAMETERS     107

/*** Typedefs. ***/

typedef struct
{
    char    * pOptName;
    char    * pParms[MAX_PARMS];
    int     nNumParms;
} OPTION_INFO, *POPTION_INFO;

typedef int (*FN_COMMAND_HANDLER) (POPTION_INFO pOptions, int nNumOptions);

typedef struct
{
    char    szCmdName[CMD_NAME_LEN];
    char    *pszOptionNames[MAX_OPTS];
    FN_COMMAND_HANDLER pfnCmdHandler;
} COMMAND_INFO, *PCOMMAND_INFO;

//#define CC_ARCHER_API_DEBUG

static char g_PgmName[128];

static int archer_cmd_send(archer_ioctl_cmd_t cmd, unsigned long arg)
{
    int ret;
    int fd;

    fd = open(ARCHER_DRV_DEVICE_NAME, O_RDWR);
    if(fd < 0)
    {
        fprintf( stderr, "%s: %s", ARCHER_DRV_DEVICE_NAME, strerror(errno) );

        return -EINVAL;
    }

    ret = ioctl(fd, cmd, arg);
    if(ret)
    {
        fprintf( stderr, "ioctl: %s\n", strerror(errno) );
    }

    close(fd);

    return ret;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_enable_handler
 * Description  : Processes the archer enable command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_enable_handler(POPTION_INFO pOptions, int nNumOptions)
{
    fprintf( stderr, "Deprecated: Please use 'fc config --hw-accel 1'\n" );

    return 0;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_bind_handler
 * Description  : Processes the archer bind command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_bind_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet = archer_cmd_send( ARCHER_IOC_BIND, 0);

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to bind ARCHER to Flow Cache\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_disable_handler
 * Description  : Processes the archer disable command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_disable_handler(POPTION_INFO pOptions, int nNumOptions)
{
    fprintf( stderr, "Deprecated: Please use 'fc config --hw-accel 0'\n" );

    return 0;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_unbind_handler
 * Description  : Processes the archer unbind command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_unbind_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet = archer_cmd_send( ARCHER_IOC_UNBIND, 0 );

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to unbind ARCHER\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_status_handler
 * Description  : Processes the archer status command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_status_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet = archer_cmd_send( ARCHER_IOC_STATUS, 0 );

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to dump ARCHER status\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_flow_dump_handler
 * Description  : Processes the ARCHER flow dump command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_flow_dump_handler(POPTION_INFO pOptions, int nNumOptions, archer_ioctl_cmd_t cmd)
{
    int max_flows = 8;
    int nRet;

    if ( nNumOptions == 1 )
    {
        if ( !strcmp( pOptions->pOptName, "--all" ) )
        {
            if ( pOptions->nNumParms != 0 )
            {
                fprintf( stderr, "%s: --all does not take a parameter\n", g_PgmName );
                return ARCHER_INVALID_OPTION;
            }

            max_flows = 0;
        }
        else if ( !strcmp( pOptions->pOptName, "--max" ) )
        {
            if ( pOptions->nNumParms != 1 )
            {
                fprintf( stderr, "%s: must specify max_flows\n", g_PgmName );
                return ARCHER_INVALID_OPTION;
            }

            max_flows = atoi( pOptions->pParms[0] );
            if(max_flows < 1)
            {
                fprintf( stderr, "%s: must specify max_flows >= 1\n", g_PgmName );
                return ARCHER_INVALID_OPTION;
            }
        }
        else
        {
            fprintf( stderr, "%s: Invalid option name\n", g_PgmName );
            return ARCHER_INVALID_OPTION;
        }
    }
    else if ( nNumOptions > 1 )
    {
        fprintf( stderr, "%s: Too many options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }

    nRet = archer_cmd_send( cmd, max_flows );

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to dump ARCHER flows\n", g_PgmName );
    }

    return nRet;
}

int archer_flows_handler(POPTION_INFO pOptions, int nNumOptions)
{
    return archer_flow_dump_handler(pOptions, nNumOptions, ARCHER_IOC_FLOWS);
}

int archer_ucast_l3_handler(POPTION_INFO pOptions, int nNumOptions)
{
    return archer_flow_dump_handler(pOptions, nNumOptions, ARCHER_IOC_UCAST_L3);
}

int archer_ucast_l2_handler(POPTION_INFO pOptions, int nNumOptions)
{
    return archer_flow_dump_handler(pOptions, nNumOptions, ARCHER_IOC_UCAST_L2);
}

int archer_mcast_handler(POPTION_INFO pOptions, int nNumOptions)
{
    return archer_flow_dump_handler(pOptions, nNumOptions, ARCHER_IOC_MCAST);
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_host_handler
 * Description  : Processes the ARCHER print command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_host_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet = archer_cmd_send( ARCHER_IOC_HOST, 0 );

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to dump ARCHER host\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_stats_handler
 * Description  : Processes the archer stats command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_stats_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet = archer_cmd_send( ARCHER_IOC_STATS, 0);

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to stats ARCHER\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_mode_handler
 * Description  : Processes the ARCHER mode command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_mode_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet;

    if ( nNumOptions == 0 )
    {
        fprintf( stderr, "%s: No options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( nNumOptions > 1 )
    {
        fprintf( stderr, "%s: Too many options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( !strcmp( pOptions->pOptName, "--l3" ) )
    {
        if ( pOptions->nNumParms )
        {
            fprintf( stderr, "%s: Too many parameters specified\n", g_PgmName );
            return ARCHER_INVALID_PARAMETER;
        }

        nRet = archer_cmd_send( ARCHER_IOC_MODE, ARCHER_MODE_L3 );
    }
    else if ( !strcmp( pOptions->pOptName, "--l2+l3" ) )
    {
        if ( pOptions->nNumParms )
        {
            fprintf( stderr, "%s: Too many parameters specified\n", g_PgmName );
            return ARCHER_INVALID_PARAMETER;
        }

        nRet = archer_cmd_send( ARCHER_IOC_MODE, ARCHER_MODE_L2_L3 );
    }
    else
    {
        fprintf( stderr, "%s: invalid option [%s]\n", g_PgmName, pOptions->pOptName );
        return ARCHER_INVALID_OPTION;
    }

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to set ARCHER Debug Level\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_mpdcfg_handler
 * Description  : Processes the ARCHER MPD config command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_mpdcfg_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet;

    if ( nNumOptions == 0 )
    {
        fprintf( stderr, "%s: No options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( nNumOptions > 1 )
    {
        fprintf( stderr, "%s: Too many options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( !strcmp( pOptions->pOptName, "--int" ) )
    {
        archer_mpd_cfg_t mpd_cfg;

        memcpy( mpd_cfg.intf_name, pOptions->pParms[0], 16 );
        
        if ( pOptions->nNumParms > 1 )
        {
            int i;
            char tmp[2];
            unsigned char v8;

            mpd_cfg.mode = ARCHER_MPD_ADDR_SPEC;
            // extract the specified mac address
            for (i=0; i < 6; i++)
            {
                tmp[0] = pOptions->pParms[1][i*3];
                tmp[1] = pOptions->pParms[1][i*3 + 1];

                v8 = strtol(tmp, NULL, 16);

                mpd_cfg.mac_addr[i] = v8;
            }
        }
        else
        {
            mpd_cfg.mode = ARCHER_MPD_INTF;
        }
        nRet = archer_cmd_send( ARCHER_IOC_MPDCFG, (unsigned long)&mpd_cfg);
    }
    else
    {
        fprintf( stderr, "%s: invalid option [%s]\n", g_PgmName, pOptions->pOptName );
        return ARCHER_INVALID_OPTION;
    }

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to set ARCHER Debug Level\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_wol_handler
 * Description  : Processes the ARCHER WOL enter command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_wol_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet = ARCHER_INVALID_OPTION;

    if ( nNumOptions == 1 )
    {
        if (!strcmp( pOptions->pOptName, "--int" ))
        {
            if (pOptions->nNumParms != 1)
            {
                fprintf( stderr, "%s must specify the debug parameter\n", g_PgmName );
            }
            else
            {
                char intf_name[16];;

                memcpy( intf_name, pOptions->pParms[0], 16 );

                nRet = archer_cmd_send (ARCHER_IOC_WOL, (unsigned long)intf_name);
            }
        }
    }
    else
    {
        fprintf( stderr, "%s failed to enter wol mode\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_dpi_handler
 * Description  : Processes the ARCHER dpi command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_dpi_handler(POPTION_INFO pOptions, int nNumOptions)
{
    archer_dpi_cmd_t dpi_cmd = ARCHER_DPI_CMD_MAX;
    int nRet;

    if ( nNumOptions == 0 )
    {
        fprintf( stderr, "%s: No options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( nNumOptions > 1 )
    {
        fprintf( stderr, "%s: Too many options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( !strcmp( pOptions->pOptName, "--enable" ) )
    {
        if ( pOptions->nNumParms )
        {
            fprintf( stderr, "%s: Too many parameters specified\n", g_PgmName );
            return ARCHER_INVALID_PARAMETER;
        }

        dpi_cmd = ARCHER_DPI_CMD_ENABLE;
    }
    else if ( !strcmp( pOptions->pOptName, "--disable" ) )
    {
        if ( pOptions->nNumParms )
        {
            fprintf( stderr, "%s: Too many parameters specified\n", g_PgmName );
            return ARCHER_INVALID_PARAMETER;
        }

        dpi_cmd = ARCHER_DPI_CMD_DISABLE;
    }
    else if ( !strcmp( pOptions->pOptName, "--stats" ) )
    {
        if ( pOptions->nNumParms )
        {
            fprintf( stderr, "%s: Too many parameters specified\n", g_PgmName );
            return ARCHER_INVALID_PARAMETER;
        }

        dpi_cmd = ARCHER_DPI_CMD_STATS;
    }
    else
    {
        fprintf( stderr, "%s: invalid option [%s]\n", g_PgmName, pOptions->pOptName );
        return ARCHER_INVALID_OPTION;
    }

    nRet = archer_cmd_send( ARCHER_IOC_DPI, dpi_cmd );

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to set ARCHER Debug Level\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_xtmdropalg_handler
 * Description  : Processes the ARCHER xtmdrop_alg command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_xtmdropalg_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet;
    archer_dropalg_config_t dropAlg;

    if ( nNumOptions == 0 )
    {
        fprintf( stderr, "%s: No options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( nNumOptions > 1 )
    {
        fprintf( stderr, "%s: Too many options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( !strcmp( pOptions->pOptName, "--get" ) )
    {
        dropAlg.tx_idx = atoi(pOptions->pParms[0]);

        fprintf(stderr, "getting drop alg cfg for queue %s convert %d\n", 
            pOptions->pParms[0], dropAlg.tx_idx);

        nRet = archer_cmd_send( ARCHER_IOC_XTMDROPALG_GET, (unsigned long)&dropAlg );

        fprintf(stderr, "drop Alg return alg %d lo (%d %d %d) hi (%d %d %d) mask (0x%x 0x%x)\n",
            dropAlg.alg, 
            dropAlg.thres_lo.dropProb, dropAlg.thres_lo.minThres, dropAlg.thres_lo.maxThres,
            dropAlg.thres_hi.dropProb, dropAlg.thres_hi.minThres, dropAlg.thres_hi.maxThres,
            dropAlg.priorityMask_0, dropAlg.priorityMask_1);
    }
    else if ( !strcmp( pOptions->pOptName, "--set" ) )
    {
        int arg_idx = 0;

        memset (&dropAlg, 0, sizeof (archer_dropalg_config_t));

        while (arg_idx < pOptions->nNumParms)
        {
            if (!strcmp (pOptions->pParms[arg_idx], "--qid"))
            {
                dropAlg.tx_idx = atoi(pOptions->pParms[arg_idx+1]);
                arg_idx += 2;
            }
            else if (!strcmp (pOptions->pParms[arg_idx], "--alg"))
            {
                dropAlg.alg = atoi(pOptions->pParms[arg_idx+1]);
                arg_idx += 2;
            }
            else if (!strcmp (pOptions->pParms[arg_idx], "--lo"))
            {
                dropAlg.thres_lo.dropProb = atoi(pOptions->pParms[arg_idx+1]);
                dropAlg.thres_lo.minThres = atoi(pOptions->pParms[arg_idx+2]);
                dropAlg.thres_lo.maxThres = atoi(pOptions->pParms[arg_idx+3]);
                arg_idx += 4;
            }
            else if (!strcmp (pOptions->pParms[arg_idx], "--hi"))
            {
                dropAlg.thres_hi.dropProb = atoi(pOptions->pParms[arg_idx+1]);
                dropAlg.thres_hi.minThres = atoi(pOptions->pParms[arg_idx+2]);
                dropAlg.thres_hi.maxThres = atoi(pOptions->pParms[arg_idx+3]);
                arg_idx += 4;
            }
            else if (!strcmp (pOptions->pParms[arg_idx], "--mask"))
            {
                dropAlg.priorityMask_0 = strtoul(pOptions->pParms[arg_idx+1], NULL, 16);
                dropAlg.priorityMask_1 = strtoul(pOptions->pParms[arg_idx+2], NULL, 16);
                arg_idx += 3;
            } 
            else
            {
                fprintf( stderr, "%s: invalid parameter for xtmdrop_alg --set [%s]\n", g_PgmName, pOptions->pParms[arg_idx] );
                return ARCHER_INVALID_OPTION;
            }
        }
        fprintf(stderr, "attempting to set drop Alg for queue %d alg %d lo (%d %d %d) hi (%d %d %d) mask (0x%x 0x%x)\n",
            dropAlg.tx_idx, dropAlg.alg, 
            dropAlg.thres_lo.dropProb, dropAlg.thres_lo.minThres, dropAlg.thres_lo.maxThres,
            dropAlg.thres_hi.dropProb, dropAlg.thres_hi.minThres, dropAlg.thres_hi.maxThres,
            dropAlg.priorityMask_0, dropAlg.priorityMask_1);

        nRet = archer_cmd_send( ARCHER_IOC_XTMDROPALG_SET, (unsigned long)&dropAlg );
    }
    else
    {
        fprintf( stderr, "%s: invalid option [%s]\n", g_PgmName, pOptions->pOptName );
        return ARCHER_INVALID_OPTION;
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_debug_handler
 * Description  : Processes the ARCHER debug command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_debug_handler(POPTION_INFO pOptions, int nNumOptions)
{
    int nRet;
    int log;

    if ( nNumOptions == 0 )
    {
        fprintf( stderr, "%s: No options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( nNumOptions > 1 )
    {
        fprintf( stderr, "%s: Too many options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( !strcmp( pOptions->pOptName, "--log" ) )
    {
        if ( pOptions->nNumParms != 1 )
        {
            fprintf( stderr, "%s: must specify the Debug log level\n", g_PgmName );
            return ARCHER_INVALID_OPTION;
        }

        log = atoi( pOptions->pParms[0] );

        nRet = archer_cmd_send( ARCHER_IOC_DEBUG, log);
    }
    else
    {
        fprintf( stderr, "%s: invalid option [%s]\n", g_PgmName, pOptions->pOptName );
        return ARCHER_INVALID_OPTION;
    }

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to set ARCHER Debug Level\n", g_PgmName );
    }

    return nRet;
}

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_sysport_handler
 * Description  : Processes the archer sysport command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int archer_sysport_handler(POPTION_INFO pOptions, int nNumOptions)
{
    archer_sysport_cmd_t sysport_cmd = ARCHER_SYSPORT_CMD_MAX;
    int nRet;

    if ( nNumOptions == 0 )
    {
        fprintf( stderr, "%s: No options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( nNumOptions > 1 )
    {
        fprintf( stderr, "%s: Too many options specified\n", g_PgmName );
        return ARCHER_INVALID_OPTION;
    }
    else if ( !strcmp( pOptions->pOptName, "--reg" ) )
    {
        if ( pOptions->nNumParms )
        {
            fprintf( stderr, "%s: Too many parameters specified\n", g_PgmName );
            return ARCHER_INVALID_PARAMETER;
        }

        sysport_cmd = ARCHER_SYSPORT_CMD_REG_DUMP;
    }
    else if ( !strcmp( pOptions->pOptName, "--port" ) )
    {
        if ( pOptions->nNumParms )
        {
            fprintf( stderr, "%s: Too many parameters specified\n", g_PgmName );
            return ARCHER_INVALID_PARAMETER;
        }

        sysport_cmd = ARCHER_SYSPORT_CMD_PORT_DUMP;
    }
    else
    {
        fprintf( stderr, "%s: invalid option [%s]\n", g_PgmName, pOptions->pOptName );
        return ARCHER_INVALID_OPTION;
    }

    nRet = archer_cmd_send( ARCHER_IOC_SYSPORT, sysport_cmd );

    if ( nRet != ARCHER_SUCCESS )
    {
        fprintf( stderr, "%s: failed to send ARCHER Sysport command\n", g_PgmName );
    }

    return nRet;
}

/*** File Scoped Globals. ***/
COMMAND_INFO g_Cmds[] =
{
    {"enable",            {""},                    archer_enable_handler},
    {"bind",              {""},                    archer_bind_handler},
    {"disable",           {""},                    archer_disable_handler},
    {"unbind",            {""},                    archer_unbind_handler},
    {"status",            {""},                    archer_status_handler},
    {"flows",             {"--max", "--all", ""},  archer_flows_handler},
    {"ucast_l3",          {"--max", "--all", ""},  archer_ucast_l3_handler},
    {"ucast_l2",          {"--max", "--all", ""},  archer_ucast_l2_handler},
    {"mcast",             {"--max", "--all", ""},  archer_mcast_handler},
    {"host",              {""},                    archer_host_handler},
    {"stats",             {""},                    archer_stats_handler},
    {"mode",              {"--l3", "--l2+l3", ""}, archer_mode_handler},
    {"mpd_cfg",           {"--int", "", ""},       archer_mpdcfg_handler},
    {"wol_enter",         {"--int", ""},           archer_wol_handler},
    {"dpi",               {"--enable", "--disable", "--stats", ""}, archer_dpi_handler},
    {"xtmdrop_alg",       {"--get", "--set", ""},  archer_xtmdropalg_handler},
    {"debug",             {"--log", ""},           archer_debug_handler},
    {"sysport",           {"--reg", "--port", ""}, archer_sysport_handler},
    {""}
};

/*
 *------------------------------------------------------------------------------
 * Function Name: archer_help
 * Description  : Displays the BCM Archer CLI help
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
void archer_help(void)
{
    printf(
        ARCHER_MODNAME " Control Utility:\n\n"

        "::::::  Usage  ::::::\n\n"

        ":::: System :::::\n\n"
        "     archerctl status\n"
        "     archerctl flows    ( --max <nbr_of_flows>, --all )\n"
        "     archerctl ucast_l3 ( --max <nbr_of_flows>, --all )\n"
        "     archerctl ucast_l2 ( --max <nbr_of_flows>, --all )\n"
        "     archerctl mcast    ( --max <nbr_of_flows>, --all )\n"
        "     archerctl host\n"
        "     archerctl stats\n"
        "     archerctl mpd_cfg [ --int <interface> <mac_addr> ]\n"
        "     archerctl wol_enter [ --int <interface> ]\n\n"

        ":::: Debug :::::\n\n"
        "     archerctl debug [ --log <log_level> ]\n"
        "     archerctl mode [ --l3 | --l2+l3 ]\n"
        "     archerctl dpi [ --enable, --disable, --stats ]\n"
        "     archerctl xtmdrop_alg [ --get <q_idx>, --set <params> ] \n"
        "                       set parameters: --qid, --alg, --lo, --hi, --mask\n"
        "     archerctl sysport [ --reg | --port ]\n\n"
        );

    return;
} /* Usage */

/*
 *------------------------------------------------------------------------------
 * Function Name: GetArgType
 * Description  : Determines if the specified command line argument is a
 *                command, option or option parameter.
 * Returns      : ARG_TYPE_COMMAND, ARG_TYPE_OPTION, ARG_TYPE_PARAMETER
 *------------------------------------------------------------------------------
 */
int GetArgType(char *pszArg, PCOMMAND_INFO pCmds, char **ppszOptions)
{
    int nArgType = ARG_TYPE_PARAMETER;

    if ( ppszOptions )  /* See if the argument is a option. */
    {
        do
        {
            if ( !strcmp( pszArg, *ppszOptions ) )
            {
                nArgType = ARG_TYPE_OPTION;
                break;
            }
        } while ( *++ppszOptions );
    }

    /* Next, see if the argument is an command. */
    if ( nArgType == ARG_TYPE_PARAMETER )
    {
        while ( pCmds->szCmdName[0] != '\0' )
        {
            if ( !strcmp( pszArg, pCmds->szCmdName ) )
            {
                nArgType = ARG_TYPE_COMMAND;
                break;
            }
            pCmds++;
        }
    }

    return nArgType;    /* Otherwise, assume that it is a parameter. */
} /* GetArgType */

/*
 *------------------------------------------------------------------------------
 * Function Name: GetCommand
 * Description  : Returns the COMMAND_INFO structure for the specified
 *                command name.
 * Returns      : COMMAND_INFOR structure pointer
 *------------------------------------------------------------------------------
 */
PCOMMAND_INFO GetCommand(char *pszArg, PCOMMAND_INFO pCmds)
{
    PCOMMAND_INFO pCmd = NULL;

    while ( pCmds->szCmdName[0] != '\0' )
    {
        if ( !strcmp( pszArg, pCmds->szCmdName ) )
        {
            pCmd = pCmds;
            break;
        }
        pCmds++;
    }

    return pCmd;
} /* GetCommand */


/*
 *------------------------------------------------------------------------------
 * Function Name: ProcessCommand
 * Description  : Gets the options and option paramters for a command and
 *                calls the command handler function to process the command.
 * Returns      : 0 - success, non-0 - error
 *------------------------------------------------------------------------------
 */
int ProcessCommand(PCOMMAND_INFO pCmd, int argc, char **argv,
                   PCOMMAND_INFO pCmds, int *pnArgNext)
{
    int nRet = ARCHER_SUCCESS, nNumOptInfo = 0, nArgType = 0;
    OPTION_INFO OptInfo[MAX_OPTS], *pCurrOpt = NULL;

    memset( OptInfo, 0x00, sizeof(OptInfo) );
    *pnArgNext = 0;

    do
    {
        if ( argc == 0 ) break;

        nArgType = GetArgType( *argv, pCmds, pCmd->pszOptionNames );
        switch ( nArgType )
        {
            case ARG_TYPE_OPTION:
                if ( nNumOptInfo < MAX_OPTS )
                {
                    pCurrOpt = &OptInfo[nNumOptInfo++];
                    pCurrOpt->pOptName = *argv;
                }
                else
                {
                    nRet = ARCHER_INVALID_NUMBER_OF_OPTIONS;
                    fprintf( stderr, "%s: too many options\n", g_PgmName );
                }
                (*pnArgNext)++;
                break;

            case ARG_TYPE_PARAMETER:
                if ( pCurrOpt && pCurrOpt->nNumParms < MAX_PARMS )
                    pCurrOpt->pParms[pCurrOpt->nNumParms++] = *argv;
                else
                {
                    if ( pCurrOpt )
                    {
                        nRet = ARCHER_INVALID_OPTION;
                        fprintf( stderr, "%s: invalid option\n", g_PgmName );
                    }
                    else
                    {
                        nRet = ARCHER_INVALID_NUMBER_OF_OPTIONS;
                        fprintf( stderr, "%s: too many options\n", g_PgmName );
                    }
                }
                (*pnArgNext)++;
                break;

            case ARG_TYPE_COMMAND:
                /* The current command is done. */
                break;
        } /* switch ( nArgType ) */

        argc--, argv++;

    } while ( (nRet == ARCHER_SUCCESS) && (nArgType!=ARG_TYPE_COMMAND) );

    if ( nRet == ARCHER_SUCCESS )
        nRet = (*pCmd->pfnCmdHandler)( OptInfo, nNumOptInfo );

    return nRet;
} /* ProcessCommand */

/*
 *------------------------------------------------------------------------------
 * Function Name: main
 * Description  : Main program function.
 * Returns      : 0 - success, non-0 - error.
 *------------------------------------------------------------------------------
 */
#ifdef BUILD_STATIC
int archerctl_main(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
    int nExitCode = 0;
    PCOMMAND_INFO pCmd;
    /* Save the name that started this program into a global variable. */
    strcpy( g_PgmName, *argv );
    if ( argc == 1 )
    {
        archer_help( );
        exit( nExitCode );
    }
    argc--, argv++;
    while ( argc && nExitCode == 0 )
    {
        if ( GetArgType( *argv, g_Cmds, NULL ) == ARG_TYPE_COMMAND )
        {
            int argnext = 0;
            pCmd = GetCommand( *argv, g_Cmds );
            argc--; argv++;
            nExitCode = ProcessCommand( pCmd, argc, argv, g_Cmds, &argnext );
            argc -= argnext;
            argv += argnext;
        }
        else
        {
            nExitCode = ARCHER_INVALID_COMMAND;
            fprintf( stderr, "%s: invalid command\n", g_PgmName );
        }
    }
    exit( nExitCode );
}
