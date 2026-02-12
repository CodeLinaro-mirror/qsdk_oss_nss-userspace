/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef __PPECFG_FAMILY_H
#define __PPECFG_FAMILY_H

#include "ppecfg_acl.h"
#include "ppecfg_policer.h"
#include "ppecfg_qos.h"
#include "ppecfg_exception.h"
#ifdef NSS_PPE_DSCP_FEATURE
#include "ppecfg_dscp.h"
#endif
#ifdef NSS_PPE_PM_FEATURE
#include "ppecfg_pm.h"
#endif
#ifdef NSS_PPE_EXT_VLAN_FEATURE
#include "ppecfg_vlan.h"
#endif
#ifdef NSS_PPE_PORT_MGMT_FEATURE
#include "ppecfg_port_mgmt.h"
#endif
#include <nss_ppenl_base.h>

/*
 * Family match params
 */
extern struct ppecfg_param ppecfg_acl_params[PPECFG_ACL_CMD_MAX];
extern struct ppecfg_param ppecfg_policer_params[PPECFG_POLICER_CMD_MAX];
extern struct ppecfg_param ppecfg_qos_params[PPECFG_QOS_CMD_MAX];
extern struct ppecfg_param ppecfg_exception_params[PPECFG_EXCEPTION_PARAM_MAX];
#ifdef NSS_PPE_DSCP_FEATURE
extern struct ppecfg_param ppecfg_dscp_params[PPECFG_DSCP_CMD_MAX];
#endif
#ifdef NSS_PPE_PM_FEATURE
extern struct ppecfg_param ppecfg_pm_params[PPECFG_PM_CMD_MAX];
#endif
#ifdef NSS_PPE_EXT_VLAN_FEATURE
extern struct ppecfg_param ppecfg_vlan_params[PPECFG_VLAN_CMD_MAX];
#endif
#ifdef NSS_PPE_PORT_MGMT_FEATURE
extern struct ppecfg_param ppecfg_port_mgmt_params[PPECFG_PORT_MGMT_CMD_MAX];
#endif

#endif /* __PPECFG_FAMILY_H*/
