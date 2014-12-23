/*
 * $Id$
 */

#include <stdlib.h>
#include <assert.h>
#include "base32.h"

typedef unsigned char uint8_t;

/*
 * Code snagged from the bitzi bitcollider
 *
 * Note:
 *   The output array (dst) must be at least
 *   len * 8 / 5 + 1 characters long.
*/
char *to_base32(const unsigned char src[], const size_t len,
		char *const dst) {
	static const char base32abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

	size_t si, index, di;
	uint8_t word;

	si = di = index = 0;

	while (si < len) {
		/* Is the current word going to span a byte boundary? */
		if (index > 3) {
			word = (uint8_t)(src[si] & (0xFF >> index));
			index = (index + 5) % 8;
			word <<= index;
			if ((si + 1) < len)
				word |= src[si + 1] >> (8 - index);
			si++;
		} else {
			word = (uint8_t)(src[si] >> (8 - (index + 5))) & 0x1F;
			index = (index + 5) % 8;
			if (index == 0)
				si++;
		}

		assert(word < 32);
		dst[di] = base32abc[word];
		++di;
	}

	dst[di] = 0;
	return dst;
}

