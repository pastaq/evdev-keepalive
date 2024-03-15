/*
 * gcc -Wall -O2 -o evdev_keepalive evdev_keepalive.c
 */

#include <stdio.h>

#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void null_action(int signal, siginfo_t *info, void *data) {
    /* Nothing here. */
}

int main(int argc, char *argv[]) {
    char buffer[32];
    int fd;
    sigset_t set;
    struct sigaction action;

    if (argc <= 1) {
        fprintf(stderr, "error: missing kernel name argument\n");

        return -1;
    }

    snprintf(buffer, sizeof(buffer), "/dev/input/%s", argv[1]);

    fprintf(stdout, "info: keeping %s open...\n", buffer);
    fflush(stdout);

    fd = open(buffer, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "error: failed to open: %s\n", buffer);

        return -2;
    }

    sigemptyset(&set);
    memset(&action, 0x00, sizeof(struct sigaction));

    action.sa_sigaction = null_action;
    action.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);

    sigsuspend(&set);

    fprintf(stdout, "info: closing %s...\n", buffer);

    close(fd);

    return 0;
}
