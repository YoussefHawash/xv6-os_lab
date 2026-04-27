#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
   if (argc != 3)
   {
      printf(2, "Usage: setpriority pid priority\n");
      exit();
   }
   int pid, pr;
   pid = atoi(argv[1]);
   pr = atoi(argv[2]);
   printf(1, "Old priority: %d\n", setpriority(pid, pr));
   printf(1, "New priority of %d: %d\n", pid, pr);
   exit();
}