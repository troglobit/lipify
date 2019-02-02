/* The contents of this file is in the public domain. */
#include <ipify.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	int family = AF_UNSPEC;
	char addr[256];
	int i, sd;

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h")) {
			printf("ipify [-46h]\n");
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
		
		printf("Invalid option: '%s'\n", argv[i]);
		return 1;
	}

	sd = ipify_connect1(family);
	if (sd < 0)
		return 1;

	if (!ipify_query(sd, addr, sizeof(addr)))
		printf("%s\n", addr);

	return ipify_disconnect(sd);
}
