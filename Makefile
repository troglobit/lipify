.PHONY: all testit clean

# Figure out root of library, unless used as submodule
ROOTDIR    ?= $(shell pwd)

VERSION    := 0.1
NAME       := lipify

CC         ?= $(CROSS)gcc
AR         ?= $(CROSS)ar

CFLAGS     += -fPIC
CPPFLAGS   += -DPACKAGE_NAME='"$(NAME)"' -DPACKAGE_VERSION='"$(VERSION)"'
CPPFLAGS   += -I.
ARFLAGS     = crus

OBJS        = ipify.o
HDRS        = ipify.h
JUNK        = *~ *.bak *.map .*.d *.so* *.a DEADJOE core core.*

VER         = 1
LIBNAME     = libipify
SOLIB       = $(LIBNAME).so.$(VER)
SYMLIB      = $(LIBNAME).so
STATICLIB   = $(LIBNAME).a
TARGET      = $(STATICLIB) $(SOLIB)

%.o: %.c
	@printf "  CC      $(subst $(ROOTDIR)/,,$(shell pwd)/)$@\n"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

all: $(TARGET)

testit: testit.o $(STATICLIB)
	@printf "  LINK    $(subst $(ROOTDIR)/,,$(shell pwd)/)$@\n"
	@$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$@.map -o $@ $^ $(LDLIBS$(LDLIBS-$(@)))

$(SOLIB): Makefile $(OBJS) $(HDRS)
	@printf "  LINK    $@\n"
	@$(CC) $(LDFLAGS) -shared -Wl,-soname=$@ -o $@ $(OBJS)

$(STATICLIB): Makefile $(OBJS) $(HDRS)
	@printf "  ARCHIVE $@\n"
	@$(AR) $(ARFLAGS) $@ $(OBJS) 2>/dev/null

clean:
	-@$(RM) $(TARGET) *.o testit

distclean: clean
	-@$(RM) $(DEPS) $(JUNK)
