C lib for ipify.org
===================

lipify connects to http://ipify.org to query your current IP address.

Example
-------

```C
    #include <ipify.h>
    
    int main(void)
    {
            int sd;
            char addr[256];
    
            sd = ipify_connect();
            if (sd < 0)
                    return 1;
    
            if (!ipify_query(sd, addr, sizeof(addr)))
                    printf("%s\n", addr);
    
            return ipify_disconnect(sd);
    }
```

