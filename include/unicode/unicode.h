#ifndef UNICODE_H
#define UNICODE_H

#include <stdint.h>

typedef uint_least8_t u8;
typedef uint_least16_t u16;
typedef uint_least32_t u32;
typedef uint_least64_t u64;

typedef u32 rune;
typedef u8 byte;

/**
 * @brief
 * 	These bit masks are used to mask the UTF-8 encoding
 * 	specific bits added at the start of an encoded byte.
 * 	These were proudly stolen from the go source ;).
 */
#define mask4 0b00000111
#define mask3 0b00001111
#define mask2 0b00011111
#define maskx 0b00111111

/** @brief
 * 	These bit masks represent how the first byte of a
 * 	UTF-8 encoded byte sequence would look like for byte
 * 	sequence of different lengths.
 */
#define b4 0b11110000
#define b3 0b11100000
#define b2 0b11000000
#define b1 0b00000000
#define bx 0b10000000

/**
 * @brief
 * 	Assuming that `b` is the first byte of a UTF-8 encoded
 * 	byte sequence, this function returns the number of bytes
 * 	that make up a single unicode codepoint/rune.
 */
u8 utf8_bytelen(byte b);

/**
 * @brief
 * 	Returns the number of bytes needed to encode `rune` to UTF-8.
 * 	This uses integer comparison to determine the number of bytes,
 * 	see <https://www.rfc-editor.org/info/rfc3629/#section-3>.
 */
u8 rune_bytelen(rune rune);

/**
 * @param utf8
 * 	A stream of UTF-8 encoded text.
 *
 * @param bytelen
 * 	The length of `utf8` stream in bytes.
 *
 * @return
 * 	Returns the number of runes in the `utf8` encoded text
 * 	stream & `0` on error.
 */
u32 rune_count(byte *utf8, u32 bytelen);

/**
 * @param runes
 * 	A stream of unicode codepoints which we would encode
 * 	in this function.
 *
 * @param runelen
 * 	The number of unicode codepoints in the `runes` stream.
 *
 * @return
 * 	Returns the number of bytes required to encode the
 * 	`runes` stream as `utf8` stream & `0` on error.
 */
u32 byte_count(rune *runes, u32 runelen);

/**
 * @param rune
 * 	Rune to be encoded to UTF-8
 *
 * @param bytelen
 * 	Specifies the number of bytes it would take to encode
 * 	`rune` to UTF-8. `utf8` buffer should be atleast this
 * 	large.
 *
 * @param utf8
 * 	A byte array atleast `bytelen` long to store the encoded
 * 	UTF-8 bytes. This is allocated/managed by the caller.
 *
 * @return
 * 	Returns `true` on success, `false` on error
 */
bool utf8_encode(rune rune, u8 bytelen, byte *utf8);

/**
 * @brief
 * 	Decodes a single rune from the UTF-8 stream `utf8`.
 * 	`runes` should point to the buffer to store the decoded
 * 	rune, and `bytelen` is the number of bytes to be
 * 	decoded from the UTF-8 stream (determined by `utf8_bytelen`).
 */
bool utf8_decode(const u8 *utf8, u8 bytelen, rune *runes);

/**
 * @param utf8
 * 	A stream of UTF-8 encoded text.
 *
 * @param bytelen
 * 	The length of `utf8` stream in bytes.
 *
 * @param runes
 * 	A buffer to store unicode codepoints from a decoded
 * 	UTF-8 stream. This is allocated by the caller and the
 * 	size can be obtained by calling `rune_count`.
 *
 * @param runelen
 * 	Length of the `runes` buffer. This can be obtained
 * 	by calling `rune_count`. This should be done by the
 * 	caller.
 *
 * @return
 * 	Returns `false` on error and `true` on success;
 */
bool utf8_decode_stream(byte *utf8, u32 bytelen, rune *runes, u32 runelen);

/**
 * @param runes
 * 	A stream of unicode codepoints which we would encode
 * 	in this function.
 *
 * @param runelen
 * 	The number of unicode codepoints in the `rune` stream.
 *
 * @param utf8
 * 	A buffer to store UTF-8 encoded text from the rune
 * 	stream. This is allocated by the caller and the
 * 	size can be obtained by calling `byte_count`.
 *
 * 	The buffer size however can be larger than `byte_count`
 * 	since there might be `\0` or `\n` at the end of the
 * 	byte stream.
 *
 * @return
 * 	Returns `false` on error and `true` on success;
 */
bool utf8_encode_stream(rune *runes, u32 runelen, byte *utf8);

/**
 * @brief
 * 	Checks whether `utf8` is a valid UTF-8 encoded stream.
 *
 * @param utf8
 * 	A pointer to a UTF-8 byte stream.
 *
 * @param bytelen
 * 	The number of bytes in the `utf8` stream
 *
 * @return
 * 	Returns `false` on error and `true` on success;
 */
bool valid_utf8(byte *utf8, u32 bytelen);

#endif
