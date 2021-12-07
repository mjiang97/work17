#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define KEY 24601
#define SEM_KEY 24601

int main() {
    int semd = semget(SEM_KEY, 1, 0);
    int n = semctl(semd, 0, GETVAL);
    if (n > 0) {
        struct sembuf sb;

        sb.sem_num = 0;
        sb.sem_flg = SEM_UNDO;
        sb.sem_op = -1;

        semop(semd, &sb, 1);
        int shmd = shmget(KEY, sizeof(int), 0640);
        int *data;
        data = shmat(shmd, 0, 0);
        int fd = open("message", O_RDONLY, 0);
        lseek(fd, *data, SEEK_SET);
        printf("Previous line added: ");
        char c;
        int size = 0;
        int bytes = read(fd, &c, sizeof(char));
        while (bytes > 0) {
            printf("%c", c);
            size++;
            bytes = read(fd, &c, sizeof(char));
        }
        *data = size + *data;
        char next_line[100];
        printf("\nNew Line:\t");
        fgets(next_line, sizeof(next_line), stdin);
        int new_size = strcspn(next_line, "\n");
        next_line[new_size] = 0;
        
        close(fd);
        fd = open("message", O_WRONLY | O_APPEND, 0);
        write(fd, next_line, new_size);
        close(fd);

        sb.sem_op = 1;
        semop(semd, &sb, 1);
    }
}