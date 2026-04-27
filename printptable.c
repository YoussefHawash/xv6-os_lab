#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

struct pinfo
{
   char name[16];
   int pid;
   int ppid;
   int sz;
   int state;
   int pr;
};
int main(int argc, char *argv[])
{
   struct pinfo info[NPROC];
   int count = printptable(info);
   printf(1, "Active processes: %d\n", count);
   for (int i = 0; i < count; i++)
   {
      printf(1, "Name: %s, PID: %d,   State: %d, Priority: %d\n", info[i].name, info[i].pid, info[i].state, info[i].pr);
   }
   exit();
}