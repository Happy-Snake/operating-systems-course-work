#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <semaphore.h>
#include <fcntl.h> 
#include <sys/stat.h>

using namespace std;

struct attr
{
    int flag;
    sem_t * sem_id;
    int fd;
    attr (int fl, sem_t * id, int f)
    {
        flag = fl;
        sem_id = id;
        fd = f;
    }
};

void *func(void *ptr)
{
    while ((int)(((attr*)ptr)->flag) == 0)
    {
        char a ='2';
        sem_wait(((attr*)ptr)->sem_id);
        for (int i = 0; i < 5; i++)
        {
            cout << a << flush;
            write(((attr*)ptr)->fd, &a, sizeof(a));
            sleep(1);
        }
        a = '\n';
        write(((attr*)ptr)->fd, &a, sizeof(a));
        sem_post(((attr*)ptr)->sem_id);
        sleep(1);
    }
    pthread_exit(ptr);
}

int main()
{
    //Создание именованного семафора
    sem_t * sem_id = sem_open("/sem_name", O_CREAT, 0644, 1);
    //Создание (или открытие) файла
    int fd = open("text.txt",O_CREAT|O_RDWR|O_TRUNC|O_APPEND,0644);
    if (fd == -1)
    {
        cout << "Ошибка открытия файла";
        return -1;
    }
    pthread_t id1; 
    attr a (0, sem_id, fd);
    //Создание потока
    pthread_create(&id1, NULL, func, &a);
    //Ожидание нажатия Enter и смена флагов
    cout << "Press enter" << endl;
    getchar();
    cout << "Completing the work process" << endl;
    a.flag = 1;
    pthread_join(id1, NULL);
    //Закрытие файла
    close(fd);
    //Закрытие именованного семафора
    sem_close(sem_id);
    //Удаление именнованного семафора
    sem_unlink("/sem_name");
}