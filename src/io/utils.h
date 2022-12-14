/**
 * @brief IO helper and utils methods.
 * @file utils.h
 * @date: 2022-07-19
 * @author: Merlin Unterfinger
 */

#ifndef OSURS_IO_UTILS_H_
#define OSURS_IO_UTILS_H_

/**
 * @brief
 *
 * Needs buffer size of char[9], due to string terminating sign.
 *
 * @param buffer
 * @param time
 * @return int
 */
int compose_time(char *buffer, int time);

/**
 * @brief
 *
 * @param time
 * @return int
 */
int parse_time(char *time);

#endif  // OSURS_IO_UTILS_H_