#include <stdio.h>

#include "unicode/unicode.h"

u32 rune_count(byte *utf8, u32 bytelen) {
	fprintf(stderr, "ERROR: not implemented\n");
	return 0;
}

u32 byte_count(rune *rune, u32 runelen) {
	fprintf(stderr, "ERROR: not implemented\n");
	return 0;
}

bool utf8_decode_stream(byte *utf8, u32 bytelen, rune *rune, u32 runelen) {
	fprintf(stderr, "ERROR: not implemented\n");
	return 0;
}

bool utf8_encode_stream(rune *rune, u32 runelen, byte *utf8) {
	fprintf(stderr, "ERROR: not implemented\n");
	return 0;
}

bool valid_utf8(byte *utf8, u32 bytelen) {
	fprintf(stderr, "ERROR: not implemented\n");
	return 0;
}
