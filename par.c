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
      return -1;

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

   int *arr = malloc(count * sizeof(int));
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
      else if (c == ',' || c == '\n' || c == ' ' || c == '\t')
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

   if (in_number)
      arr[idx++] = sign * value;

   close(fd);
   *N = idx;
   return arr;
}

int rec(int *arr, int (*f)(int, int), int N)
{
   if (N == 1)
      return arr[0];
   return f(arr[0], rec(arr + 1, f, N - 1));
}

int parallel_compute(const char *path, int (*f)(int, int), int n_proc)
{
   int *arr = 0;
   int N = 0;

   arr = fill_array(path, &N);
   if (arr == 0 || N == 0)
      return -1;

   if (n_proc > N / 2)
      n_proc = (N == 1) ? 1 : N / 2;

   int pipes[n_proc][2];

   for (int i = 0; i < n_proc; i++)
   {
      if (pipe(pipes[i]) < 0)
      {
         printf(1, "Pipe creation failed\n");
         free(arr);
         return -1;
      }
   }

   for (int i = 0; i < n_proc; i++)
   {
      int base = N / n_proc;
      int extra = N % n_proc;
      int chunk_start = i * base + (i < extra ? i : extra);
      int chunk_size = base + (i < extra ? 1 : 0);

      int pid = fork();

      if (pid < 0)
      {
         free(arr);
         return -1;
      }

      if (pid == 0)
      {
         close(pipes[i][0]);
         int result = rec(arr + chunk_start, f, chunk_size);
         write(pipes[i][1], &result, sizeof(int));
         close(pipes[i][1]);
         free(arr);
         exit();
      }

      close(pipes[i][1]);
   }

   free(arr);

   int partial[n_proc];

   for (int i = 0; i < n_proc; i++)
   {
      read(pipes[i][0], &partial[i], sizeof(int));
      close(pipes[i][0]);
   }

   for (int i = 0; i < n_proc; i++)
      wait();

   return rec(partial, f, n_proc);
}

int main(int argc, char *argv[])
{
   if (argc != 3)
   {
      printf(1, "Usage: parsum <file> <n_proc>\n");
      exit();
   }

   int n_proc = atoi(argv[2]);
   int start = uptime();
   int result = parallel_compute(argv[1], add, n_proc);
   int end = uptime();

   if (result >= 0)
      printf(1, "Result: %d\n", result);
   else
      printf(1, "An error occurred during computation.\n");

   printf(1, "Execution time: %d ticks\n", end - start);
   exit();
}