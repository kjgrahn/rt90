# $Id: Makefile,v 1.50 2011-06-14 21:32:38 grahn Exp $
#
# Makefile
#
# Copyright (c) 2012 J�rgen Grahn <grahn+src@snipabacken.se>
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
	$(RM) test.cc Makefile.bak
	$(RM) version.cc
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

rt90: rt90.o librt90.a
	$(CXX) -o $@ rt90.o -L. -lrt90 -lproj -lm

version.cc: NEWS mkversion
	./mkversion $$(perl -ne 'print "$$1 $$2\n" and exit \
	if /^(\S+) (\S+) (\S+):$$/' NEWS) $@

librt90.a: planar.o
librt90.a: transform.o
librt90.a: coordinate.o
librt90.a: coord_transform.o
librt90.a: lmv_ctrl.o
librt90.a: version.o
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

# DO NOT DELETE

coordinate.o: coordinate.h
coord_transform.o: coordinate.h planar.h transform.h
lmv_ctrl.o: lmv_ctrl.h planar.h
planar.o: planar.h
rt90.o: transform.h planar.h coordinate.h lmv_ctrl.h direction.h
test_coord.o: coordinate.h transform.h planar.h
test_lmv.o: planar.h transform.h lmv_ctrl.h
test_parser.o: coordinate.h
transform.o: transform.h planar.h
