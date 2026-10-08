/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef __NSS_PPENL_PORT_MIRROR_API_H__
#define __NSS_PPENL_PORT_MIRROR_API_H__

#include <nss_ppenl_port_mirror_if.h>

/**
 * nss_ppenl_port_mirror_cfg()
 *	Enable or disable ingress / egress L2 VP mirroring on a named port.
 *
 * @param[in] rule  Pointer to the port-mirror rule message.  The caller must
 *                  fill in rule->cfg.port_name, rule->cfg.dir, and
 *                  rule->cfg.mirror_en before calling this function.
 *
 * @return 0 on success or negative errno on failure.
 */
int nss_ppenl_port_mirror_cfg(struct nss_ppenl_port_mirror_rule *rule);

/**
 * nss_ppenl_port_mirror_analyst_set()
 *	Set the mirror analysis (capture) port for a given direction.
 *
 * @param[in] rule  Pointer to the port-mirror rule message.  The caller must
 *                  fill in rule->analyst.analyst_port_name, rule->analyst.dir,
 *                  and rule->analyst.priority before calling this function.
 *
 * @return 0 on success or negative errno on failure.
 */
int nss_ppenl_port_mirror_analyst_set(struct nss_ppenl_port_mirror_rule *rule);

/**
 * nss_ppenl_port_mirror_analyst_clr()
 *	Clear (disable) the mirror analysis port for a given direction.
 *
 * @param[in] rule  Pointer to the port-mirror rule message.  The caller must
 *                  fill in rule->analyst.analyst_port_name and
 *                  rule->analyst.dir before calling this function.
 *
 * @return 0 on success or negative errno on failure.
 */
int nss_ppenl_port_mirror_analyst_clr(struct nss_ppenl_port_mirror_rule *rule);

#endif /* __NSS_PPENL_PORT_MIRROR_API_H__ */
