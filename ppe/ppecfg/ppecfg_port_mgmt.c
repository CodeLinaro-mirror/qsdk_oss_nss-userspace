/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * @file PPECFG PORT_MGMT handler
 */

#include "ppecfg_hlos.h"
#include <nss_ppenl_base.h>
#include <nss_ppenl_port_mgmt_if.h>
#include <nss_ppenl_port_mgmt_api.h>
#include <ppe_port_mgmt.h>
#include "ppecfg_param.h"
#include "ppecfg_port_mgmt.h"

#define MANDATORY_FIELDS_MASK 0x01
#define OPTIONAL_FIELDS_MASK  0x7E
#define PORT_FIELDS_MASK      0x06

static int ppecfg_port_mgmt_port_isolation_set(struct ppecfg_param *param, struct ppecfg_param_in *match);
static int ppecfg_port_mgmt_act_ctrl_set(struct ppecfg_param *param, struct ppecfg_param_in *match);
static int ppecfg_port_mgmt_isol_default(struct ppecfg_param *param, struct ppecfg_param_in *match);

/*
 * PORT_MGMT port isolation set parameters
 */
static struct ppecfg_param port_isol_set_params[PPECFG_PORT_MGMT_PORT_ISOL_SET_MAX] = {
	PPECFG_PARAM_INIT(PPECFG_PORT_MGMT_PORT_ISOL_SET_PORT_NAME, "port_name="),
	PPECFG_PARAM_INIT(PPECFG_PORT_MGMT_PORT_ISOL_SET_OMCI_ID, "omci_id="),
};

/*
 * PORT_MGMT port action control set parameters
 */
static struct ppecfg_param act_ctrl_set_params[PPECFG_PORT_MGMT_ACT_CTRL_SET_MAX] = {
	PPECFG_PARAM_INIT(PPECFG_PORT_MGMT_ACT_CTRL_SET_MC_ISOL_EN, "mc_isol_en="),
	PPECFG_PARAM_INIT(PPECFG_PORT_MGMT_ACT_CTRL_SET_BC_ISOL_EN, "bc_isol_en="),
};

/*
 * NOTE: whenever this table is updated, the 'enum ppecfg_port_mgmt_cmd' should also get updated
 */
struct ppecfg_param ppecfg_port_mgmt_params[PPECFG_PORT_MGMT_CMD_MAX] = {
	PPECFG_PARAMLIST_INIT("cmd=port_isol_set", port_isol_set_params, ppecfg_port_mgmt_port_isolation_set),
	PPECFG_PARAMLIST_INIT("cmd=act_ctrl_set", act_ctrl_set_params, ppecfg_port_mgmt_act_ctrl_set),
	PPECFG_PARAMFUNC_INIT("cmd=isol_default", ppecfg_port_mgmt_isol_default),
};

/*
 * ppecfg_port_mgmt_port_isolation_set()
 *	Handle PORT_MGMT port isolation set
 */
static int ppecfg_port_mgmt_port_isolation_set(struct ppecfg_param *param, struct ppecfg_param_in *match)
{
	struct nss_ppenl_port_mgmt_info nl_msg = {{0}};
	struct ppecfg_param *sub_params;
	int error;

	if (!param || !match) {
		ppecfg_log_warn("Param or match table is NULL \n");
		return -EINVAL;
	}

	/*
	 * iterate through the param table to identify the matched arguments and
	 * populate the argument list
	 */
	error = ppecfg_param_iter_tbl(param, match);
	if (error) {
		ppecfg_log_arg_error(param);
		goto done;
	}

	/*
	 * Initialize the PORT_MGMT message
	 */
	nss_ppenl_rule_port_mgmt_init(&nl_msg, NSS_PPE_PORT_MGMT_PORT_ISOL_SET_MSG);

	/*
	 * extract port name
	 */
	sub_params = &param->sub_params[PPECFG_PORT_MGMT_PORT_ISOL_SET_PORT_NAME];
	error = ppecfg_param_get_str(sub_params->data, IFNAMSIZ, &nl_msg.isol.port_name);
	if (error) {
		ppecfg_log_arg_error(sub_params);
		goto done;
	}

	/*
	 * extract OMCI ID
	 */
	sub_params = &param->sub_params[PPECFG_PORT_MGMT_PORT_ISOL_SET_OMCI_ID];
	error = ppecfg_param_get_int(sub_params->data, sizeof(uint32_t), &nl_msg.isol.omci_id);
	if (error) {
		ppecfg_log_arg_error(sub_params);
		goto done;
	}

	/*
	 * send message
	 */
	error = nss_ppenl_port_mgmt_port_isolation_set(&nl_msg);
	if (error < 0) {
		ppecfg_log_warn("Unable to send message\n");
		goto done;
	}
	ppecfg_log_info("Configured successfully, Port_name: %s Omci_ID: %d\n", nl_msg.isol.port_name, nl_msg.isol.omci_id);
done:
	return error;
}

/*
 * ppecfg_port_mgmt_act_ctrl_set()
 *	Handle PORT_MGMT port action control set
 */
static int ppecfg_port_mgmt_act_ctrl_set(struct ppecfg_param *param, struct ppecfg_param_in *match)
{
	struct nss_ppenl_port_mgmt_info nl_msg = {{0}};
	int error;

	if (!param || !match) {
		ppecfg_log_warn("Param or match table is NULL \n");
		return -EINVAL;
	}

	/*
	 * iterate through the param table to identify the matched arguments and
	 * populate the argument list
	 */
	error = ppecfg_param_iter_tbl(param, match);
	if (error) {
		ppecfg_log_arg_error(param);
		goto done;
	}

	/*
	 * Initialize the PORT_MGMT message
	 */
	nss_ppenl_rule_port_mgmt_init(&nl_msg, NSS_PPE_PORT_MGMT_ACT_CTRL_SET_MSG);

	/*
	 * extract MC Isolation Enable
	 */
	struct ppecfg_param *sub_params = &param->sub_params[PPECFG_PORT_MGMT_ACT_CTRL_SET_MC_ISOL_EN];
	if (sub_params) {
		error = ppecfg_param_get_bool(sub_params->data, &nl_msg.isol.mc_isol_en);
		if (error) {
			ppecfg_log_arg_error(sub_params);
			goto done;
		}
	}

	/*
	 * extract BC Isolation Enable
	 */
	sub_params = &param->sub_params[PPECFG_PORT_MGMT_ACT_CTRL_SET_BC_ISOL_EN];
	if (sub_params) {
		error = ppecfg_param_get_bool(sub_params->data, &nl_msg.isol.bc_isol_en);
		if (error) {
			ppecfg_log_arg_error(sub_params);
			goto done;
		}
	}

	/*
	 * send message
	 */
	error = nss_ppenl_port_mgmt_act_ctrl_set(&nl_msg);
	if (error < 0) {
		ppecfg_log_warn("Unable to send message\n");
		goto done;
	}

	ppecfg_log_info("Configured successfully, BC_ISOL_EN: %d MC_ISOL_EN: %d\n", nl_msg.isol.bc_isol_en, nl_msg.isol.mc_isol_en);
done:
	return error;
}

static int ppecfg_port_mgmt_isol_default(struct ppecfg_param *param, struct ppecfg_param_in *match)
{
	struct nss_ppenl_port_mgmt_info nl_msg = {{0}};
	int error;

	/*
	 * Initialize the PORT_MGMT message
	 */
	nss_ppenl_rule_port_mgmt_init(&nl_msg, NSS_PPE_PORT_MGMT_PORT_ISOL_DEF_MSG);

	/*
	 * send message
	 */
	error = nss_ppenl_port_mgmt_isol_default(&nl_msg);
	if (error < 0) {
		ppecfg_log_warn("Unable to send message\n");
		goto done;
	}

	ppecfg_log_info("Isol is set to default successfully\n");
done:
	return error;
}
