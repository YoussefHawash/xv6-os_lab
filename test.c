#include "types.h"
#include "stat.h"
#include "user.h"

struct pinfo
{
   char name[16];
   int pid;
   int ppid;
   int sz;
   int state;
   int pr;
};
void busy_work(int id)
{
   int i, j;
   int pr = getpriority(getpid());
   int pid = getpid();
   for (i = 0; i < 20; i++)
   {
      printf(1, "Process %d running iteration %d Priority: %d\n", pid, i, pr);

      for (j = 0; j < 10000000; j++)
      {
         // busy wait
      }
   }
}

int main(int argc, char *argv[])
{
   int pid1, pid2, pid3;
   struct pinfo info[64];

   printf(1, "\n=== Priority Scheduler Test ===\n");

   pid1 = fork();
   if (pid1 == 0)
   {
      sleep(100);
      busy_work(1);
      exit();
   }

   pid2 = fork();
   if (pid2 == 0)
   {
      sleep(100);
      busy_work(2);
      exit();
   }

   pid3 = fork();
   if (pid3 == 0)
   {
      sleep(100);
      busy_work(3);
      exit();
   }

   sleep(10);

   printf(1, "\nSetting priorities...\n");

   setpriority(pid1, 1);
   setpriority(pid2, 5);
   setpriority(pid3, 10);

   printf(1, "\nProcess priorities:\n");
   printf(1, "pid %d priority 1\n", pid1);
   printf(1, "pid %d priority 5\n", pid2);
   printf(1, "pid %d priority 10\n", pid3);

   printf(1, "\nProcess table after setting priorities:\n");
   printptable(info);

   wait();
   wait();
   wait();

   printf(1, "\n=== Priority Scheduler Test Finished ===\n");

   exit();
}