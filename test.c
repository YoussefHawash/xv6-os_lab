#include <stdio.h>


typedef unsigned int uint;
void func1(float xx, int base, int sgn,int accuracy)
{
   static char digits[] = "0123456789ABCDEF";
  char buf[16];
  int i, neg;
  uint acc_point =1;
  float x;
  uint intpart, fracpart;
  if (accuracy <= 6 && accuracy > 0) //limit to 6 decimal places
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

  while(--i >= 0)
    printf( "%c", buf[i]);
   printf( "\n");

}


int main(int argc, char *argv[]) {
   printf( "Hello, World!\n");
   func1(3.141111132323, 10, 1, 10);


   return 0;
}