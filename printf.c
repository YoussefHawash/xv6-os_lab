#include "types.h"
#include "stat.h"
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
}

static void
printint(int fd, int xx, int base, int sgn)
{
  static char digits[] = "0123456789ABCDEF";
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
  }

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);
  if(neg)
    buf[i++] = '-';

  while(--i >= 0)
    putc(fd, buf[i]);
}
static void printfloat(int fd,double xx, int base, int sgn,int accuracy ) // double because the compiler promotes float to double when passed through '...' , No rounfing
{
   static char digits[] = "0123456789ABCDEF";
  char buf[16];
  int i, neg; 
  uint acc_point =1;
  float x;
  uint intpart, fracpart;
  if (accuracy <= 6 && accuracy > 0) //limit to 6 decimal places, so it is reliable 
  {
   while (accuracy-- > 0) {
     acc_point *= 10;
   }
}
   else acc_point = 1000000; 

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
  }
 

  intpart = (uint)x;

  fracpart = (uint)((x - intpart) * acc_point);  

  i = 0;
  do{
      buf[i++] = digits[fracpart % base];
  }while((fracpart /= base) != 0);
  
  buf[i++] = '.';
  
  do{
      buf[i++] = digits[intpart % base];
  }while((intpart /= base) != 0);
  if(neg)
    buf[i++] = '-';
  while(--i >= 0)
    putc(fd, buf[i]);

}
// Print to the given fd. Only understands %d, %x, %p, %s (%f and %.accf ADDED).
void
printf(int fd, const char *fmt, ...)
{
  char *s;
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
      if(c == 'd'){
        printint(fd, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
        printint(fd, *ap, 16, 0);
        ap++;
      } else if(c == 's'){
        s = (char*)*ap;
        ap++;
        if(s == 0)
          s = "(null)";
        while(*s != 0){
          putc(fd, *s);
          s++;
        }
        
      }else if(c == 'f'){
        printfloat(fd, *(double*)ap, 10, 1, 3); // Default accuracy of 3 decimal places
        ap+=2; // Move past the double argument (which takes 2 uints)
      }  else if (c== '.')
      {
        int accuracy = 0;
        i++;
        c = fmt[i++] & 0xff;
        while (c >= '0' && c <= '9') {
          accuracy = accuracy * 10 + (c - '0');
          c = fmt[i] & 0xff;
        }
        if (c == 'f') {
          printfloat(fd, *(double*)ap, 10, 1, accuracy);
          ap+=2; // Move past the double argument (which takes 2 uints)
        } 
        else 
        {
           // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
        }
      }
      else if(c == 'c'){
        putc(fd, *ap);
        ap++;
      } else if(c == '%'){
        putc(fd, c);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
      }
      state = 0;
    }
  }
}
