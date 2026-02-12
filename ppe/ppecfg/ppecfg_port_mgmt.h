/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef __PPECFG_PORT_MGMT_H
#define __PPECFG_PORT_MGMT_H

#define PPECFG_PORT_MGMT_HDR_VERSION 4

/*
 * PPECFG PORT_MGMT commands
 *	Top‑level command identifiers for Port Management operations.
 */
enum ppecfg_port_mgmt_cmd {
	PPECFG_PORT_MGMT_CMD_PORT_ISOL_SET,   /**< Set port isolation configuration */
	PPECFG_PORT_MGMT_CMD_PORT_ISOL_GET,   /**< Get port isolation configuration */
	PPECFG_PORT_MGMT_CMD_DEF_ISOL_SET,    /**< Set default isolation behavior */
	PPECFG_PORT_MGMT_CMD_MAX	      /**< Max command indicator */
};

/*
 * PPECFG PORT_MGMT port isolation set
 *	Attributes used when setting port isolation via Netlink.
 */
enum ppecfg_port_mgmt_port_isol_set {
	PPECFG_PORT_MGMT_PORT_ISOL_SET_PORT_NAME,   /**< Port name associated with the isolation rule */
	PPECFG_PORT_MGMT_PORT_ISOL_SET_OMCI_ID,     /**< OMCI ID for which isolation is configured */
	PPECFG_PORT_MGMT_PORT_ISOL_SET_MAX	    /**< Max attribute indicator */
};

/*
 * PPECFG PORT_MGMT action control set
 *	Attributes for enabling isolation actions during configuration.
 */
enum ppecfg_port_mgmt_act_ctrl_set {
	PPECFG_PORT_MGMT_ACT_CTRL_SET_MC_ISOL_EN,   /**< Enable/disable multicast isolation */
	PPECFG_PORT_MGMT_ACT_CTRL_SET_BC_ISOL_EN,   /**< Enable/disable broadcast isolation */
	PPECFG_PORT_MGMT_ACT_CTRL_SET_MAX	    /**< Max attribute indicator */
};
#endif /* __PPECFG_PORT_MGMT_H*/
