#include <iostream>
#include <semaphore.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

struct attr
{
    int *flag;
    int *fd;
    pthread_t *thread;
    attr (int *fl, int *descr, pthread_t *tr)
    {
        flag = fl;
        fd = descr;
        thread = tr;
    }
};


bool sig_handler(int signo)
{
    cout << "SIGPIPE" << endl << flush;
    sleep(2);
    return true;
}

void *func_transfer(void *ptr)
{
    int buf = 0;
    while (*(((attr*)ptr)->flag) == 0)
    {
        buf++;
        if(signal(SIGPIPE, (__sighandler_t)sig_handler))
        {
            if (write(*((attr*)ptr)->fd, &buf, sizeof(buf)) != -1)
                cout << "Written: " << buf << endl << flush;
        }
        sleep(1);
    }
    pthread_exit(ptr);
}

void *func_open(void *ptr)
{
    bool opened = false;
    while (*(((attr*)ptr)->flag) == 0)
    {
        if (!opened)
        {
            //Открытие именованного канала
            *((attr*)ptr)->fd = open("/tmp/my_named_pipe", O_WRONLY|O_NONBLOCK|O_CREAT, 0666);
            if (*((attr*)ptr)->fd == -1)
                sleep(1);
            else
            {
                opened = true;
                attr b(((attr*)ptr)->flag, ((attr*)ptr)->fd, ((attr*)ptr)->thread);
                pthread_create(((attr*)ptr)->thread, NULL, func_transfer, &b);
            }
        }
    }
    pthread_exit(ptr);
}

int main()
{
    //Флаги завершения потоков
    int fl = 0; 
    //Объявление идентификаторов потоков
    pthread_t thread_id_open;
    pthread_t thread_id_transfer;
    //Создание именованного канала
    mkfifo("/tmp/my_named_pipe", 0666);
    //Открытие именованного канала
    int descr = open("/tmp/my_named_pipe", O_WRONLY|O_NONBLOCK|O_CREAT, 0666);   
    //Создание потока
    //Создание потока
    attr a(&fl, &descr, &thread_id_transfer);
    pthread_create(&thread_id_open, NULL, func_open, &a);
    //Ожидание нажатия Enter и смена флагов
    cout << "Press enter" << endl;
    getchar();
    cout << "Completing the work process" << endl;
    fl = 1;
    pthread_join(thread_id_open, NULL);
    pthread_join(thread_id_transfer, NULL);
    //Закрытие и удаление именованного семафора
    close(descr);
    unlink("/tmp/my_named_pipe");
}