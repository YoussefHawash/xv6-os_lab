#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"



int main(int argc, char *argv[])
{
if(argc < 3){
        exit();
}
char *filename = argv[1];
char *text = argv[2];

int n = 0;
while (text[n] != '\0'){
n++;
}

int fd = open(filename, O_CREATE | O_WRONLY);
if (fd < 0){
exit();

}for(int i = 2; i < argc; i++){
        int j = 0;
while (argv[i][j] != '\0'){
        j++;
}

write (fd, argv[i], j);

if(i < argc - 1){
write(fd, " ", 1);
}
}

write(fd, "\n", 1);

exit();
}