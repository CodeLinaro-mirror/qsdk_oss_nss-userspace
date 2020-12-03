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

#ifndef __NSS_NLSOCK_API_H__
#define __NSS_NLSOCK_API_H__

/**
 * @addtogroup libnss_nl
 * @{
 */

/**
 * @file nss_nlsock_api.h
 * 	This file declares the NSS NL Socket API(s) for direct use.
 *
 * Note: Use these API(s) only if there are no helpers available for the
 * 	 specific family
 */

/**
 * @brief NSS NL socket context
 */
struct nss_nlsock_ctx {
	/* Public, caller must populate using helpers */
	const char *family_name;		/**< family name */
	void *user_ctx;				/**< socket user's context */

	/* Private, maintained by the library */
	pthread_t thread;			/**< response sync */
	pthread_spinlock_t lock;		/**< context lock */
	int ref_cnt;				/**< references to the socket */

	struct nl_sock *nl_sk;			/**< Linux NL socket */
	struct nl_cb *nl_cb;			/**< NSS NL callback context */

	pid_t pid;				/**< pid associated with the socket */
	int family_id;				/**< family identifier */
	int grp_id;				/**< group indentifier */
	bool is_avail;				/**< indicates if the socket is available to send/listen */
};

#define nss_nlsock_log_error(arg, ...) printf("NSS_NLERROR(%s[%d]):"arg, __func__, __LINE__, ##__VA_ARGS__)
#define nss_nlsock_log_info(arg, ...) printf("NSS_NLINFO(%s[%d]):"arg, __func__, __LINE__, ##__VA_ARGS__)

/**
 * @brief helper for setting the family name
 *
 * @param sock[IN] socket context
 * @param name[IN] family name
 */
static inline void nss_nlsock_set_family(struct nss_nlsock_ctx *sock, const char *name)
{
	sock->family_name = name;
}

/**
 * @brief helper for setting the user context
 *
 * @param sock[IN] socket context
 * @param user[IN] user context
 */
static inline void nss_nlsock_set_user_ctx(struct nss_nlsock_ctx *sock, void *user)
{
	sock->user_ctx = user;
}

/**
 * @brief extract the NSS NL message data
 *
 * @param msg[IN] NL message
 *
 * @return start of NSS NL message
 */
static inline void *nss_nlsock_get_data(struct nl_msg *msg)
{
	struct genlmsghdr *genl_hdr = nlmsg_data((nlmsg_hdr(msg)));

	return genlmsg_data(genl_hdr);
}

/**
 * @brief open the NSS NL family socket
 *
 * @param sock[IN] socket context, to be allocated by the caller
 * @param cb[IN] callback function for response
 *
 * @return status of the operation
 *
 * @note underlying entity should set sock->family name for socket to open
 */
int nss_nlsock_open(struct nss_nlsock_ctx *sock, nl_recvmsg_msg_cb_t cb);

/**
 * @brief close the NSS NL family socket
 *
 * @param sock[IN] socket context
 */
void nss_nlsock_close(struct nss_nlsock_ctx *sock);

/**
 * @brief send a NSS NL message synchronously
 *
 * @param sock[IN] socket context
 * @param cm[IN] common message header
 * @param data[IN] message data
 * @param has_resp[IN] determines if response is needed from kernel
 *
 * @return status of the send operation
 *
 * @note the function blocks until ack/error is received from the kernel
 * and also blocks for the message response from the kernel if is_resp is true
 */
int nss_nlsock_send(struct nss_nlsock_ctx *sock, struct nss_nlcmn *cm, void *data, bool has_resp);

/**
 * @brief listening to asynchronous events from kernel
 *
 * @param sock[IN] socket context
 *
 * @return status of the listen
 */
int nss_nlsock_listen(struct nss_nlsock_ctx *sock);

/**
 * @brief subscribe to multicast group
 *
 * @param sock[IN] socket context
 * @param grp_name[IN] NSS NL group name
 *
 * @return status of the subscription
 */
int nss_nlsock_join_grp(struct nss_nlsock_ctx *sock, char *grp_name);

/**
 * @brief unsubscribe to multicast group
 *
 * @param sock[IN] socket context
 * @param grp_name[IN] NSS NL group name
 *
 * @return status of the operation
 */
int nss_nlsock_leave_grp(struct nss_nlsock_ctx *sock, char *grp_name);

/**
 * @brief Open a socket for listening to NSS NL event data, when a event arrives
 *	it will be delivered using through the callback function
 *
 * @param sock[IN] socket context
 * @param cb[IN] callback function
 *
 * @return status of the operation
 */
int nss_nlsock_open_mcast(struct nss_nlsock_ctx *sock, nl_recvmsg_msg_cb_t cb);

/**}@*/
#endif /* !__NSS_NLSOCK_API_H__*/
