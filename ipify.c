/* Simple C library for querying your IP address using http://ipify.org
 *
 * Copyright (c) 2015-2019  Joachim Nilsson <troglobit@gmail.com>
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

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define IPIFY_HOST    "api.ipify.org"
#define AGENT_NAME    PACKAGE_NAME "/" PACKAGE_VERSION
#define HTTP_REQUEST				\
	"GET / HTTP/1.0\r\n"			\
	"Host: " IPIFY_HOST "\r\n"		\
	"User-Agent: " AGENT_NAME "\r\n\r\n";

/*
 * Connect to api.ipify.org using either address protocol supported.  We
 * want to connect using TCP, so ask getaddrinfo() for a TCP connection
 * over either IPv4 or IPv6, then use the first successful connection.
 */
int ipify_connect(void)
{
	struct addrinfo *result, *rp;
	struct addrinfo hints;
	int rc, sd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	rc = getaddrinfo(IPIFY_HOST, "http", &hints, &result);
	if (rc || !result)
		return -1;

	for (rp = result; rp; rp = rp->ai_next) {
		sd = socket(rp->ai_family, rp->ai_socktype, 0);
		if (sd < 0)
			continue;

		rc = connect(sd, result->ai_addr, result->ai_addrlen);
		if (rc) {
			close(sd);
			sd = -1;
			continue;
		}

		break;
	}

	freeaddrinfo(result);

	return sd;
}

int ipify_query(int sd, char *addr, size_t len)
{
	const char *req = HTTP_REQUEST;
	ssize_t rc;
	char tmp[sizeof(struct in6_addr)];
	char buf[512], *ptr;
	int domain;

	rc = send(sd, req, strlen(req), 0);
	if (rc < 0)
		return -1;

	rc = recv(sd, buf, sizeof(buf), 0);
	if (rc < 0)
		return -1;
	buf[rc] = 0;

	ptr = strstr(buf, "200 OK");
	if (!ptr)
		return 1;

	ptr = strstr(ptr, "\r\n\r\n");
	if (!ptr)
		return 1;
	ptr += 4;

	domain = AF_INET;
	if (!inet_pton(domain, ptr, tmp)) {
		domain = AF_INET6;
		if (!inet_pton(domain, ptr, tmp))
			return 1;
	}

	if (!inet_ntop(domain, tmp, addr, len))
		return 1;

	return 0;
}

int ipify_disconnect(int sd)
{
	shutdown(sd, SHUT_RDWR);
	return close(sd);
}

int ipify(char *addr, size_t len)
{
	int sd, ret;

	sd = ipify_connect();
	if (sd < 0)
		return 1;

	ret  = ipify_query(sd, addr, len);
	ret |= ipify_disconnect(sd);

	return ret;
}
