#ifndef BASE_H
#define BASE_H

/*
 * Evaluates a base-conversion expression, e.g. "bin(25)", "hex(255)",
 * "oct(64)", "dec(1111b)", and writes the result into
 * result[resultSize]. Returns 1 on success, 0 (with an error message
 * in result) on failure.
 *
 *   bin(N)   decimal integer N -> binary string
 *   hex(N)   decimal integer N -> hexadecimal string (uppercase)
 *   oct(N)   decimal integer N -> octal string
 *   dec(X)   X is digits followed by a base suffix -> decimal integer
 *            suffix 'b'/'B' = binary, 'o'/'O' = octal, 'h'/'H' = hex
 *            e.g. dec(1111b), dec(377o), dec(FFh)
 *
 * Negative integers are supported for bin/hex/oct as sign-magnitude
 * (a leading '-' on the output), not two's complement — two's
 * complement requires a fixed bit width, which this calculator
 * doesn't otherwise track, so sign-magnitude keeps the behavior
 * unambiguous.
 */
int evaluateBaseExpression(const char expr[], char result[], int resultSize);

#endif