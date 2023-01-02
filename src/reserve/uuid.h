/**
 * @brief Generating UUIDs.
 * @file uuid.h
 * @date: 2022-12-29
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_RESERVE_UUID_H_
#define OSURS_RESERVE_UUID_H_

/**
 * @brief Length of a UUID string, including hyphens.
 *
 * The length of a UUID string is 36 characters, including the four hyphens that
 * separate the different parts of the UUID.
 */
#define UUID_LEN 36

/**
 * @brief Generates a unique UUID using a secure random number generator.
 *
 * The UUID is generated using a secure random number generator, such as
 * `/dev/urandom` on Unix-like systems or the `CryptGenRandom` function on
 * Windows. The UUID is stored in the character array pointed to by `buffer`.
 * The array must be at least UUID_LEN + 1 bytes long.
 *
 * @param buffer Pointer to a character array where the generated UUID will be
 * stored.
 */
void generate_uuid(char *buffer);

#endif  // OSURS_RESERVE_UUID_H_