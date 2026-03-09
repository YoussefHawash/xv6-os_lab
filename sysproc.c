#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "pinfo.h"
#include "sysinfo.h"
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int sys_getpinfo(void) {
  struct pinfo* info;
  int pid;
  if(argptr(0, (void*)&info, sizeof(*info)) < 0)
    return -1;
  if(argint(1, &pid) < 0)
    return -1;
  return getprocinfo(info, (uint)pid);
}

int sys_listproc(void) {
  struct pinfo* infos;
  if(argptr(0, (void*)&infos, sizeof(struct pinfo) * NPROC) < 0)
    return -1;
  return listproc(infos);
}

int
sys_sysinfo(void)
{
  struct sysinfo *info;
  int free_pages, used_pages;

  if(argptr(0, (char**)&info, sizeof(struct sysinfo)) < 0)
    return -1;

  acquire(&tickslock);
  info->uptime = ticks / 100;  // 100 ticks/sec
  release(&tickslock);

  getmeminfo_counts(&free_pages, &used_pages);
  info->freeram  = (uint)free_pages;
  info->usedram  = (uint)used_pages;
  info->totalram = (uint)(free_pages + used_pages);

  info->nprocs = (uint)getactiveprocs();

  return 0;
}
int sys_getppid(void) {
  struct proc *p = myproc();
  if (p->parent == 0) return -1;
  return p->parent->pid;
}
