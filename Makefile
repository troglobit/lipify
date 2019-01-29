.PHONY: all clean distclean

# Figure out root of library, unless used as submodule
ROOTDIR    ?= $(shell pwd)

VERSION    ?= 0.4
NAME       := lipify
PKG        := $(NAME)-$(VERSION)
ARCHIVE    := $(PKG).tar.xz

CC         ?= $(CROSS)gcc
AR         ?= $(CROSS)ar

CFLAGS     += -fPIC
CPPFLAGS   += -DPACKAGE_NAME='"$(NAME)"' -DPACKAGE_VERSION='"$(VERSION)"'
CPPFLAGS   += -I.
ARFLAGS     = crus

DISTFILES   = README.md LICENSE
HEADERS     = ipify.h
EXEC        = ipify
OBJS        = ipify.o
HDRS        = ipify.h
JUNK        = *~ *.bak *.map .*.d *.so* *.a DEADJOE core core.*

VER         = 1
LIBNAME     = libipify
SOLIB       = $(LIBNAME).so.$(VER)
SYMLIB      = $(LIBNAME).so
STATICLIB   = $(LIBNAME).a
TARGET      = $(STATICLIB) $(SOLIB) $(EXEC)

# Default install paths
prefix     ?= /usr/local
bindir     ?= $(prefix)/bin
libdir     ?= $(prefix)/lib
datadir    ?= $(prefix)/share/doc/$(LIBNAME)
incdir     ?= $(prefix)/include

%.o: %.c
	@printf "  CC      $(subst $(ROOTDIR)/,,$(shell pwd)/)$@\n"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

all: $(TARGET)

$(EXEC): main.o $(STATICLIB)
	@printf "  LINK    $(subst $(ROOTDIR)/,,$(shell pwd)/)$@\n"
	@$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$@.map -o $@ $^ $(LDLIBS$(LDLIBS-$(@)))

$(SOLIB): Makefile $(OBJS) $(HDRS)
	@printf "  LINK    $@\n"
	@$(CC) $(LDFLAGS) -shared -Wl,-soname=$@ -o $@ $(OBJS)

$(STATICLIB): Makefile $(OBJS) $(HDRS)
	@printf "  ARCHIVE $@\n"
	@$(AR) $(ARFLAGS) $@ $(OBJS) 2>/dev/null

install-exec: $(SOLIB) $(EXEC)
	@printf "  INSTALL $(DESTDIR)$(libdir)/$(SOLIB)\n"
	@install -d $(DESTDIR)$(libdir)
	@install $(SOLIB) $(DESTDIR)$(libdir)/$(SOLIB)
	@ln -sf $(SOLIB) $(DESTDIR)$(libdir)/$(SYMLIB)
	@printf "  INSTALL $(DESTDIR)$(bindir)/$(EXEC)\n"
	@install -d $(DESTDIR)$(bindir)
	@install $(EXEC) $(DESTDIR)$(bindir)/$(EXEC)

install-dev:
	@install -d $(DESTDIR)$(incdir)
	@for file in $(HEADERS); do					\
		printf "  INSTALL $(DESTDIR)$(incdir)/$$file\n";	\
		install -m 0644 $$file $(DESTDIR)$(incdir)/$$file;	\
	done
	@printf "  INSTALL $(DESTDIR)$(libdir)/$(STATICLIB)\n"
	@install -d $(DESTDIR)$(libdir)
	@install $(STATICLIB) $(DESTDIR)$(libdir)/$(STATICLIB)

install-data:
	@install -d $(DESTDIR)$(datadir)
	@for file in $(DISTFILES); do					\
		printf "  INSTALL $(DESTDIR)$(datadir)/$$file\n";	\
		install -m 0644 $$file $(DESTDIR)$(datadir)/$$file;	\
	done

install: install-dev install-exec install-data

uninstall:
	-@$(RM) -r $(DESTDIR)$(datadir)
	@for file in $(HEADERS); do					\
		$(RM) -r $(DESTDIR)$(incdir)/$$file;			\
	done
	-@$(RM) $(DESTDIR)$(libdir)/$(SOLIB)
	-@$(RM) $(DESTDIR)$(libdir)/$(SYMLIB)
	-@$(RM) $(DESTDIR)$(libdir)/$(STATICLIB)

clean:
	-@$(RM) $(TARGET) *.o

distclean: clean
	-@$(RM) $(DEPS) $(JUNK)

dist:
	@echo "Building .xz tarball of $(PKG) in parent dir..."
	git archive --format=tar --prefix=$(PKG)/ v$(VERSION) | xz >../$(ARCHIVE)
	@(cd ..; md5sum $(ARCHIVE) | tee $(ARCHIVE).md5)

