/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef __NSS_PPENL_COS_MAP_API_H__
#define __NSS_PPENL_COS_MAP_API_H__


/*
 * TODO: Enable a true synchronous API and remove callback registration
 */
#define NSS_PPENL_COS_MAP_RET_SUCCESS			0	/* Successful operation return value from ppe */
#define	NSS_PPENL_COS_MAP_CREATE_RULE_FAIL	1	/* Rule creation failed */
#define	NSS_PPENL_COS_MAP_DESTROY_RULE_FAIL	2	/* Rule deletion failed */
#define	NSS_PPENL_COS_MAP_FLUSH_RULE_FAIL	3	/* Rule flush failed */
#define NSS_PPENL_COS_MAP_PORT_GROUP_SET_FAIL  4	/* Port group set failed */

void nss_ppenl_cos_map_init_config(struct nss_ppenl_cos_map_config *config, enum nss_ppe_cos_map_message_types type);
int nss_ppenl_cos_map_send_config(struct nss_ppenl_cos_map_config *config);

#endif /* __NSS_PPENL_COS_MAP_API_H__ */
