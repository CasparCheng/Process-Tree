#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ptree.h"

int main(int argc, char **argv) {
    // TODO: Update error checking and add support for the optional -d flag
    // printf("Usage:\n\tptree [-d N] PID\n");
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "Usage:\n\t%s [-d N] PID\n", argv[0]);
        return 1; /* command line error */
    }

    int i_n;      /* index of N */
    int i_pid;    /* index of PID */
    int n;        /* N */
    int pid;      /* PID */
    char *endptr; /* end pointer used in strtol() */

    if (argc == 4) { /* locate -d flag */
        if (strcmp(argv[1], "-d") == 0) {
            i_n = 2;
            i_pid = 3;
        } else if (strcmp(argv[2], "-d") == 0) {
            i_n = 3;
            i_pid = 1;
        } else { /* -d flag not found */
            fprintf(stderr, "Usage:\n\t%s [-d N] PID\n", argv[0]);
            return 1; /* command line error */
        }
        n = strtol(argv[i_n], &endptr, 10);
        if (endptr == argv[i_n] || *endptr != '\0' || n < 0 || n > INT_MAX) {
            fprintf(stderr, "Usage:\n\t%s [-d N] PID\n", argv[0]);
            fprintf(stderr, "\tN must be an non-negative integer\n");
            return 1; /* command line error */
        }
    } else { /* no -d flag */
        n = 0;
        i_pid = 1;
    }

    pid = strtol(argv[i_pid], &endptr, 10);
    if (endptr == argv[i_pid] || *endptr != '\0' || pid < 0 || pid > INT_MAX) {
        fprintf(stderr, "Usage:\n\t%s [-d N] PID\n", argv[0]);
        fprintf(stderr, "\tPID must be an non-negative integer\n");
        return 1; /* command line error */
    }

    // NOTE: This only works if no -d option is provided and does not
    // error check the provided argument or generate_ptree. Fix this!
    // struct TreeNode *root = NULL;
    // generate_ptree(&root, strtol(argv[1], NULL, 10));
    // print_ptree(root, 0);

    int rc = 0;
    struct TreeNode *root = NULL;
    if (generate_ptree(&root, pid) != 0) { /* error occurs */
        rc = 2;
    }
    print_ptree(root, n);

    return rc;
}
