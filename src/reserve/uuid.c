/**
 * @brief Generating UUIDs.
 * @file uuid.h
 * @date: 2022-12-29
 * @author: Merlin Unterfinger
 */

#include "uuid.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

void generate_uuid(char *buffer) {
    static const char *chars = "0123456789abcdef";
    int i;

#ifdef _WIN32
    // Use the CryptGenRandom function to generate random bytes on Windows.
    HCRYPTPROV hCryptProv;
    if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL,
                             CRYPT_VERIFYCONTEXT)) {
        fprintf(stderr, "Error acquiring cryptographic context\n");
        exit(1);
    }

    // Seed the rand function with the random bytes.
    unsigned seed;
    if (!CryptGenRandom(hCryptProv, sizeof(seed), (BYTE *)&seed)) {
        fprintf(stderr, "Error generating random bytes\n");
        exit(1);
    }
    srand(seed);

    // Release the cryptographic context
    CryptReleaseContext(hCryptProv, 0);
#else
    // Use the /dev/urandom device to generate random bytes on Unix-like
    // systems.
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        perror("Error opening /dev/urandom");
        exit(1);
    }

    // Seed the rand function with the random bytes.
    unsigned seed;
    if (read(fd, &seed, sizeof(seed)) < 0) {
        perror("Error reading from /dev/urandom");
        exit(1);
    }
    srand(seed);

    // Close the /dev/urandom device.
    close(fd);
#endif

    for (i = 0; i < UUID_LEN; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            buffer[i] = '-';
        } else {
            buffer[i] = chars[rand() % 16];
        }
    }
}