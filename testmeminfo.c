#include "types.h"
#include "stat.h"
#include "sysinfo.h"
#include "user.h"

int
main(void)
{
  struct sysinfo s;
  int pid;
  int i;
  char *p;

  // baseline
  sysinfo(&s);
  printf(1, "=== baseline ===\n");
  printf(1, "uptime: %d sec\n", s.uptime);
  printf(1, "mem: %d KB total  %d KB free  %d KB used\n",
         s.totalram * 4, s.freeram * 4, s.usedram * 4);
  printf(1, "procs: %d\n", s.nprocs);

  pid = fork();
  if(pid < 0){
    printf(1, "fork failed\n");
    exit();
  }

  if(pid == 0){
    p = sbrk(16 * 4096);
    for(i = 0; i < 16 * 4096; i += 4096)
      p[i] = 1;
    exit();
  }

  // child is running
  sysinfo(&s);
  printf(1, "=== child running ===\n");
  printf(1, "uptime: %d sec\n", s.uptime);
  printf(1, "mem: %d KB total  %d KB free  %d KB used\n",
         s.totalram * 4, s.freeram * 4, s.usedram * 4);
  printf(1, "procs: %d active\n", s.nprocs);

  wait();

  // child done, memory should be back
  sysinfo(&s);
  printf(1, "=== after wait ===\n");
  printf(1, "uptime: %d sec\n", s.uptime);
  printf(1, "mem: %d KB total  %d free  %d used\n",
         s.totalram * 4, s.freeram * 4, s.usedram * 4);
  printf(1, "procs: %d\n", s.nprocs);

  exit();
}
