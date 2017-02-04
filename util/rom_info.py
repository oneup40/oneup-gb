#!/usr/bin/env python2

import hashlib, os, string, struct, sys

def unbin(s):
    res = ''
    for c in s:
        if c in string.printable:
            res += c
        else:
            res += '\\x%.2x' % (ord(c,))

    return res

if len(sys.argv) < 2:
    progname = sys.argv[0] if sys.argv else 'rom_info.py'
    print >>sys.stderr, 'usage:'
    print >>sys.stderr, '   %s ROM [ROM] [ROM] ...' % (progname,)
    sys.exit(1)

for i,filename in enumerate(sys.argv[1:]):
    with open(filename, 'rb') as f:
        data = f.read()

        print '      File name: %s' % (os.path.basename(filename),)
        print '          Title: %s' % (unbin(data[0x134:0x144]),)

        hdr_cksum = ord(data[0x14D])
        print 'Header checksum: %.2X' % (hdr_cksum,)

        cksum, = struct.unpack('>H', data[0x14E:0x150])
        print '       Checksum: %.4X' % (cksum,)

        print '            MD5: %s' % (hashlib.md5(data).hexdigest(),)
        print '           SHA1: %s' % (hashlib.sha1(data).hexdigest(),)

        if i != len(sys.argv) - 2:
            print
