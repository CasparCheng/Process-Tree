#include <stdio.h>
// Add your other system includes here.
#include <stdlib.h>
#include <sys/stat.h>

#include "ptree.h"

// Defining the constants described in ptree.h
const unsigned int MAX_PATH_LENGTH = 1024;

// If TEST is defined (see the Makefile), will look in the tests
// directory for PIDs, instead of /proc.
#ifdef TEST
const char *PROC_ROOT = "tests";
#else
const char *PROC_ROOT = "/proc";
#endif

/*
 * Creates a PTree rooted at the process pid.
 * The function returns 0 if the tree was created successfully
 * and 1 if the tree could not be created or if at least
 * one PID was encountered that could not be found or was not an
 * executing process.
 */
int generate_ptree(struct TreeNode **root, pid_t pid) {
    // Here's a way to generate a string representing the name of
    // a file to open. Note that it uses the PROC_ROOT variable.

    /* generate exe file path */
    char procfile[MAX_PATH_LENGTH + 1];
    if (sprintf(procfile, "%s/%d/exe", PROC_ROOT, pid) < 0) {
        fprintf(stderr, "sprintf failed to produce a filename\n");
        return 1;
    }

    struct stat buf; /* file stat structure */

    if (lstat(procfile, &buf) == 0) { /* exe symbolic link exists */
        /* create node */
        if ((*root = (struct TreeNode *)malloc(sizeof(struct TreeNode))) ==
            NULL) {
            fprintf(stderr, "malloc failed to allocate memory\n");
            return 1;
        }
        /* initialize all fields */
        (*root)->pid = pid;
        (*root)->name = NULL;
        (*root)->child = NULL;
        (*root)->sibling = NULL;

        /* generate cmdline file path */
        char cmdlinefile[MAX_PATH_LENGTH + 1];
        if (sprintf(cmdlinefile, "%s/%d/cmdline", PROC_ROOT, pid) <
            0) { /* failed to generate string */
            fprintf(stderr, "sprintf failed to produce a filename\n");
            free(*root);
            *root = NULL;
            return 1;
        }

        if (stat(cmdlinefile, &buf) == 0) { /* cmdline file exists */
            FILE *fp;                       /* file pointer */
            if ((fp = fopen(cmdlinefile, "r")) == NULL) { /* failed to read */
                fprintf(stderr, "fopen failed to read file\n");
                free(*root);
                *root = NULL;
                return 1;
            }
            if (((*root)->name = malloc(MAX_PATH_LENGTH + 1)) ==
                NULL) { /* failed to allocate memory */
                fprintf(stderr, "malloc failed to allocate memory\n");
                free(*root);
                *root = NULL;
                fclose(fp); /* close file */
                return 1;
            }
            if (fgets((*root)->name, MAX_PATH_LENGTH + 1, fp) ==
                NULL) { /* empty cmdline file */
                free((*root)->name);
                (*root)->name = NULL;
            }
            fclose(fp); /* close file */
        }

        /* generate children file path */
        char childfile[MAX_PATH_LENGTH + 1];
        if (sprintf(childfile, "%s/%d/task/%d/children", PROC_ROOT, pid, pid) <
            0) { /* failed to generate string */
            fprintf(stderr, "sprintf failed to produce a filename\n");
            return 1;
        }

        int rc = 0; /* return code */

        if (stat(childfile, &buf) == 0) { /* children file exists */
            FILE *fp;                                   /* file pointer */
            if ((fp = fopen(childfile, "r")) == NULL) { /* failed to read */
                fprintf(stderr, "fopen failed to read file\n");
                return 1;
            }
            int first = 1;
            /* read pids of child processes one by one */
            while (fscanf(fp, "%d", &pid) == 1) {
                if (first) { /* the first child process */
                    if (generate_ptree(&((*root)->child), pid) == 0) {
                        first = 0;
                    } else {
                        rc = 1;
                    }
                } else { /* the rest child processes */
                    struct TreeNode *n = (*root)->child;
                    while (n->sibling) { /* find the last sibling slot */
                        n = n->sibling;
                    }
                    if (generate_ptree(&(n->sibling), pid) != 0) {
                        rc = 1;
                    }
                }
            }
            fclose(fp); /* close file */
        }

        return rc;
    } else { /* children file not found */
        return 1;
    }

    // You will want to remove this line eventually since this
    // function should not print anything.
    // printf("%s\n", procfile);
}

/*
 * Prints the TreeNodes encountered on a preorder traversal of an PTree
 * to a specified maximum depth. If the maximum depth is 0, then the
 * entire tree is printed.
 */
void print_ptree(struct TreeNode *root, int max_depth) {
    // Here's a way to keep track of the depth (in the tree) you're at
    // and print 2 * that many spaces at the beginning of the line.
    static int depth = 0;

    /* proceed only if
       #1 the root is not NULL
       #2 the max_depth is not reached yet */
    if (root && (max_depth == 0 || depth != max_depth)) {
        /* print indentation */
        printf("%*s", depth * 2, "");
        /* print current process */
        printf("%d: %s\n", root->pid, root->name);

        /* increase the depth */
        ++depth;
        /* print child process */
        print_ptree(root->child, max_depth);
        /* decrease the depth */
        --depth;

        /* print the next sibling if exists
           siblings will be indented at the same level */
        print_ptree(root->sibling, max_depth);
    }
}
