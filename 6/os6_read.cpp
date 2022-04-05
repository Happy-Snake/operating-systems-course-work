#include <unistd.h>
#include <iostream>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <sys/mman.h>

using namespace std;

struct attr
{
    int flag;
    sem_t * sem_id_write;
    sem_t * sem_id_read;
    int *addr;
    attr (int fl, sem_t * id_write, sem_t * id_read, int *a)
    {
        flag = fl;
        sem_id_write = id_write;
        sem_id_read = id_read;
        addr = a;
    }
};

void *func(void *ptr)
{
    int value = 0;
    while ((int)(((attr*)ptr)->flag) == 0)
    {
        sem_wait(((attr*)ptr)->sem_id_write);
        memcpy(&value, ((attr*)ptr)->addr, sizeof(value));
        sem_post(((attr*)ptr)->sem_id_read);
        cout << value << ", " << flush;
        sleep(1);
    }
    pthread_exit(ptr);
}

int main()
{
    //Объявление идентификатор потока
    pthread_t thread_id;
    //Создание разделяемой памяти
    int shm = shm_open("shared_memory", O_CREAT|O_RDWR, 0644);
    //Установление размера разделяемой памяти до требуемого размера
    ftruncate(shm, sizeof(int));
    //Отображение разделяемой памяти на локальный адрес
    int *addr = (int*)mmap(0, sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    //Создание семафора записи
    sem_t * sem_id_write = sem_open("/sem_write", O_CREAT, 0644, 0);
    //Создание семафора чтения
    sem_t * sem_id_read = sem_open("/sem_read", O_CREAT, 0644, 0);
    //Создание потока из функции потока
    attr a(0, sem_id_write, sem_id_read, addr);
    pthread_create(&thread_id, NULL, func, &a);
    //Ожидание нажатия Enter и смена флагов
    cout << "Press enter" << endl;
    getchar();
    cout << "Completing the work process" << endl;
    a.flag = 1;
    pthread_join(thread_id, NULL);
    //Закрытие именованного семафора
    sem_close(sem_id_write);
    sem_close(sem_id_read);
    //Удаление именнованного семафора
    sem_unlink("/sem_write");
    sem_unlink("/sem_read");
    //Закрытие отображения разделяемой памяти на локальный адрес
     munmap(0,sizeof(int));
    //Удаление разделяемой памяти
    close(shm);
    shm_unlink("shared_memory");
}