/* Simple C library for querying your IP address using http://ipify.org
 *
 * Copyright (c) 2015-2022  Joachim Wiberg <troglobit@gmail.com>
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

#ifndef IPIFY_H_
#define IPIFY_H_

#include <stddef.h>

#define IPIFY_HOST   "api.ipify.org"

int ipify_connect    (void);
int ipify_connect1   (int family);
int ipify_connect2   (const char *host, int family);
int ipify_query      (int sd, char *addr, size_t len);
int ipify_disconnect (int sd);

int ipify            (char *addr, size_t len);

#endif /* IPIFY_H_ */
