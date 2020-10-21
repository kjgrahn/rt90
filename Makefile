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

.PHONY: install
install: rt90
	install -m755 rt90 $(INSTALLBASE)/bin/
	install -m644 rt90.1 $(INSTALLBASE)/man/man1/

.PHONY: clean
clean:
	$(RM) rt90.1.{ps,pdf}
	$(RM) *.[oa]
	$(RM) test.cc
	$(RM) rt90 tests
	$(RM) -r dep

CXXFLAGS=-Wall -Wextra -pedantic -Wold-style-cast -std=c++98 -g -Os

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

test.cc: libtest.a
	testicle -o$@ $^

tests: test.o librt90.a libtest.a
	$(CXX) -o $@ test.o -L. -ltest -lrt90 -lproj -lm

rt90: rt90.o librt90.a
	$(CXX) -o $@ rt90.o -L. -lrt90 -lproj -lm

librt90.a: planar.o
librt90.a: transform.o
librt90.a: coordinate.o
librt90.a: coord_transform.o
librt90.a: lmv_ctrl.o
	$(AR) -r $@ $^

libtest.a: test_lmv.o
libtest.a: test_coord.o
libtest.a: test_parser.o
	$(AR) -r $@ $^

%.1.ps : %.1
	groff -man -ma4 $< >$@
%.1.pdf : %.1.ps
	ps2pdf $< >$@

.PHONY: tags
tags: TAGS
TAGS:
	etags *.cc *.h

depend:
	makedepend -- $(CFLAGS) -- -Y *.cc

love:
	@echo "not war?"

$(shell mkdir -p dep)
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.c=$(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
-include dep/*.d
