/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef __PPECFG_PORT_MIRROR_H
#define __PPECFG_PORT_MIRROR_H

/*
 * ppecfg_port_mirror_cmd
 *	Top-level command identifiers for port-mirror operations.
 *
 * cmd=mirror_enable
 *   Enable ingress or egress mirroring on a port.
 *   Required: port_name=<ifname> direction=<ingress|egress>
 *
 * cmd=mirror_disable
 *   Disable ingress or egress mirroring on a port.
 *   Required: port_name=<ifname> direction=<ingress|egress>
 *
 * cmd=analyst_port_set
 *   Set the mirror analysis (capture) port for a given direction.
 *   Required: analyst_port_name=<ifname> direction=<ingress|egress>
 *   Optional: priority=<0-255>  (default 0)
 *
 * cmd=analyst_port_clr
 *   Clear (disable) the mirror analysis port for a given direction.
 *   Required: analyst_port_name=<ifname> direction=<ingress|egress>
 */
enum ppecfg_port_mirror_cmd {
	PPECFG_PORT_MIRROR_CMD_EN,		/**< Enable mirroring on a port (cmd=mirror_enable) */
	PPECFG_PORT_MIRROR_CMD_DIS,		/**< Disable mirroring on a port (cmd=mirror_disable) */
	PPECFG_PORT_MIRROR_CMD_ANALYST_SET,	/**< Set the analyst / capture port */
	PPECFG_PORT_MIRROR_CMD_ANALYST_CLR,	/**< Clear the analyst / capture port */
	PPECFG_PORT_MIRROR_CMD_MAX
};

/*
 * ppecfg_port_mirror_cfg_params
 *	Sub-parameters for cmd=mirror_enable and cmd=mirror_disable.
 */
enum ppecfg_port_mirror_cfg_params {
	PPECFG_PORT_MIRROR_CFG_PORT_NAME,	/**< port_name=<ifname> */
	PPECFG_PORT_MIRROR_CFG_DIR,		/**< direction=<ingress|egress> */
	PPECFG_PORT_MIRROR_CFG_MAX
};

/*
 * ppecfg_port_mirror_analyst_params
 *	Sub-parameters for cmd=analyst_port_set and cmd=analyst_port_clr.
 */
enum ppecfg_port_mirror_analyst_params {
	PPECFG_PORT_MIRROR_ANALYST_PORT_NAME,	/**< analyst_port_name=<ifname> */
	PPECFG_PORT_MIRROR_ANALYST_DIR,		/**< direction=<ingress|egress> */
	PPECFG_PORT_MIRROR_ANALYST_PRIORITY,	/**< priority=<0-255> (optional, set only) */
	PPECFG_PORT_MIRROR_ANALYST_MAX
};

#endif /* __PPECFG_PORT_MIRROR_H */
