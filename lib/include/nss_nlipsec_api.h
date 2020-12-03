/*
 **************************************************************************
 * Copyright (c) 2019-2021, The Linux Foundation. All rights reserved.
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 **************************************************************************
 */

#ifndef __NSS_NLIPSEC_API_H__
#define __NSS_NLIPSEC_API_H__

/**
 * @file nss_nlipsec_api.h
 * 	This file declares the NSS NL IPsec API(s) for userspace. These
 * 	API(s) are wrapper functions for IPsec family specific operation.
 */

/**
 * @brief response callback function
 *
 * @param user_ctx[IN] user context, provided at socket open
 * @param rule[IN] IPsec rule associated with the response
 * @param resp_ctx[IN] data that the user wants per callback
 */
typedef void (*nss_nlipsec_resp_t)(void *user_ctx, struct nss_nlipsec_rule *rule, void *resp_ctx);

/**
 * @brief event callback function
 *
 * @param user_ctx[IN] user context, provided at socket open
 * @param rule[IN] IPsec rule associated with the event
 */
typedef void (*nss_nlipsec_event_t)(void *user_ctx, struct nss_nlipsec_rule *rule);

/**
 * @brief NSS NL IPsec response
 */
struct nss_nlipsec_resp {
	void *data;		/**< response context */
	nss_nlipsec_resp_t cb;	/**< response callback */
};

/**
 * @brief NSS IPsec context
 */
struct nss_nlipsec_ctx {
	struct nss_nlsock_ctx sock;	/**< NSS socket context */
	nss_nlipsec_event_t event;	/**< NSS event callback function */
};

/**
 * @brief Open NSS IPsec NL socket
 *
 * @param ctx[IN] NSS NL socket context, allocated by the caller
 * @param user_ctx[IN] user context stored per socket
 * @param event_cb[IN] event callback handler
 *
 * @return status of the open call
 */
int nss_nlipsec_sock_open(struct nss_nlipsec_ctx *ctx, void *user_ctx, nss_nlipsec_event_t event_cb);

/**
 * @brief Close NSS IPsec NL socket
 *
 * @param ctx[IN] NSS NL context
 */
void nss_nlipsec_sock_close(struct nss_nlipsec_ctx *ctx);

/**
 * @brief sends NSS IPsec rule message synchronously via netlink
 *
 * @param ctx[IN] NSS IPsec NL context
 * @param rule[IN] IPsec rule to use
 * @param cb[IN] response callback handler
 * @param data[IN] response data per callback
 *
 * @return status of send, where '0' is success and -ve means failure
 */
int nss_nlipsec_sock_send(struct nss_nlipsec_ctx *ctx, struct nss_nlipsec_rule *rule, nss_nlipsec_resp_t cb, void *data);

/**
 * @brief IPsec initialization command
 *
 * @param rule[IN] IPsec rule
 * @param type[IN] command type
 */
void nss_nlipsec_init_cmd(struct nss_nlipsec_rule *rule, enum nss_nlipsec_cmd type);

/**}@*/
#endif /* !__NSS_NLIPV4_API_H__*/
