#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# rt90 - coordinate conversion between RT90 and SWEREF99
#
# Copyright (c) 2012 Jörgen Grahn
# All rights reserved.
#
"coordinate conversion between RT90 and SWEREF99"

import os
import os.path
import getopt
import sys
import subprocess


class Accuracy:
    """RT90 accuracy for both input and output, i.e.  whether they
    should be output in 4--7 digits (1km--1m), and which number of
    digits should be tolerated.
    """
    _division = { 7: 1e0, 6: 1e1, 5: 1e2, 4: 1e3 } 
    def __init__(self, a):
        "Init from a sequence of numbers."
        self.output = a[-1]
        self.div = self._division[self.output]
        self.input = set(a)
    def fmt_rt90(self, coord):
        "Format a tuple as RT 90, using the preferred accuracy."
        coord = [ x/self.div for x in coord ]
        return '%.0f %.0f' % tuple(coord)
    def fmt_sweref99(self, coord):
        "Format a tuple as SWEREF 99, e.g. '7454204 761811'."
        return '%.0f %.0f' % coord
    def normalize(self, (x, y)):
        """Scale an RT90 tuple to 1m resolution.
        """
        if x<1e4: factor=1e3
        elif x<1e5: factor=1e2
        elif x<1e6: factor=1e1
        elif x<1e7: factor=1e0
        return x*factor, y*factor

class Proj:
    """Invoking cs2cs(1), really.  Not instantiated
    directly; just the common parts of Forward and Backward.
    """
    _rt90_sweref99 = ['+ellps=WGS84',
                      '+proj=tmerc',
                      '+lat_0=0',
                      '+lon_0=15.80628452944445',
                      '+k=1.00000561024',
                      '+x_0=1500064.274',
                      '+y_0=-667.711']
    _sweref99_planar = ['+init=epsg:3006']
    def __call__(self, coord):
        """Perform the translation, by calling proj's cs2cs(1).
        Input is a tuple, and treated as three-decimal floats
        (millimeter resolution). That's also the output
        resolution we request.

        On error, None is returned.
        """
        try:
            return self._call(coord)
        except:
            return None
    def _call(self, coord):
        "The actual implementation, which may throw."
        # It would be nice to exec cs2cs once and use it for
        # all translations, but I'm unsure if that would be safe;
        # writing and reading to pipes can cause deadlock if the
        # called program isn't carefully written, and I doubt
        # whether cs2cs is.
        coord = '%.3f %.3f\n' % tuple(coord)
        sub = subprocess.Popen(self._args, bufsize=1,
                               stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT,
                               close_fds=True)
        sub.stdin.write(coord)
        sub.stdin.close()
        s = sub.stdout.readline()
        rc = sub.wait()
        x, y, z = s.split()
        return float(x), float(y)

class Forward(Proj):
    """Conversion RT90 2.5 gon V --> SWEREF 99 TM.
    """
    _args = ['cs2cs', '-rs'] + Proj._rt90_sweref99 + \
        ['+to'] + Proj._sweref99_planar + ['-f', '%.3f']

class Backward(Proj):
    """Conversion SWEREF 99 TM --> RT90 2.5 gon V.
    """
    _args = ['cs2cs', '-rs'] + Proj._sweref99_planar + \
        ['+to'] + Proj._rt90_sweref99 + ['-f', '%.3f']

class Convert:
    """Parse and convert a line of text as a coordinate, according to
    'direction' and 'accuracy'. Deals with strings only.
    """
    def __init__(self, direction, accuracy):
        self._from = Forward()
        self._to   = Backward()
        if direction=='from': self._to = None
        elif direction=='to': self._from = None
        self._accuracy = accuracy
    def _valid_rt90(self, ss):
        nn = [ len(x) for x in ss ]
        return nn[0] == nn[1] and nn[0] in self._accuracy.input
    def _valid_sweref99(self, ss):
        nn = [ len(x) for x in ss ]
        return nn == [7, 6]
    def convert(self, s):
        """Try to convert 's' as a coordinate.  If it makes sense,
        return True and set 'self.coord'. Otherwise, may set 'self.err'
        to indicate a failed conversion attempt.
        """
        self.err = False
        ss = s.split()
        if len(ss) != 2: return False
        try:
            coord = [ int(x) for x in ss ]
        except ValueError:
            return False
        translation = None
        if self._valid_sweref99(ss):
            translation = self._to
            fmt = self._accuracy.fmt_rt90
        elif self._valid_rt90(ss):
            translation = self._from
            fmt = self._accuracy.fmt_sweref99
            coord = self._accuracy.normalize(coord)
        if not translation: return False

        result = translation(coord)
        self.err = result is None
        if self.err: return False

        self.coord = fmt(result)
        return True


def selftest():
    """Perform self-test using LMV's four test coordinates. If both
    RT90 -> SWEREF99 and SWEREF99 -> RT90 is correct to 0.1 m resolution,
    returns 0 and prints nothing.
    """
    def close_enough(a, b):
        if not a or not b: return None
        dx = a[0]-b[0]
        dy = a[1]-b[1]
        return dx*dx + dy*dy < 0.1
    #              RT 90 2.5 gon V             SWEREF 99 TM
    data = (('A', (7453389.762, 1727060.905), (7454204.638, 761811.242)),
            ('B', (7047738.415, 1522128.637), (7046077.605, 562140.337)),
            ('C', (6671665.273, 1441843.186), (6669189.376, 486557.055)),
            ('D', (6249111.351, 1380573.079), (6246136.458, 430374.835)))
    rc = 0
    forward = Forward()
    backward = Backward()
    out = sys.stdout.write
    for name, rt90, sweref99 in data:
        if not close_enough(forward(rt90), sweref99):
            out('Reference point %s failed conversion to SWEREF 99\n' % name)
            out('input was       : %s\n' % `rt90`)
            out('output should be: %s\n' % `sweref99`)
            out('but output was  : %s\n' % `forward(rt90)`)
            rc = 1
        if not close_enough(backward(sweref99), rt90):
            out('Reference point %s failed conversion to RT90\n' % name)
            out('input was       : %s\n' % `sweref99`)
            out('output should be: %s\n' % `rt90`)
            out('but output was  : %s\n' % `backward(sweref99)`)
            rc = 1
    return rc


def convert(direction, accuracy, coord):
    """As 'convert_f' on a file containing only one line, 'coord'.
    """
    rc = 0
    cvt = Convert(direction, accuracy)
    out = sys.stdout.write
    err = sys.stderr.write
    if cvt.convert(coord):
        out('%s\n' % cvt.coord)
    else:
        err('conversion failed\n')
        rc = 1
    return rc


def convert_f(direction, accuracy, f):
    """Convert coordinates in file-like object 'f' and write to
    standard output, using 'accuracy' for RT90 accuracy.
    Direction is "from" RT90, "to" RT90, or None (convert in either
    direction). Returns a shell exit code.
    """
    rc = 0
    n = 0
    cvt = Convert(direction, accuracy)
    out = sys.stdout.write
    err = sys.stderr.write
    while 1:
        s = f.readline()
        if not s: break
        n += 1
        if cvt.convert(s):
            out('%s\n' % cvt.coord)
        elif cvt.err:
            err('conversion error on line %d\n' % n)
            out(s)
            rc = 1
        else:
            out(s)
    return rc


if __name__ == "__main__":
    prog = os.path.split(sys.argv[0])[1]
    usage = ['[-4567] [north east]',
             '[--from | --to] [-4567] [north east]',
             '--test',
             '--version',
             '--help']
    usage = [ '%s %s' % (prog, x) for x in usage ]
    usage = '\n       '.join(usage)
    usage = '%s: %s' % ('usage', usage)

    try:
        opts, argv = getopt.getopt(sys.argv[1:],
                                   '4567',
                                   ('from', 'to', 'back',
                                    'test',
                                    'version', 'help'))
        version = None
        help = None
        test = None
        accuracy = []
        direction = None
        for option, _ in opts:
            if   option == '-4': accuracy.append(4)
            elif option == '-5': accuracy.append(5)
            elif option == '-6': accuracy.append(6)
            elif option == '-7': accuracy.append(7)
            elif option == '--from': direction = 'from'
            elif option in ('--to', '--back'): direction = 'to'
            elif option == '--test': test = 1
            elif option == '--version': version = 1
            elif option == '--help': help = 1
        if len(argv) not in (0, 2):
            raise ValueError('wrong number of arguments')
        coord = argv
        if version:
            print '%s, unknown version' % prog
            print 'Copyright (C) 2012 Jörgen Grahn'
            sys.exit(0)
        if help:
            print usage
            sys.exit(0)
    except (ValueError, getopt.GetoptError), s:
        print >>sys.stderr, 'error:', s
        print >>sys.stderr, usage
        sys.exit(1)

    if test:
        sys.exit(selftest())

    if not accuracy:
        accuracy = [5, 6, 7]
    accuracy = Accuracy(accuracy)

    if coord:
        coord = ' '.join(coord)
        sys.exit(convert(direction, accuracy, coord))
    else:
        sys.exit(convert_f(direction, accuracy, sys.stdin))
