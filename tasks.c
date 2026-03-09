#include "types.h"
#include "param.h"
#include "pinfo.h"
#include "user.h"

char *statenames[] = { "unused", "embryo", "sleep", "runble", "run", "zombie" };

void
printpad(char *s, int width)
{
  int len = strlen(s);
  printf(1, "%s", s);
  while(len++ < width)
    printf(1, " ");
}

void
printipad(int val, int width)
{
  char buf[16];
  int len, i;

  if(val == 0){
    buf[0] = '0';
    buf[1] = 0;
  } else {
    int neg = 0;
    int v = val;
    if(v < 0){ neg = 1; v = -v; }
    i = 0;
    while(v > 0){
      buf[i++] = '0' + (v % 10);
      v /= 10;
    }
    if(neg) buf[i++] = '-';
    buf[i] = 0;
    // reverse  
    for(int a = 0, b = i-1; a < b; a++, b--){
      char t = buf[a]; buf[a] = buf[b]; buf[b] = t;
    }
  }
  printpad(buf, width);
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    struct pinfo info;
    int pid = atoi(argv[1]);
    if(getpinfo(&info, (uint)pid) < 0) {
      printf(1, "failed for pid %d\n", pid);
    } else {  
      printf(1, "PID: %d\n", info.pid);
      printf(1, "PPID: %d\n", info.ppid);
      printf(1, "SIZE: %d\n", info.sz);
      printf(1, "STATE: %s\n", statenames[info.state]);
      printf(1, "NAME: %s\n", info.name);
    }
    exit();
  }
  struct pinfo infos[NPROC];
  int n, i;

  n = listproc(infos);
  if(n < 0) {
    printf(1, "listproc failed\n");
    exit();
  }
  printpad("PID", 8);
  printpad("PPID", 8);
  printpad("SIZE", 10);
  printpad("STATE", 10);
  printpad("NAME", 16);
  printf(1, "\n");
  // separator
  for(i = 0; i < 52; i++)
    printf(1, "-");
  printf(1, "\n");
  for(i = 0; i < n; i++) {
    
    printipad(infos[i].pid, 8);
    printipad(infos[i].ppid, 8);
    printipad(infos[i].sz, 10);
    printpad(statenames[infos[i].state], 10);
    printpad(infos[i].name, 16);
    printf(1, "\n");
  }
  // separator
  for(i = 0; i < 52; i++)
    printf(1, "-");
  printf(1, "\n");
  printf(1, "Total processes: %d\n", n);
  exit();
}
// Note : AI was used to help in the padding part only!