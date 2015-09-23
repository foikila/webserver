#ifndef LOGGING_H
#define LOGGING_H

/**
 * Logs the message to STDOUT.
 * @param  msg The message to log
 * @return Minus 1 if not successful else 0.
 */
int log_fail(char* msg);

/**
 * Logs the message to STDOUT.
 * @param  msg The message to log
 * @return Minus 1 if not successful else 0.
 */
int log_success(char* msg);

#endif
