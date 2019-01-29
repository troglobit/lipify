#include <ipify.h>

int main(void)
{
	char addr[256];
#if 0
	int sd;

	sd = ipify_connect();
	if (sd < 0)
		return 1;

	if (!ipify_query(sd, addr, sizeof(addr)))
		printf("%s\n", addr);

	return ipify_disconnect(sd);
#else
	if (ipify(addr, sizeof(addr)))
		return 1;

	printf("%s\n", addr);
	return 0;
#endif
}
