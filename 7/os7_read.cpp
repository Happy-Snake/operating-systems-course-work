#include <iostream>
#include <semaphore.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

struct attr
{
    int flag;
    int fd;
    attr (int fl, int descr)
    {
        flag = fl;
        fd = descr;
    }
};

void *func(void *ptr)
{
    int buf = 0;
    while ((int)(((attr*)ptr)->flag) == 0)
    {
        if (read(((attr*)ptr)->fd, &buf, sizeof(buf)) != -1)
            cout << "Read: " << buf << " " << endl << flush;
        sleep(1);
    }
    pthread_exit(ptr);
}

int main()
{
    //Объявление идентификаторов потоков
    pthread_t thread_id_read;
    //cout << "Read program started" << endl << flush;
    //Создание именованного канала
    mkfifo("/tmp/my_named_pipe", 0666);
    //Открытие именованного канала
    int descr = open("/tmp/my_named_pipe", O_RDONLY|O_NONBLOCK|O_CREAT, 0666);   
    //Создание потока
    attr a(0, descr);
    pthread_create(&thread_id_read, NULL, func, &a);
    //Ожидание нажатия Enter и смена флагов
    cout << "Press enter" << endl;
    getchar();
    cout << "Completing the work process" << endl;
    a.flag = 1;
    pthread_join(thread_id_read, NULL);
    //Закрытие именованного семафора
    close(descr);
    unlink("/tmp/my_named_pipe");
}