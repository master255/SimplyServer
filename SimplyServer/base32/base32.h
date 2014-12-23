/*
 * $Id$
 */

#ifndef __BASE32_H
#define __BASE32_H

/*
 * Returns minimal length of a buffer sufficient to hold
 * the result of applying base32 transformation to an array
 * of x characters. Accounts for NUL-terminator.
 * Can be used. e.g. to calculate the required length
 * of the dst array passed to to_base32 function.
 */
#define BASE32_DESTLEN(x) ((x * sizeof(char)) * 8 / 5 + 1)

char *to_base32(const unsigned char src[], const size_t len,
		char *const dst);

#endif /* __BASE32_H */

