#include "types.h"
#include "stat.h"
#include "sysinfo.h"
#include "user.h"

static void
snapshot(const char *label)
{
  struct sysinfo s;
  if(sysinfo(&s) < 0){
    printf(1, "sysinfo failed\n");
    exit();
  }
  printf(1, "--- %s ---\n", label);
  printf(1, "  uptime : %d sec\n",    s.uptime);
  printf(1, "  memory : %d KB total  %d KB free  %d KB used\n",
         s.totalram * 4, s.freeram * 4, s.usedram * 4);
  printf(1, "  procs  : %d active\n", s.nprocs);
}

int
main(void)
{
  int pid;

  snapshot("system baseline");

  pid = fork();
  if(pid < 0){
    printf(1, "fork failed\n");
    exit();
  }

  if(pid == 0){
    // Child: allocate 16 pages, touch them, then exit.
    char *p = sbrk(16 * 4096);
    int i;
    for(i = 0; i < 16 * 4096; i += 4096)
      p[i] = 1;
    exit();
  }

  // Parent: child is alive — nprocs should be +1.
  snapshot("child running (+1 proc, +16 pages)");

  wait();

  // Child has exited and been reaped — back to baseline.
  snapshot("after child exited (memory reclaimed)");

  exit();
}
