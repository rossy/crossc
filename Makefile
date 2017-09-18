CXX = g++
AR = ar
CXXFLAGS = -g -O2 -Wall
OBJ := spirv_cfg.o \
       spirv_cpp.o \
       spirv_cross.o \
       spirv_glsl.o \
       spirv_hlsl.o \
       spirv_msl.o \
       crossc-hlsl.o \
       crossc.o
DEP := $(OBJ:.o=.d)
STLIB := libcrossc.a
INC := crossc.h
PC := crossc.pc

CHOST != $(CXX) -dumpmachine

ifneq (,$(findstring x86_64-w64-,$(CHOST)))
    prefix = /mingw64
else
ifneq (,$(findstring i686-w64-,$(CHOST)))
    prefix = /mingw32
else
    prefix = /usr/local
endif
endif
exec_prefix = $(prefix)
includedir = $(prefix)/include
libdir = $(exec_prefix)/lib

CPPFLAGS += $(DEFS)
CXXFLAGS += -std=c++11 -MMD -MP

all: $(STLIB) $(PC)
.PHONY: all

-include $(DEP)

%.o: %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.o: SPIRV-Cross/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.pc: %.pc.in
	sed -e 's,%PREFIX%,$(prefix),' \
	    -e 's,%EXEC_PREFIX%,$(exec_prefix),' \
	    -e 's,%INCLUDEDIR%,$(includedir),' \
	    -e 's,%LIBDIR%,$(libdir),' $< > $@

$(STLIB): $(OBJ)
	$(AR) rcs $@ $(OBJ)

install: all
	install -dm755 $(DESTDIR)$(libdir)/pkgconfig
	install -m644 $(STLIB) $(DESTDIR)$(libdir)/$(STLIB)
	install -m644 $(PC) $(DESTDIR)$(libdir)/pkgconfig/$(PC)
	install -dm755 $(DESTDIR)$(includedir)
	install -m644 $(INC) $(DESTDIR)$(includedir)/$(INC)
.PHONY: install

uninstall:
	-rm -f $(DESTDIR)$(libdir)/$(STLIB)
	-rm -f $(DESTDIR)$(libdir)/pkgconfig/$(PC)
	-rm -f $(DESTDIR)$(includedir)/$(INC)
.PHONY: uninstall

clean:
	-rm -f $(STLIB) $(PC) $(OBJ) $(DEP)
.PHONY: clean
