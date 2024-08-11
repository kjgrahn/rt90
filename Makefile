# $Id: Makefile,v 1.50 2011-06-14 21:32:38 grahn Exp $
#
# Makefile
#
# Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

.PHONY: all
all: rt90
all: test/test

.PHONY: install
install: rt90
	install -m755 rt90 $(INSTALLBASE)/bin/
	install -m644 rt90.1 $(INSTALLBASE)/man/man1/

.PHONY: clean
clean:
	$(RM) rt90.1.{ps,pdf}
	$(RM) *.o lib*.a
	$(RM) test/*.o test/lib*.a
	$(RM) test/test test/test.cc
	$(RM) rt90
	$(RM) -r dep

CXXFLAGS=-Wall -Wextra -pedantic -Wold-style-cast -std=c++14 -g -Os
ARFLAGS=rTP

.PHONY: check checkv
check: test/test
	./test/test
checkv: test/test
	valgrind -q ./test/test -v

test/test.cc: test/libtest.a
	orchis -o $@ $^

test/test: test/test.o librt90.a test/libtest.a
	$(CXX) -o $@ test/test.o -L. -Ltest -ltest -lrt90 -lproj -lm

rt90: rt90.o librt90.a
	$(CXX) -o $@ rt90.o -L. -lrt90 -lproj -lm

librt90.a: planar.o
librt90.a: transform.o
librt90.a: coordinate.o
librt90.a: coord_transform.o
librt90.a: lmv_ctrl.o
	$(AR) $(ARFLAGS) $@ $^

test/libtest.a: test/lmv.o
test/libtest.a: test/coord.o
test/libtest.a: test/parser.o
test/libtest.a: test/join.o
	$(AR) $(ARFLAGS) $@ $^

test/%.o: CPPFLAGS+=-I.

%.1.ps : %.1
	groff -man -ma4 $< >$@
%.1.pdf : %.1.ps
	ps2pdf $< >$@

.PHONY: tags TAGS
tags: TAGS
TAGS:
	ctags --exclude='test*' -eR . --extra=q

love:
	@echo "not war?"

$(shell mkdir -p dep/test)
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.c=$(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/test/%.d: ;
-include dep/*.d
-include dep/test/*.d
