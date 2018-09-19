CXX ?= g++
AR ?= ar
CXXFLAGS ?= -g -O2 -Wall

exec_prefix = $(prefix)
includedir = $(prefix)/include
bindir = $(exec_prefix)/bin
libdir = $(exec_prefix)/lib

HDR = $(dir $(firstword $(MAKEFILE_LIST)))/crossc.h
SOMAJOR = $(shell grep 'define CROSSC_VERSION_MAJOR' $(HDR) | sed -e 's/.* //')
SOMINOR = $(shell grep 'define CROSSC_VERSION_MINOR' $(HDR) | sed -e 's/.* //')
SOPATCH = $(shell grep 'define CROSSC_VERSION_PATCH' $(HDR) | sed -e 's/.* //')

STLIB = libcrossc.a

CHOST = $(shell $(CXX) -dumpmachine)

ifneq (,$(findstring x86_64-w64-,$(CHOST)))
    prefix = /mingw64
    sodir = $(bindir)
    SOLIB = crossc-$(SOMAJOR).dll
    IMPLIB = libcrossc.dll.a
    LDFLAGS += -Wl,--dynamicbase,--nxcompat,--no-seh
    LDFLAGS += -Wl,--image-base,0x140000000,--high-entropy-va
    override LDFLAGS += -Wl,--out-implib,$(IMPLIB)
else ifneq (,$(findstring i686-w64-,$(CHOST)))
    prefix = /mingw32
    sodir = $(bindir)
    SOLIB = crossc-$(SOMAJOR).dll
    IMPLIB = libcrossc.dll.a
    LDFLAGS += -Wl,--dynamicbase,--nxcompat,--no-seh
    override LDFLAGS += -Wl,--out-implib,$(IMPLIB)
else ifneq (,$(findstring -apple-darwin,$(CHOST)))
    prefix = /usr/local
    sodir = $(libdir)
    LNNAME = libcrossc.dylib
    SOLIB = libcrossc.$(SOMAJOR).dylib
    override CXXFLAGS += -fPIC
    override LDFLAGS += -install_name $(SOLIB)
    override LDFLAGS += -current_version $(SOMAJOR).$(SOMINOR).$(SOPATCH)
    override LDFLAGS += -compatibility_version $(SOMAJOR).$(SOMINOR)
else
    prefix = /usr/local
    sodir = $(libdir)
    LNNAME = libcrossc.so
    SONAME = libcrossc.so.$(SOMAJOR)
    SOLIB = libcrossc.so.$(SOMAJOR).$(SOMINOR).$(SOPATCH)
    LDFLAGS += -Wl,-O1,-z,relro
    override CXXFLAGS += -fPIC
    override LDFLAGS += -Wl,-soname,$(SONAME)
endif

OBJ := spirv_cfg.o \
       spirv_cpp.o \
       spirv_cross.o \
       spirv_glsl.o \
       spirv_hlsl.o \
       spirv_msl.o \
       crossc-glsl.o \
       crossc-hlsl.o \
       crossc.o
DEP := $(OBJ:.o=.d)
PC := crossc.pc
INC := crossc.h
VER := crossc.sym

override CPPFLAGS += $(DEFS)
override CXXFLAGS += -std=c++14 -MMD -MP

vpath % $(dir $(firstword $(MAKEFILE_LIST)))

all: static shared
static: $(STLIB)
shared: $(SOLIB)
.PHONY: all static shared

-include $(DEP)

%.o: %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.o: SPIRV-Cross/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.pc: %.pc.in
	sed -e 's,%SOMAJOR%,$(SOMAJOR),' \
	    -e 's,%SOMINOR%,$(SOMINOR),' \
	    -e 's,%SOPATCH%,$(SOPATCH),' \
	    -e 's,%PREFIX%,$(prefix),' \
	    -e 's,%EXEC_PREFIX%,$(exec_prefix),' \
	    -e 's,%INCLUDEDIR%,$(includedir),' \
	    -e 's,%LIBDIR%,$(libdir),' \
	    -e 's, *%LIBS%,,' \
	    -e 's,%LIBS.PRIVATE%,-lstdc++,' $< > $@

%-static.pc: %.pc.in
	sed -e 's,%SOMAJOR%,$(SOMAJOR),' \
	    -e 's,%SOMINOR%,$(SOMINOR),' \
	    -e 's,%SOPATCH%,$(SOPATCH),' \
	    -e 's,%PREFIX%,$(prefix),' \
	    -e 's,%EXEC_PREFIX%,$(exec_prefix),' \
	    -e 's,%INCLUDEDIR%,$(includedir),' \
	    -e 's,%LIBDIR%,$(libdir),' \
	    -e 's,%LIBS%,-lstdc++,' \
	    -e 's, *%LIBS.PRIVATE%,,' $< > $@

$(STLIB): $(OBJ)
	$(AR) rcs $@ $(OBJ)

ifneq (,$(findstring -apple-darwin,$(CHOST)))
$(SOLIB): $(OBJ)
	$(CXX) -dynamiclib -Wl,-exported_symbol,'_crossc_*' $(LDFLAGS) $(OBJ) $(LIBS) -o $@
else
$(SOLIB): $(VER) $(OBJ)
	$(CXX) -shared -Wl,--version-script=$< $(LDFLAGS) $(OBJ) $(LIBS) -o $@
endif

install-includes: $(INC)
	install -dm755 $(DESTDIR)$(includedir)
	install -m644 $< $(DESTDIR)$(includedir)/$(INC)
.PHONY: install-includes

install-pc: crossc.pc
	install -dm755 $(DESTDIR)$(libdir)/pkgconfig
	install -m644 $< $(DESTDIR)$(libdir)/pkgconfig/$(PC)
.PHONY: install-pc

install-pc-static: crossc-static.pc
	install -dm755 $(DESTDIR)$(libdir)/pkgconfig
	install -m644 $< $(DESTDIR)$(libdir)/pkgconfig/$(PC)
.PHONY: install-pc-static

install-stlib: static
	install -dm755 $(DESTDIR)$(libdir)
	install -m644 $(STLIB) $(DESTDIR)$(libdir)/$(STLIB)
.PHONY: install-stlib

install-solib: shared
	install -dm755 $(DESTDIR)$(sodir)
	install -m755 $(SOLIB) $(DESTDIR)$(sodir)/$(SOLIB)
ifneq (,$(IMPLIB))
	install -dm755 $(DESTDIR)$(libdir)
	install -m644 $(IMPLIB) $(DESTDIR)$(libdir)/$(IMPLIB)
endif
ifneq (,$(LNNAME))
	ln -fs $(SOLIB) $(DESTDIR)$(sodir)/$(LNNAME)
endif
.PHONY: install-solib

install: install-stlib install-solib install-includes install-pc
install-static: install-stlib install-includes install-pc-static
install-shared: install-solib install-includes install-pc
.PHONY: install install-static install-shared

uninstall:
	-rm -f $(DESTDIR)$(libdir)/$(STLIB)
	-rm -f $(DESTDIR)$(sodir)/$(SOLIB)
ifneq (,$(IMPLIB))
	-rm -f $(DESTDIR)$(libdir)/$(IMPLIB)
endif
ifneq (,$(LNNAME))
	-rm -f $(DESTDIR)$(sodir)/$(LNNAME)
endif
	-rm -f $(DESTDIR)$(libdir)/pkgconfig/$(PC)
	-rm -f $(DESTDIR)$(includedir)/$(INC)
.PHONY: uninstall

clean:
	-rm -f $(STLIB) $(SOLIB) $(IMPLIB) crossc.pc crossc-static.pc $(OBJ) $(DEP)
.PHONY: clean
