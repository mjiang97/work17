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

int main(int argc, char **argv) {
    if (argc > 1) {
        int shmd = shmget(KEY, sizeof(int), IPC_CREAT | 0640);
        int semd = semget(SEM_KEY, 1, IPC_CREAT | 0644);
        
        if (strcmp(argv[1], "-create") == 0) {
            union semun us;
            us.val = 1;
            semctl(semd, 0, SETVAL, us);
            char * message = argv[2];
            int f = open("message", O_CREAT | O_WRONLY | O_TRUNC, 0644);
            char *c = argv[2];
            while (*c) {
                write(f, c, sizeof(char));
                c++;
            }
            close(f);
            int *data;
            data = shmat(shmd, 0, 0);
            *data = 0;
            shmdt(data);
        } else if (strcmp(argv[1], "-remove") == 0) {
            shmctl(shmd, IPC_RMID, 0);
            semctl(semd, IPC_RMID, 0);
            int f = open("message", O_RDONLY, 0);
            char * c;
            int bytes = read(f, c, sizeof(char));
            
            while (bytes > 0) {
                printf("%c", *c);
                bytes = read(f, c, sizeof(char));
            }
            printf("\n");
        }
    }
}
