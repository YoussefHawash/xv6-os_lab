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
   int pid = getpid();

   for (i = 0; i < 12; i++)
   {
      // Read priority fresh each iteration to see decay in action.
      int pr = getpriority(pid);
      printf(1, "Process %d (id=%d) iteration %d Priority: %d\n", pid, id, i, pr);

      for (j = 0; j < 6000000; j++)
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
      // Give the parent a short window to assign priorities first.
      sleep(2);
      busy_work(1);
      exit();
   }

   pid2 = fork();
   if (pid2 == 0)
   {
      sleep(2);
      busy_work(2);
      exit();
   }

   pid3 = fork();
   if (pid3 == 0)
   {
      sleep(2);
      busy_work(3);
      exit();
   }

   // Set priorities before the children begin their CPU work.
   setpriority(pid1, 10);
   setpriority(pid2, 15);
   setpriority(pid3, 20);

   printf(1, "\nSetting priorities...\n");
   printf(1, "pid %d priority 10\n", pid1);
   printf(1, "pid %d priority 15\n", pid2);
   printf(1, "pid %d priority 20\n", pid3);

   printf(1, "Parent sees priorities: %d %d %d\n",
          getpriority(pid1), getpriority(pid2), getpriority(pid3));

   printf(1, "\nProcess table after setting priorities:\n");
   printptable(info);

   // Now wait for children to finish
   wait();
   wait();
   wait();

   printf(1, "\n=== Priority Scheduler Test Finished ===\n");

   exit();
}