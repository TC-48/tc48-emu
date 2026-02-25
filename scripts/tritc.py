#!/usr/bin/env python
import math
import sys

from dataclasses import dataclass

@dataclass
class TritsInfo:
    bits_exact: float
    bits_needed: int
    unsigned_min: int
    unsigned_max: int
    signed_min: int
    signed_max: int

def trits_info(n: int) -> TritsInfo:
    bits_exact = n * math.log2(3)
    bits_needed = math.ceil(bits_exact)
    total = 3 ** n
    
    return TritsInfo(
        bits_exact=bits_exact,
        bits_needed=bits_needed,
        unsigned_min=0,
        unsigned_max=total - 1,
        signed_min=-(total // 2),
        signed_max=(total + 1) // 2 - 1
    )

def help():
    print('Usage:')
    print(f'{sys.argv[0]} <number-of-trits>')
    print('Example:')
    print(f'{sys.argv[0]} 24')

def main() -> int:
    if len(sys.argv) != 2:
        help()
        return 2
    if sys.argv[1] == '--help' or sys.argv[1] == '-h':
        help()
        return 0

    try:
        n = int(sys.argv[1])
    except ValueError:
        help()
        return 2
    except Exception as e:
        print(f'error: {e}')
        return 1

    if n < 0:
        print('error: number of trits must be non-negative')
        return 2

    info = trits_info(n)

    print('-- Bits:')
    print(f'Exact bits: {info.bits_exact}')
    print(f'Bits needed (ceil): {info.bits_needed}')

    print('\n-- Unsigned range:')
    print(f'{info.unsigned_min} .. {info.unsigned_max}')

    print('\n-- Signed range (ternary complement):')
    print(f'{info.signed_min} .. {info.signed_max}')
    return 0

if __name__ == '__main__':
    exit(main())
