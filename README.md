C library for api.ipify.org
===========================

lipify connects to http://api.ipify.org to query your current IP address.
Both IPv4 and IPv6 is supported, for connecting to the ipify server and
returning your address.

The library weighs in at 14 kiB.  A versioned tarball can be downloaded
from GitHub: https://github.com/troglobit/lipify/releases


Build & Install
---------------

By default, lipify installs to `/usr/local`:

```
make
make install
```

The default can be changed by setting the `prefix=` variable at install:

```
make install prefix=/opt
```

> The `DESTDIR=` variable is also supported, for the benefit of packagers.


Example
-------

The simplest example of lipify looks like this:

```C
    #include <stdio.h>
    #include <ipify.h>

    int main(void)
    {
            char addr[256];

            if (ipify(addr, sizeof(addr)))
                    return 1;

            printf("%s\n", addr);

            return 0;
    }
```

You can also get a descriptor, to use with `poll()` or an event loop library like
[libuEv](https://github.com/troglobit/libuev), like this:


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


License
-------

lipify is free software, licensed to you under the very permissive
[ISC license](https://en.wikipedia.org/wiki/ISC_license).  See the
file LICENSE in the lipify distribution for details.


Origin & References
-------------------

lipify is written by [Joachim Wiberg](https://troglobit.com), post
bug reports and pull requests to the project's GitHub repository at
https://github.com/troglobit/lipify
