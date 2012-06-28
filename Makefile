# $Id: Makefile,v 1.50 2011-06-14 21:32:38 grahn Exp $
#
# Makefile
#
# Copyright (c) 2012 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

.PHONY: all
all: 

.PHONY: install
install: rt90
	install -m755 rt90 $(INSTALLBASE)/bin/
	install -m644 rt90.1 $(INSTALLBASE)/man/man1/

.PHONY: clean
clean:
	$(RM) rt90.1.{ps,pdf}
	$(RM) *.[oa]
	$(RM) test.cc Makefile.bak
	$(RM) rt90 tests

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

librt90.a: planar.o
librt90.a: transform.o
	$(AR) -r $@ $^

libtest.a: test_lmv.o
	$(AR) -r $@ $^

%.1.ps : %.1
	groff -man -ma4 $< >$@
%.1.pdf : %.1.ps
	ps2pdf $< >$@

depend:
	makedepend -- $(CFLAGS) -- -Y *.cc

love:
	@echo "not war?"

# DO NOT DELETE

planar.o: planar.h
test_lmv.o: planar.h transform.h
transform.o: transform.h planar.h
