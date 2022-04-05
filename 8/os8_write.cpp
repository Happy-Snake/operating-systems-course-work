#include <unistd.h>
#include <iostream>
#include <fcntl.h> 
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

using namespace std;

struct attr
{
    int flag;
    mqd_t mqd_id;
    attr (int fl, mqd_t id)
    {
        flag = fl;
        mqd_id = id;
    }
};

void *func(void *ptr)
{
    int count = 0;
    char buf[2048] = "a";
    while ((int)(((attr*)ptr)->flag) == 0)
    {
        //Запись сообщения в очередь
        int result = mq_send((mqd_t)((attr*)ptr)->mqd_id, (const char*)buf, sizeof(buf), 0);
        if (result == -1)
            perror( "Sending error" );
        else 
            cout << "Sent: " << buf << endl << flush;
        sleep(1);
        (int)(*buf)++;
    }
    pthread_exit(ptr);
}

int main()
{
    mqd_t mqd_id;
    struct mq_attr attrs;
    char buffer;

    attrs.mq_flags = 0;
    attrs.mq_maxmsg = 10;
    attrs.mq_msgsize = 2048;
    attrs.mq_curmsgs = 0;


    //Объявление идентефикатора потока
    pthread_t thread_id;
    //Создание (открытие) очереди сообщений 
    mqd_id = mq_open("/myqueue", O_CREAT|O_WRONLY|O_NONBLOCK, 0644, &attrs);
    if (mqd_id == -1)
        perror( "Open to write failed" );
    else 
    {
        //Создание потока из функции потока
        attr a(0, mqd_id);
        pthread_create(&thread_id, NULL, func, &a);
        //Ожидание нажатия клавиши
        cout << "Press enter" << endl;
        getchar();
        cout << "Completing the work process" << endl;
        //Установление флага завершения потока
        a.flag = 1;
        //Ждать завершение потока
        pthread_join(thread_id, NULL);
        //Закрыть очередь сообщений
        cout << "Closing queue" << endl;
        mq_close(mqd_id);
        //Удалить очередь сообщений
        mq_unlink("/myqueue");
    }
}