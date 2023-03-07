/*
 * Copyright (c) 2023, Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#ifndef __NSS_PPENL_ACL_API_H__
#define __NSS_PPENL_ACL_API_H__

/** @addtogroup nss_ppenl_acl_datatypes @{ */

/**
 * Response callback for ACL.
 *
 * @param[in] user_ctx User context (provided at socket open).
 * @param[in] rule ACL rule.
 * @param[in] resp_ctx User data per callback.
 *
 * @return
 * None.
 */
typedef void (*nss_ppenl_acl_resp_cb_t)(void *user_ctx, struct nss_ppenl_acl_rule *rule, void *resp_ctx);

/**
 * Initializes ACL rule message.
 *
 * @param[in] rule ACL rule.
 * @param[in] type Command type.
 *
 * @return
 * None.
 */
void nss_ppenl_acl_init_rule(struct nss_ppenl_acl_rule *rule, enum nss_ppe_acl_message_types type);

/*
 * TODO Enable a true synchronous API and remove callback registration
 */
int nss_ppenl_acl_rule_add(struct nss_ppenl_acl_rule *rule, nss_ppenl_acl_resp_cb_t cb, void *data);
int nss_ppenl_acl_rule_del(struct nss_ppenl_acl_rule *rule, nss_ppenl_acl_resp_cb_t cb, void *data);



/** @} *//* end_addtogroup nss_ppenl_acl_functions */

#endif /* __NSS_PPENL_ACL_API_H__ */
