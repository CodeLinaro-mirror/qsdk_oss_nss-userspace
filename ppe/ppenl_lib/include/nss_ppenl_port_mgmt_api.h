/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef __NSS_PPENL_PORT_MGMT_API_H__
#define __NSS_PPENL_PORT_MGMT_API_H__

#include <ppe_port_mgmt.h>

/**
 * nss_ppenl_port_mgmt_port_isolation_set()
 *	Set the PORT_MGMT port isolation
 *
 * @param[in] port_name		Port name.
 * @param[in] omci_id		OMCI ID.
 * @param[out] isol_info	PPE port isolation information (return value).
 *
 * @return
 * 0 on success or negative on failure.
 */
int nss_ppenl_port_mgmt_port_isolation_set(struct nss_ppenl_port_mgmt_info *port_mgmt_info);

/**
 * nss_ppenl_port_mgmt_act_ctrl_set()
 *	Set the PORT_MGMT action control configs
 *
 * @param[in] isol_info		PPE port isolation information.
 *
 * @return
 * 0 on success or negative on failure.
 */
int nss_ppenl_port_mgmt_act_ctrl_set(struct nss_ppenl_port_mgmt_info *port_mgmt_info);

/**
 * nss_ppenl_port_mgmt_isol_default()
 *	Set the PORT_MGMT default isol configs
 *
 * @param[in] isol_info		PPE port isolation information.
 *
 * @return
 * 0 on success or negative on failure.
 */
int nss_ppenl_port_mgmt_isol_default(struct nss_ppenl_port_mgmt_info *port_mgmt_info);

#endif /* __NSS_PPENL_PORT_MGMT_API_H__ */
