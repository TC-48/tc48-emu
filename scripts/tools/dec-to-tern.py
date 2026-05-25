#!/usr/bin/python
import argparse
import sys

def dec_to_tern(num: int, padding: int = 0) -> str:
    if num == 0:
        return '0' * padding if padding >= 0 else '0'

    trits: list[str] = []
    while num > 0:
        trits.append(str(num % 3))
        num //= 3

    tern = ''.join(reversed(trits))
    tern = tern.rjust(padding, '0')
    return tern

def main() -> int:
    parser = argparse.ArgumentParser(
        prog='dec-to-tern',
        description='convert decimal numbers to ternary',
    )

    parser.add_argument(
        'number', type=int, help='the decimal integer to convert to ternary'
    );
    parser.add_argument(
        '-p', '--padding',
        type=int, default=0,
        help='minimum trit length for alignment',
    )

    args = parser.parse_args()
    if args.padding < 0 or args.number < 0:
        print('error: input cannot be negative')
        return 1

    print(dec_to_tern(args.number, padding=args.padding))
    return 0

if __name__ == '__main__':
    sys.exit(main())
