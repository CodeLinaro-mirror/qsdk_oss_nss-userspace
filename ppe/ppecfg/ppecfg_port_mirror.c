/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * @file PPECFG port-mirror handler
 *
 * Implements the "family=port_mirror" CLI family for ppecfg.
 *
 * Supported commands
 * ------------------
 *
 *   ppecfg family=port_mirror cmd=mirror_enable
 *          port_name=<ifname> direction=<ingress|egress>
 *
 *   ppecfg family=port_mirror cmd=mirror_disable
 *          port_name=<ifname> direction=<ingress|egress>
 *
 *   ppecfg family=port_mirror cmd=analyst_port_set
 *          analyst_port_name=<ifname> direction=<ingress|egress>
 *          [priority=<0-255>]
 *
 *   ppecfg family=port_mirror cmd=analyst_port_clr
 *          analyst_port_name=<ifname> direction=<ingress|egress>
 */

#include "ppecfg_hlos.h"
#include <nss_ppenl_base.h>
#include <nss_ppenl_port_mirror_if.h>
#include <nss_ppenl_port_mirror_api.h>
#include "ppecfg_param.h"
#include "ppecfg_port_mirror.h"

/*
 * Forward declarations
 */
static int ppecfg_port_mirror_cfg_enable(struct ppecfg_param *param,
					 struct ppecfg_param_in *match);
static int ppecfg_port_mirror_cfg_disable(struct ppecfg_param *param,
					  struct ppecfg_param_in *match);
static int ppecfg_port_mirror_analyst_set(struct ppecfg_param *param,
					  struct ppecfg_param_in *match);
static int ppecfg_port_mirror_analyst_clr(struct ppecfg_param *param,
					  struct ppecfg_param_in *match);

/*
 * Sub-parameter tables
 */

/*
 * cmd=mirror_enable / cmd=mirror_disable sub-parameters.
 */
static struct ppecfg_param port_mirror_cfg_params[PPECFG_PORT_MIRROR_CFG_MAX] = {
	PPECFG_PARAM_INIT(PPECFG_PORT_MIRROR_CFG_PORT_NAME, "port_name="),
	PPECFG_PARAM_INIT(PPECFG_PORT_MIRROR_CFG_DIR,       "direction="),
};

/*
 * cmd=analyst_port_set / cmd=analyst_port_clr sub-parameters.
 */
static struct ppecfg_param port_mirror_analyst_params[PPECFG_PORT_MIRROR_ANALYST_MAX] = {
	PPECFG_PARAM_INIT(PPECFG_PORT_MIRROR_ANALYST_PORT_NAME, "analyst_port_name="),
	PPECFG_PARAM_INIT(PPECFG_PORT_MIRROR_ANALYST_DIR,       "direction="),
	PPECFG_PARAM_INIT(PPECFG_PORT_MIRROR_ANALYST_PRIORITY,  "priority="),
};

/*
 * Top-level command table.
 * NOTE: whenever this table is updated, enum ppecfg_port_mirror_cmd must also
 * be updated to keep the indices in sync.
 */
struct ppecfg_param ppecfg_port_mirror_params[PPECFG_PORT_MIRROR_CMD_MAX] = {
	PPECFG_PARAMLIST_INIT("cmd=mirror_enable",          port_mirror_cfg_params,
			      ppecfg_port_mirror_cfg_enable),
	PPECFG_PARAMLIST_INIT("cmd=mirror_disable",         port_mirror_cfg_params,
			      ppecfg_port_mirror_cfg_disable),
	PPECFG_PARAMLIST_INIT("cmd=analyst_port_set", port_mirror_analyst_params,
			      ppecfg_port_mirror_analyst_set),
	PPECFG_PARAMLIST_INIT("cmd=analyst_port_clr", port_mirror_analyst_params,
			      ppecfg_port_mirror_analyst_clr),
};

/*
 * ppecfg_port_mirror_parse_dir()
 *	Parse a direction string into the corresponding enum value.
 *
 * @param dir_str[IN]   Null-terminated direction string ("ingress" or "egress").
 * @param dir_out[OUT]  Receives the parsed nss_ppe_port_mirror_dir value.
 *
 * @return 0 on success, -EINVAL if the string is not recognised.
 */
static int ppecfg_port_mirror_parse_dir(const char *dir_str,
					uint8_t *dir_out)
{
	if (!strcmp(dir_str, "ingress")) {
		*dir_out = NSS_PPE_PORT_MIRROR_DIR_INGRESS;
		return 0;
	}

	if (!strcmp(dir_str, "egress")) {
		*dir_out = NSS_PPE_PORT_MIRROR_DIR_EGRESS;
		return 0;
	}

	ppecfg_log_warn("Invalid direction '%s': use ingress|egress\n", dir_str);
	return -EINVAL;
}

/*
 * ppecfg_port_mirror_cfg_common()
 *	Common handler for cmd=mirror_enable and cmd=mirror_disable.
 *
 * Parses port_name= and direction= from the CLI, builds the Netlink message,
 * and calls nss_ppenl_port_mirror_cfg().
 */
static int ppecfg_port_mirror_cfg_common(struct ppecfg_param *param,
					 struct ppecfg_param_in *match,
					 bool mirror_en)
{
	struct nss_ppenl_port_mirror_rule nl_msg = {{0}};
	struct ppecfg_param *sub_params;
	char dir_str[16];
	int error;

	if (!param || !match) {
		ppecfg_log_warn("Param or match table is NULL\n");
		return -EINVAL;
	}

	error = ppecfg_param_iter_tbl(param, match);
	if (error) {
		ppecfg_log_arg_error(param);
		return error;
	}

	nss_ppenl_port_mirror_rule_init(&nl_msg,
					mirror_en ? NSS_PPE_PORT_MIRROR_ENABLE_MSG
						  : NSS_PPE_PORT_MIRROR_DISABLE_MSG);

	/*
	 * Extract port_name.
	 */
	sub_params = &param->sub_params[PPECFG_PORT_MIRROR_CFG_PORT_NAME];
	error = ppecfg_param_get_str(sub_params->data, IFNAMSIZ,
				     nl_msg.cfg.port_name);
	if (error) {
		ppecfg_log_arg_error(sub_params);
		return error;
	}

	/*
	 * Extract and validate direction.
	 */
	sub_params = &param->sub_params[PPECFG_PORT_MIRROR_CFG_DIR];
	error = ppecfg_param_get_str(sub_params->data, sizeof(dir_str), dir_str);
	if (error) {
		ppecfg_log_arg_error(sub_params);
		return error;
	}

	error = ppecfg_port_mirror_parse_dir(dir_str, &nl_msg.cfg.dir);
	if (error) {
		return error;
	}


	error = nss_ppenl_port_mirror_cfg(&nl_msg);
	if (error < 0) {
		ppecfg_log_warn("Unable to send port-mirror cfg message\n");
		return error;
	}

	ppecfg_log_info("port-mirror %s: port=%s direction=%s\n",
			mirror_en ? "enabled" : "disabled",
			nl_msg.cfg.port_name, dir_str);
	return 0;
}

/*
 * ppecfg_port_mirror_cfg_enable()
 *	Handle cmd=mirror_enable – enable ingress or egress mirroring on a port.
 */
static int ppecfg_port_mirror_cfg_enable(struct ppecfg_param *param,
					 struct ppecfg_param_in *match)
{
	return ppecfg_port_mirror_cfg_common(param, match, true);
}

/*
 * ppecfg_port_mirror_cfg_disable()
 *	Handle cmd=mirror_disable – disable ingress or egress mirroring on a port.
 */
static int ppecfg_port_mirror_cfg_disable(struct ppecfg_param *param,
					  struct ppecfg_param_in *match)
{
	return ppecfg_port_mirror_cfg_common(param, match, false);
}

/*
 * ppecfg_port_mirror_analyst_common()
 *	Common handler for cmd=analyst_port_set and cmd=analyst_port_clr.
 *
 * Parses analyst_port_name=, direction=, and (optionally) priority= from the
 * CLI, builds the Netlink message, and calls the appropriate API.
 */
static int ppecfg_port_mirror_analyst_common(struct ppecfg_param *param,
					     struct ppecfg_param_in *match,
					     bool enable)
{
	struct nss_ppenl_port_mirror_rule nl_msg = {{0}};
	struct ppecfg_param *sub_params;
	char dir_str[16];
	int error;

	if (!param || !match) {
		ppecfg_log_warn("Param or match table is NULL\n");
		return -EINVAL;
	}

	error = ppecfg_param_iter_tbl(param, match);
	if (error) {
		ppecfg_log_arg_error(param);
		return error;
	}

	nss_ppenl_port_mirror_rule_init(&nl_msg,
					enable ? NSS_PPE_PORT_MIRROR_SET_ANALYST_PORT_MSG
					       : NSS_PPE_PORT_MIRROR_CLR_ANALYST_PORT_MSG);

	/*
	 * Extract analyst_port_name.
	 */
	sub_params = &param->sub_params[PPECFG_PORT_MIRROR_ANALYST_PORT_NAME];
	error = ppecfg_param_get_str(sub_params->data, IFNAMSIZ,
				     nl_msg.analyst.analyst_port_name);
	if (error) {
		ppecfg_log_arg_error(sub_params);
		return error;
	}

	/*
	 * Extract and validate direction.
	 */
	sub_params = &param->sub_params[PPECFG_PORT_MIRROR_ANALYST_DIR];
	error = ppecfg_param_get_str(sub_params->data, sizeof(dir_str), dir_str);
	if (error) {
		ppecfg_log_arg_error(sub_params);
		return error;
	}

	error = ppecfg_port_mirror_parse_dir(dir_str, &nl_msg.analyst.dir);
	if (error) {
		return error;
	}

	/*
	 * Extract optional priority (only meaningful for set; ignored on clear).
	 */
	sub_params = &param->sub_params[PPECFG_PORT_MIRROR_ANALYST_PRIORITY];
	if (sub_params->valid) {
		uint8_t priority = 0;

		error = ppecfg_param_get_int(sub_params->data, sizeof(uint8_t),
					     &priority);
		if (error) {
			ppecfg_log_arg_error(sub_params);
			return error;
		}

		nl_msg.analyst.priority = priority;
	}

	if (enable) {
		error = nss_ppenl_port_mirror_analyst_set(&nl_msg);
	} else {
		error = nss_ppenl_port_mirror_analyst_clr(&nl_msg);
	}

	if (error < 0) {
		ppecfg_log_warn("Unable to send analyst_port_%s message\n",
				enable ? "set" : "clr");
		return error;
	}

	ppecfg_log_info("analyst_port_%s: analyst_port=%s direction=%s priority=%u\n",
			enable ? "set" : "clr",
			nl_msg.analyst.analyst_port_name,
			dir_str,
			nl_msg.analyst.priority);
	return 0;
}

/*
 * ppecfg_port_mirror_analyst_set()
 *	Handle cmd=analyst_port_set – configure the mirror analysis port.
 */
static int ppecfg_port_mirror_analyst_set(struct ppecfg_param *param,
					  struct ppecfg_param_in *match)
{
	return ppecfg_port_mirror_analyst_common(param, match, true);
}

/*
 * ppecfg_port_mirror_analyst_clr()
 *	Handle cmd=analyst_port_clr – clear the mirror analysis port.
 */
static int ppecfg_port_mirror_analyst_clr(struct ppecfg_param *param,
					  struct ppecfg_param_in *match)
{
	return ppecfg_port_mirror_analyst_common(param, match, false);
}
