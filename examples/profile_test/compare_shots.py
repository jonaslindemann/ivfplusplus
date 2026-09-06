#!/usr/bin/env python
#
# Compares the images profile_test --shot writes for two render profiles.
#
# "No GL errors" is not the same as "draws the same picture", and the difference
# between those two is where the migration actually goes wrong: a class whose
# modern path never uploads its matrices draws nothing at all and reports a
# perfectly clean error count. This catches that.
#
# Usage:
#   mkdir shots
#   profile_test legacy --shot shots
#   profile_test mixed  --shot shots
#   python compare_shots.py shots [legacy] [mixed]
#
# Run each case in isolation (--only NAME) when you want to be sure a class
# stands on its own. Rendered back to back, an unconverted class picks up the
# uniforms the previous object left behind and can look almost right.
#
# Requires nothing beyond the standard library -- profile_test writes 24-bit BMP
# precisely so this stays dependency-free.

import os
import struct
import sys


def read_bmp(path):
    with open(path, 'rb') as f:
        data = f.read()

    offset = struct.unpack_from('<I', data, 10)[0]
    width = struct.unpack_from('<i', data, 18)[0]
    height = struct.unpack_from('<i', data, 22)[0]
    bpp = struct.unpack_from('<H', data, 28)[0]

    if bpp != 24:
        raise ValueError('%s: expected 24-bit BMP, got %d' % (path, bpp))

    row_bytes = width * 3
    padding = (4 - (row_bytes % 4)) % 4

    pixels = bytearray()
    for y in range(height):
        start = offset + y * (row_bytes + padding)
        pixels += data[start:start + row_bytes]

    return width, height, bytes(pixels)


def compare(path_a, path_b):
    width, height, a = read_bmp(path_a)
    _, _, b = read_bmp(path_b)

    if len(a) != len(b):
        raise ValueError('image sizes differ')

    total = 0
    differing = 0
    largest = 0
    lit_a = 0
    lit_b = 0

    for i in range(0, len(a), 3):
        delta = (abs(a[i] - b[i]) +
                 abs(a[i + 1] - b[i + 1]) +
                 abs(a[i + 2] - b[i + 2]))
        total += delta

        # A threshold, because shading the same surface per-fragment instead of
        # per-vertex moves almost every pixel by a little. Only count a pixel as
        # differing when it moved by more than that noise floor.

        if delta > 12:
            differing += 1
        if delta > largest:
            largest = delta

        if a[i] or a[i + 1] or a[i + 2]:
            lit_a += 1
        if b[i] or b[i + 1] or b[i + 2]:
            lit_b += 1

    count = len(a) // 3
    return {
        'mean': total / (count * 3.0),
        'percent': 100.0 * differing / count,
        'max': largest,
        'lit_a': lit_a,
        'lit_b': lit_b,
    }


def main():
    directory = sys.argv[1] if len(sys.argv) > 1 else 'shots'
    profile_a = sys.argv[2] if len(sys.argv) > 2 else 'legacy'
    profile_b = sys.argv[3] if len(sys.argv) > 3 else 'mixed'

    prefix_a = profile_a + '_'
    names = sorted(fn[len(prefix_a):-4] for fn in os.listdir(directory)
                   if fn.startswith(prefix_a) and fn.endswith('.bmp'))

    rows = []
    for name in names:
        path_a = os.path.join(directory, prefix_a + name + '.bmp')
        path_b = os.path.join(directory, profile_b + '_' + name + '.bmp')

        if not os.path.exists(path_b):
            continue

        rows.append((name, compare(path_a, path_b)))

    rows.sort(key=lambda r: -r[1]['percent'])

    print('%-18s %8s %8s %6s %8s %8s  %s' %
          ('case', 'meanAbs', 'diff%', 'max', profile_a[:8], profile_b[:8], 'note'))
    print('-' * 78)

    failures = 0

    for name, r in rows:
        note = ''

        if r['lit_a'] > 0 and r['lit_b'] == 0:
            note = '%s DRAWS NOTHING' % profile_b.upper()
        elif r['lit_b'] > 0 and r['lit_a'] == 0:
            note = '%s DRAWS NOTHING' % profile_a.upper()
        elif r['lit_a'] > 0 and abs(r['lit_a'] - r['lit_b']) > 0.25 * r['lit_a']:
            note = 'coverage differs'

        if note:
            failures += 1

        print('%-18s %8.2f %7.1f%% %6d %8d %8d  %s' %
              (name, r['mean'], r['percent'], r['max'], r['lit_a'], r['lit_b'], note))

    print('-' * 78)
    print('%d cases, %d with a coverage problem' % (len(rows), failures))

    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
