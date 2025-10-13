/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef __NSS_PPENL_QOS_API_H__
#define __NSS_PPENL_QOS_API_H__

/** @addtogroup chapter_nlQOS
 This chapter describes QOS APIs in the user space.
 These APIs are wrapper functions for QOS family-specific operations.
*/

/** @addtogroup nss_ppenl_qos_datatypes @{ */

/**
 * Response callback for QOS.
 *
 * @param[in] user_ctx User context (provided at socket open).
 * @param[in] req QOS req.
 * @param[in] resp_ctx User data per callback.
 *
 * @return
 * None.
 */
typedef void (*nss_ppenl_qos_resp_cb_t)(void *user_ctx, struct nss_ppenl_qos_req *req, void *resp_ctx);

/*
 * TODO: Enable a true synchronous API and remove callback registration
 */
#define PPECFG_QOS_RET_SUCCESS			0	/* Successful operation return value from ppe */
#define PPECFG_QOS_RET_CLASS_NON_LEAF		1	/* Non Leaf class return value from ppe */
#define PPECFG_QOS_RET_INVALID_CLASS		2	/* Invalid class return value from ppe */
#define PPECFG_QOS_RET_INVALID_DEV		3	/* Invalid physical interface return value from ppe */
#define PPECFG_QOS_RET_NO_QDISC_CONFIGURED	4	/* PPE QDISC not configured */
#define	PPECFG_QOS_RET_SHAPER_CREATE_FAIL	5	/* Shaper creation failed */
#define	PPECFG_QOS_RET_SHAPER_DELETE_FAIL	6	/* Shaper delete failed */
#define	PPECFG_QOS_RET_IF_QUEUES_CREATE_FAIL	7	/* Interface queues creation failed */
#define	PPECFG_QOS_RET_IF_QUEUES_FLUSH_FAIL	8	/* Interface queues flush failed */
#define	PPECFG_QOS_RET_IF_SHAPER_SET_FAIL	9	/* Interface shaper set failed */
#define	PPECFG_QOS_RET_QUEUE_PQ_MAPPING_FAIL	10	/* Priority queue mapping failed */
#define PPECFG_QOS_RET_TCONT_STATS_GET_FAIL     11  /* Tcont stats fetch failure */
#define	PPECFG_QOS_RET_RESET_TCONT_CREDIT_FAIL  12	/* Tcont credit reset failure */
#define	PPECFG_QOS_RET_QUEUE_TM_CONFIG_FAIL	13	/* Queue traffic management configuration failed */
#define	PPECFG_QOS_RET_QUEUE_LIMIT_CONFIG_FAIL	14	/* Queue limit and threshold configuration failed */
#define	PPECFG_QOS_RET_QUEUE_CTRL_SET_FAIL	15	/* Queue control set failed */

void nss_ppenl_qos_init_req(struct nss_ppenl_qos_req *req, enum nss_ppe_qos_message_types type);
int nss_ppenl_qos_send_req(struct nss_ppenl_qos_req *req);

#endif /* __NSS_PPENL_QOS_API_H__ */
