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
 * @param utf8
 * 	A stream of UTF-8 encoded text.
 *
 * @param utf8_length
 * 	The length of `utf8` stream in bytes.
 *
 * @return
 * 	Returns the number of runes in the `utf8` encoded text
 * 	stream & `0` on error.
 */
u32 rune_count(byte *utf8, u32 utf8_length);

/**
 * @param rune
 * 	A stream of unicode codepoints which we would encode
 * 	in this function.
 *
 * @param rune_length
 * 	The number of unicode codepoints in the `rune` stream.
 *
 * @return
 * 	Returns the number of bytes required to encode the
 * 	`rune` stream as `utf8` stream & `0` on error.
 */
u32 byte_count(rune *rune, u32 rune_length);

/**
 * @param utf8
 * 	A stream of UTF-8 encoded text.
 *
 * @param utf8_length
 * 	The length of `utf8` stream in bytes.
 *
 * @param rune
 * 	A buffer to store unicode codepoints from a decoded
 * 	UTF-8 stream. This is allocated by the caller and the
 * 	size can be obtained by calling `rune_count`.
 *
 * @param rune_length
 * 	Length of the `rune` buffer. This can be obtained
 * 	by calling `rune_count`. This should be done by the
 * 	caller.
 *
 * @return
 * 	Returns `false` on error and `true` on success;
 */
bool utf8_to_rune(byte *utf8, u32 utf8_length, rune *rune, u32 rune_length);

/**
 * @param rune
 * 	A stream of unicode codepoints which we would encode
 * 	in this function.
 *
 * @param rune_length
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
bool rune_to_utf8(rune *rune, u32 rune_length, byte *utf8);

/**
 * @brief
 * 	Checks whether `utf8` is a valid UTF-8 encoded stream.
 *
 * @param utf8
 * 	A pointer to a UTF-8 byte stream.
 *
 * @param utf8_length
 * 	The number of bytes in the `utf8` stream
 *
 * @return
 * 	Returns `false` on error and `true` on success;
 */
bool utf8_is_valid(byte *utf8, u32 utf8_length);

#endif
