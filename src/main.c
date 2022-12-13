/**
 * @brief Osurs CLI (tbd).
 * @file main.c
 * @date: 2022-07-19
 * @author: Merlin Unterfinger
 */

#include <osurs/io.h>
#include <osurs/reserve.h>

int main(int argc, char *argv[]) {
    if (argc > 0) {
        printf("usage: %s <arg>\n", argv[0]);
    }
    return 0;
}
