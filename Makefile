CXX = g++
AR = ar
CXXFLAGS = -g -O2 -Wall
STLIB = libcrossc.a

HDR = $(dir $(firstword $(MAKEFILE_LIST)))/crossc.h
SOMAJOR != grep 'define CROSSC_VERSION_MAJOR' $(HDR) | sed -e 's/.* //'
SOMINOR != grep 'define CROSSC_VERSION_MINOR' $(HDR) | sed -e 's/.* //'
SOPATCH != grep 'define CROSSC_VERSION_PATCH' $(HDR) | sed -e 's/.* //'
CHOST != $(CXX) -dumpmachine

ifneq (,$(findstring x86_64-w64-,$(CHOST)))
    prefix = /mingw64
    SOLIB = crossc-$(SOMAJOR).dll
    IMPLIB = libcrossc.dll.a
    LDFLAGS += -Wl,--out-implib,$(IMPLIB)
    LDFLAGS += -Wl,--dynamicbase,--nxcompat,--no-seh
    LDFLAGS += -Wl,--image-base,0x140000000,--high-entropy-va
    dlldir = $(bindir)
else ifneq (,$(findstring i686-w64-,$(CHOST)))
    prefix = /mingw32
    SOLIB = crossc-$(SOMAJOR).dll
    IMPLIB = libcrossc.dll.a
    LDFLAGS += -Wl,--out-implib,$(IMPLIB)
    LDFLAGS += -Wl,--dynamicbase,--nxcompat,--no-seh
    dlldir = $(bindir)
else
    prefix = /usr/local
    LNNAME = libcrossc.so
    SONAME = libcrossc.so.$(SOMAJOR)
    SOLIB = libcrossc.so.$(SOMAJOR).$(SOMINOR).$(SOPATCH)
    CXXFLAGS += -fPIC
    LDFLAGS += -Wl,-soname,$(SONAME)
    dlldir = $(libdir)
endif

exec_prefix = $(prefix)
includedir = $(prefix)/include
bindir = $(exec_prefix)/bin
libdir = $(exec_prefix)/lib

OBJ := spirv_cfg.o \
       spirv_cpp.o \
       spirv_cross.o \
       spirv_glsl.o \
       spirv_hlsl.o \
       spirv_msl.o \
       crossc-hlsl.o \
       crossc.o
DEP := $(OBJ:.o=.d)
PC := crossc.pc
INC := crossc.h
VER := crossc.sym

CPPFLAGS += $(DEFS)
CXXFLAGS += -std=c++14 -MMD -MP

vpath % $(dir $(firstword $(MAKEFILE_LIST)))

all: static shared
data: $(PC)
static: data $(STLIB)
shared: data $(SOLIB)
.PHONY: all data static shared

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
	    -e 's,%LIBDIR%,$(libdir),' $< > $@

$(STLIB): $(OBJ)
	$(AR) rcs $@ $(OBJ)

$(SOLIB): $(VER) $(OBJ)
	$(CXX) -shared -Wl,--version-script=$< $(LDFLAGS) $(OBJ) $(LIBS) -o $@

install-data: $(INC) data
	install -dm755 $(DESTDIR)$(libdir)/pkgconfig
	install -m644 $(PC) $(DESTDIR)$(libdir)/pkgconfig/$(PC)
	install -dm755 $(DESTDIR)$(includedir)
	install -m644 $< $(DESTDIR)$(includedir)/$(INC)
.PHONY: install-data

install-static: static install-data
	install -dm755 $(DESTDIR)$(libdir)
	install -m644 $(STLIB) $(DESTDIR)$(libdir)/$(STLIB)
.PHONY: install-static

install-shared: shared install-data
	install -dm755 $(DESTDIR)$(dlldir)
	install -m644 $(SOLIB) $(DESTDIR)$(dlldir)/$(SOLIB)
ifneq (,$(IMPLIB))
	install -dm755 $(DESTDIR)$(libdir)
	install -m644 $(IMPLIB) $(DESTDIR)$(libdir)/$(IMPLIB)
endif
ifneq (,$(LNNAME))
	ln -s $(SOLIB) $(DESTDIR)$(dlldir)/$(LNNAME)
endif
.PHONY: install-shared

install: install-static install-shared
.PHONY: install

uninstall:
	-rm -f $(DESTDIR)$(libdir)/$(STLIB)
	-rm -f $(DESTDIR)$(dlldir)/$(SOLIB)
ifneq (,$(IMPLIB))
	-rm -f $(DESTDIR)$(libdir)/$(IMPLIB)
endif
ifneq (,$(LNNAME))
	-rm -f $(DESTDIR)$(dlldir)/$(LNNAME)
endif
	-rm -f $(DESTDIR)$(libdir)/pkgconfig/$(PC)
	-rm -f $(DESTDIR)$(includedir)/$(INC)
.PHONY: uninstall

clean:
	-rm -f $(STLIB) $(SOLIB) $(IMPLIB) $(PC) $(OBJ) $(DEP)
.PHONY: clean
