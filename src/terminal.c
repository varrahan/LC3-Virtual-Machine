#include "terminal.h"
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

struct termios original_tio;

// Disable terminal input buffering for immediate character input
void disable_input_buffering(void) {
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

// Restore original terminal settings
void restore_input_buffering(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

// Check if a key has been pressed
uint16_t check_key(void) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &timeout) != 0;
}