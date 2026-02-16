#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
   if (argc < 2) {
      printf(2, "Usage: printstats <numbers>\n");
      exit();
   }
   float avg = 0.0;
   uint population = argc -1;
   int median,min,max;
   int num_list[population];
   //populate
   for (int i = 1; i < argc; i++) {
      avg += atosi(argv[i]);
      num_list[i-1] = atosi(argv[i]);
   }
   avg= avg/population;
//sort the numbers to find median, min and max
   for (int i = 0; i < population-1; i++) {
      for (int j = i+1; j < population; j++) {
         if (num_list[i] > num_list[j]) {
            int temp = num_list[i];
            num_list[i] = num_list[j];
            num_list[j] = temp;
         }
      }
   }

   min = num_list[0];
   max = num_list[population-1];
   median = num_list[(population-1)/2];
  
   //standard deviation
   float sum_squared_diff = 0.0;
   for (int i = 0; i < population; i++) {
      float diff = num_list[i] - avg;
      sum_squared_diff += diff * diff;
   }
   float std_dev = sum_squared_diff / population;
   std_dev = sqrt(std_dev);   
      printf(1, "Average: %f\n", avg);
      printf(1, "Min: %d\n", min);
      printf(1, "Max: %d\n", max);
      printf(1, "Median: %d\n", median);
      printf(1, "Standard Deviation: %f\n", std_dev);

  exit();
}