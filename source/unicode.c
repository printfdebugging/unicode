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
		return false;
	}
	return true;
}

u32 rune_count(byte *utf8, u32 bytelen) {
	u32 runelen = 0;
	for (u32 index = 0, byteseqlen = 0; index < bytelen; index += byteseqlen, ++runelen) {
		if ((byteseqlen = utf8_bytelen(utf8[index])) == 0)
			return 0;
		if (!valid_utf8(utf8 + index, byteseqlen))
			return 0;
	}
	return runelen;
}

u32 byte_count(rune *rune, u32 runelen) {
	u32 bytecount = 0;
	for (u32 runeidx = 0, bytelen = 0; runeidx < runelen; bytecount += bytelen, ++runeidx)
		if ((bytelen = rune_bytelen(rune[runeidx])) == 0)
			return 0;
	return bytecount;
}

bool utf8_decode_stream(byte *utf8, u32 bytelen, rune *runes, u32 runelen) {
	for (u32 runeidx = 0, byteidx = 0, byteseqlen = 0; runeidx < runelen; ++runeidx, byteidx += byteseqlen) {
		runes[runeidx] = 0;
		if ((byteseqlen = utf8_bytelen(utf8[byteidx])) == 0)
			return false;
		if (!valid_utf8(utf8 + byteidx, byteseqlen))
			return false;
		if (!(utf8_decode(utf8 + byteidx, byteseqlen, runes + runeidx)))
			return false;
	}
	return true;
}

bool utf8_encode_stream(rune *runes, u32 runelen, byte *utf8) {
	for (u32 runeidx = 0, byteidx = 0, byteseqlen = 0; runeidx < runelen; ++runeidx, byteidx += byteseqlen) {
		if ((byteseqlen = rune_bytelen(runes[runeidx])) == 0)
			return false;
		if (!(utf8_encode(runes[runeidx], byteseqlen, utf8 + byteidx)))
			return false;
	}
	return true;
}

/*
 * https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G27506
 * +---------------------+-------------+-------------+------------+------------+
 * |     Code Points     | First Byte  | Second Byte | Third Byte | Fourth Byte|
 * | U+0000..U+007F      | 00..7F      |             |            |            |
 * | U+0080..U+07FF      | C2..DF      | 80..BF      |            |            |
 * +---------------------+-------------+-------------+------------+------------+
 * | U+0800..U+0FFF      | E0          | A0..BF      | 80..BF     |            |
 * | U+1000..U+CFFF      | E1..EC      | 80..BF      | 80..BF     |            |
 * | U+D000..U+D7FF      | ED          | 80..9F      | 80..BF     |            |
 * | U+E000..U+FFFF      | EE..EF      | 80..BF      | 80..BF     |            |
 * +---------------------+-------------+-------------+------------+------------+
 * | U+10000..U+3FFFF    | F0          | 90..BF      | 80..BF     | 80..BF     |
 * | U+40000..U+FFFFF    | F1..F3      | 80..BF      | 80..BF     | 80..BF     |
 * | U+100000..U+10FFFF  | F4          | 80..8F      | 80..BF     | 80..BF     |
 * +---------------------+-------------+-------------+------------+------------+
 */
bool valid_utf8(byte *utf8, u8 bytelen) {
	switch (bytelen) {
		case 1:
			return byte_in_range(utf8[0], 0x00, 0x7f);
		case 2:
			return byte_in_range(utf8[0], 0xc2, 0xdf) && byte_in_range(utf8[1], bx, bf);
		case 3:
			return byte_in_range(utf8[2], bx, bf) &&
			    ((byte_in_range(utf8[1], bx, bf) && (byte_in_range(utf8[0], 0xe1, 0xec) || byte_in_range(utf8[0], 0xee, 0xef))) ||
			     (byte_in_range(utf8[1], 0xa0, 0xbf) && utf8[0] == b3) ||
			     (byte_in_range(utf8[1], bx, 0x9f) && utf8[0] == 0xed));
		case 4:
			return (byte_in_range(utf8[3], bx, bf) && byte_in_range(utf8[2], bx, bf)) &&
			    ((byte_in_range(utf8[1], 0x90, bf) && utf8[0] == 0xf0) ||
			     (byte_in_range(utf8[1], bx, bf) && byte_in_range(utf8[0], 0xf1, 0xf3)) ||
			     (byte_in_range(utf8[1], bx, 0x8f) && utf8[0] == 0xf4));
		default:
			return false;
	}
}

bool valid_utf8_stream(byte *utf8, u32 bytelen) {
	for (u32 idx = 0, seqlen = 0; idx < bytelen; idx += seqlen) {
		if ((seqlen = utf8_bytelen(utf8[idx])) == 0)
			return false;
		if (!valid_utf8(utf8 + idx, seqlen))
			return false;
	}
	return true;
}
