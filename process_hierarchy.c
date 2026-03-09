#include "types.h"
#include "user.h"
#include "stat.h"

void print_process_tree(int level) {
    int pid = getpid();
    int ppid = getppid();

    for (int i = 0; i < level; i++) {
        printf(1, "   ");
    }
    printf(1, "PID: %d, PPID %d \n", pid, ppid);

    if (level < 3) {
        int child_id = fork();
        if (child_id == 0) {
            print_process_tree(level + 1);
            exit();
        } else {
            wait();
        }
    }
}

int main(void) {
    printf(1, "Starting Process Hierarchy Visualizer\n");
    print_process_tree(0);
    exit();
}
