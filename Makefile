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
	$(RM) Makefile.bak

.PHONY: check checkv
check: 
checkv: 

%.1.ps : %.1
	groff -man -ma4 $< >$@
%.1.pdf : %.1.ps
	ps2pdf $< >$@

love:
	@echo "not war?"
