/* The contents of this file is in the public domain. */
#include <ipify.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	char *host = IPIFY_HOST;
	int family = AF_UNSPEC;
	char addr[256];
	int i, sd;

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h")) {
			printf("ipify [-46h] [HOST]\n");
			return 0;
		}
		if (!strcmp(argv[i], "-4")) {
			family = AF_INET;
			continue;
		}
		if (!strcmp(argv[i], "-6")) {
			family = AF_INET6;
			continue;
		}

		break;
	}

	if (i < argc)
		host = argv[i++];

	sd = ipify_connect2(host, family);
	if (sd < 0)
		return 1;

	if (!ipify_query(sd, addr, sizeof(addr)))
		printf("%s\n", addr);

	return ipify_disconnect(sd);
}
