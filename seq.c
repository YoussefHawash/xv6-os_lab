#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int add(int a, int b)
{
   return a + b;
}

int count_numbers(const char *path)
{
   int fd = open(path, 0);
   if (fd < 0)
   {
      printf(1, "Failed to open file %s\n", path);
      return -1;
   }

   char c;
   int in_number = 0;
   int count = 0;

   while (read(fd, &c, 1) == 1)
   {
      if ((c >= '0' && c <= '9') || c == '-')
      {
         if (!in_number)
         {
            count++;
            in_number = 1;
         }
      }
      else
      {
         in_number = 0;
      }
   }

   close(fd);
   return count;
}

// Fill array
int *fill_array(const char *path, int *N)
{
   int fd = open(path, 0);
   if (fd < 0)
   {
      printf(1, "Failed to open file %s\n", path);
      return 0;
   }

   int count = count_numbers(path);
   if (count <= 0)
   {
      close(fd);
      return 0;
   }

   int *arr = (int *)malloc(count * sizeof(int));
   if (arr == 0)
   {
      printf(1, "Memory allocation failed\n");
      close(fd);
      return 0;
   }

   char c;
   int value = 0;
   int sign = 1;
   int in_number = 0;
   int idx = 0;

   while (read(fd, &c, 1) == 1)
   {
      if (c == '-')
      {
         sign = -1;
         value = 0;
         in_number = 1;
      }
      else if (c >= '0' && c <= '9')
      {
         value = value * 10 + (c - '0');
         in_number = 1;
      }
      else if (c == ',')
      {
         if (in_number)
         {
            arr[idx++] = sign * value;
            value = 0;
            sign = 1;
            in_number = 0;
         }
      }
   }

   // handle last number
   if (in_number)
   {
      arr[idx++] = sign * value;
   }

   close(fd);
   *N = idx;
   return arr;
}

int rec(int *arr, int (*f)(int, int), int size)
{
   if (size == 1)
      return arr[0];
   return f(arr[0], rec(arr + 1, f, size - 1));
}

int sequential_compute(const char *path, int (*f)(int, int))
{
   int *arr = 0;
   int size = 0;

   arr = fill_array(path, &size);
   if (arr == 0 || size == 0)
      return -1;

   int result = rec(arr, f, size);
   free(arr);
   return result;
}

int main(int argc, char *argv[])
{
   if (argc != 2)
   {
      printf(1, "Usage: seqsum <file>\n");
      exit();
   }

   int start = uptime();
   int result = sequential_compute(argv[1], add);
   int end = uptime();

   if (result >= 0)
      printf(1, "Result: %d\n", result);
   else
      printf(1, "An error occurred during computation.\n");

   printf(1, "Execution time: %d ticks\n", end - start);

   exit();
}