// sysinfo.h — shared between kernel and user space.
// Include types.h before this file.

struct sysinfo {
  uint uptime;    // seconds since boot (ticks / 100, xv6 runs at 100 Hz)
  uint totalram;  // total usable physical memory pages
  uint freeram;   // free physical memory pages
  uint usedram;   // used physical memory pages (totalram - freeram)
  uint nprocs;    // number of active (non-UNUSED) processes
};
