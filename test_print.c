#include <stdio.h>
#include <stdlib.h>

#include "ptree.h"


int main(int argc, char *argv[]) {
    // Creates a ptree to test printing
    // Notice that in this tree the names are string literals. This is fine for
    // testing but is not what the assignment asks you to do in generate_ptree.
    // Read the handout carefully. 
    struct TreeNode root;
    struct TreeNode child_one, child_two;
    struct TreeNode grandchild_one_of_child_one, grandchild_two_of_child_one;
    struct TreeNode grandchild_one_of_child_two, grandchild_two_of_child_two;
    root.pid = 4511;
    root.name = "root process";
    root.child = &child_one;
    root.sibling = NULL;

    child_one.pid = 4523;
    child_one.name = "first child";
    child_one.child = &grandchild_one_of_child_one;
    child_one.sibling = &child_two;

    grandchild_one_of_child_one.pid = 4601;
    grandchild_one_of_child_one.name = "first child - first grandchild";
    grandchild_one_of_child_one.child = NULL;
    grandchild_one_of_child_one.sibling = &grandchild_two_of_child_one;

    grandchild_two_of_child_one.pid = 4602;
    grandchild_two_of_child_one.name = "first child - second grandchild";
    grandchild_two_of_child_one.child = NULL;
    grandchild_two_of_child_one.sibling = NULL;

    child_two.pid = 4524; 
    child_two.name = "second child";
    child_two.child = &grandchild_one_of_child_two;
    child_two.sibling = NULL;

    grandchild_one_of_child_two.pid = 4608;
    grandchild_one_of_child_two.name = "second child - first grandchild";
    grandchild_one_of_child_two.child = NULL;
    grandchild_one_of_child_two.sibling = &grandchild_two_of_child_two;

    grandchild_two_of_child_two.pid = 4609;
    grandchild_two_of_child_two.name = "second child - second grandchild";
    grandchild_two_of_child_two.child = NULL;
    grandchild_two_of_child_two.sibling = NULL;

    print_ptree(&root, 0);
    print_ptree(&root, 1);
    print_ptree(&root, 2);
    print_ptree(&root, 3);
    print_ptree(&root, 4);

    return 0;
}
