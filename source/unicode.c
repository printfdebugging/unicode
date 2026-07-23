#include <stdio.h>

#include "unicode/unicode.h"

u8 utf8_bytelen(byte b) {
	if ((b & 0b11111000) == b4)
		return 4;
	if ((b & 0b11110000) == b3)
		return 3;
	if ((b & 0b11100000) == b2)
		return 2;
	if ((b & 0b10000000) == b1)
		return 1;

	fprintf(stderr, "ERROR: invalid utf8 byte\n");
	return 0;
}

u8 rune_bytelen(rune rune) {
	if (rune < 0x0000007F)
		return 1;
	if (0x00000080 < rune && rune < 0x000007FF)
		return 2;
	if (0x00000800 < rune && rune < 0x0000FFFF)
		return 3;
	if (0x00010000 < rune && rune < 0x0010FFFF)
		return 4;
	fprintf(stderr, "ERROR: invalid rune\n");
	return 0;
}

bool utf8_encode(rune rune, u8 bytelen, byte *utf8) {
	if (bytelen == 1) {
		utf8[0] = rune;
	} else if (bytelen == 2) {
		utf8[0] = b2 | ((rune >> 06) & mask2);
		utf8[1] = bx | (rune & maskx);
	} else if (bytelen == 3) {
		utf8[0] = b3 | ((rune >> 12) & mask3);
		utf8[1] = bx | ((rune >> 06) & maskx);
		utf8[2] = bx | (rune & maskx);
	} else if (bytelen == 4) {
		utf8[0] = b4 | ((rune >> 18) & mask4);
		utf8[1] = bx | ((rune >> 12) & maskx);
		utf8[2] = bx | ((rune >> 06) & maskx);
		utf8[3] = bx | (rune & maskx);
	} else {
		fprintf(stderr, "ERROR: bytelen cannot be anything other than 1,2,3,4\n");
		return false;
	}
	return true;
}

bool utf8_decode(const u8 *utf8, u8 bytelen, rune *runes) {
	if (bytelen == 4) {
		*runes = (utf8[0] & mask4) << 18 | (utf8[1] & maskx) << 12 | (utf8[2] & maskx) << 6 | (utf8[3] & maskx);
	} else if (bytelen == 3) {
		*runes = (utf8[0] & mask3) << 12 | (utf8[1] & maskx) << 6 | (utf8[2] & maskx);
	} else if (bytelen == 2) {
		*runes = (utf8[0] & mask2) << 6 | (utf8[1] & maskx);
	} else if (bytelen == 1) {
		*runes |= ((u32) utf8[0]);
	} else {
		fprintf(stderr, "ERROR: invalid bytelen, it should be one of 4, 3, 2, 1\n");
		return false;
	}
	return true;
}

u32 rune_count(byte *utf8, u32 bytelen) {
	u32 runelen = 0;
	u32 index = 0;

	while (index < bytelen) {
		u8 block_length = utf8_bytelen(utf8[index]);
		if (block_length == 0)
			return 0;
		index += block_length;
		runelen += 1;
	}
	return runelen;
}

u32 byte_count(rune *rune, u32 runelen) {
	u32 bytecount = 0;
	for (u32 runeidx = 0; runeidx < runelen; ++runeidx) {
		u32 bytelen = rune_bytelen(rune[runeidx]);
		if (bytelen == 0)
			return 0;
		bytecount += bytelen;
	}
	return bytecount;
}

bool utf8_decode_stream(byte *utf8, u32 bytelen, rune *runes, u32 runelen) {
	u8 byteseqlen = 0;
	for (u32 runeidx = 0, byteidx = 0; runeidx < runelen; ++runeidx, byteidx += byteseqlen) {
		byteseqlen = utf8_bytelen(utf8[byteidx]);
		runes[runeidx] = 0;
		if (!(utf8_decode(utf8 + byteidx, byteseqlen, runes + runeidx)))
			return false;
	}
	return true;
}

bool utf8_encode_stream(rune *runes, u32 runelen, byte *utf8) {
	u8 byteseqlen = 0;
	for (u32 runeidx = 0, byteidx = 0; runeidx < runelen; ++runeidx, byteidx += byteseqlen) {
		byteseqlen = rune_bytelen(runes[runeidx]);
		if (!(utf8_encode(runes[runeidx], byteseqlen, utf8 + byteidx)))
			return false;
	}
	return true;
}

bool valid_utf8(byte *utf8, u32 bytelen) {
	fprintf(stderr, "ERROR: not implemented\n");
	return 0;
}
