/* The contents of this file is in the public domain. */
#include <ipify.h>

int main(void)
{
	char addr[256];
	int sd;

	sd = ipify_connect();
	if (sd < 0)
		return 1;

	if (!ipify_query(sd, addr, sizeof(addr)))
		printf("%s\n", addr);

	return ipify_disconnect(sd);
}
