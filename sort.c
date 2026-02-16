#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int i, j, n, temp;

    if (argc < 2){
        printf(1, "Please write the following format: sort <num1> <num2> ... <numN>\n");
        exit();
    }

    n = argc - 1;
    int arr[128];

    for (i = 0; i < n; i++){
        arr[i] = atoi(argv[i+1]);
    }

    for(i = 0; i< n - 1; i++){
        for(j = 0; j <n - i - 1; j++){
            if(arr[j] > arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    printf(1,"Sort Result:\n");
    for(i = 0; i<n; i++){
        printf(1,"%d\n", arr[i]);
    }

    exit();
}