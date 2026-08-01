/**
 * @file base.h
 * @brief Number base conversion: bin(), oct(), hex(), dec().
 */
#ifndef BASE_H
#define BASE_H

/**
 * @brief Evaluates a base-conversion expression and writes the result
 * into @p result.
 *
 * Supported forms:
 *   - `bin(N)`  decimal integer N -> binary string
 *   - `hex(N)`  decimal integer N -> hexadecimal string (uppercase)
 *   - `oct(N)`  decimal integer N -> octal string
 *   - `dec(X)`  X is digits followed by a base suffix -> decimal integer;
 *     suffix 'b'/'B' = binary, 'o'/'O' = octal, 'h'/'H' = hex
 *     (e.g. `dec(1111b)`, `dec(377o)`, `dec(FFh)`)
 *
 * Negative integers are supported for bin/hex/oct as sign-magnitude
 * (a leading '-' on the output), not two's complement -- two's
 * complement requires a fixed bit width, which this calculator
 * doesn't otherwise track, so sign-magnitude keeps the behavior
 * unambiguous.
 *
 * @param expr       Expression such as "hex(255)".
 * @param result     Destination buffer for the formatted result (on
 *                   success) or an error message (on failure).
 * @param resultSize Size of @p result in bytes.
 * @return 1 on success, 0 on failure. */
int evaluateBaseExpression(const char expr[], char result[], int resultSize);

#endif
