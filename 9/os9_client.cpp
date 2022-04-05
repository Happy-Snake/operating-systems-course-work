#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <vector>
#include <pwd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

using namespace std;

struct attr
{
    int flag;
    int *socket;
    struct sockaddr_in *saddr;
    socklen_t *saddrlen;
    pthread_mutex_t *m_id;
    vector <string> *msglist;
    attr (int fl, int *sock, sockaddr_in *addr, socklen_t *len, pthread_mutex_t* m, vector <string> *list)
    {
        flag = fl;
        socket = sock;
        saddr = addr;
        saddrlen = len;
        m_id = m;
        msglist = list;
    }
};

void *send(void* ptr)
{
    int count = 0;
    char sndbuf[256];
	while ((int)(((attr*)ptr)->flag) == 0) 
    {
        int len = sprintf(sndbuf,"Request, %d",count);
        int sentcount = sendto(*((attr*)ptr)->socket,sndbuf,sizeof(sndbuf),0,(struct sockaddr*)((attr*)ptr)->saddr,*(((attr*)ptr)->saddrlen));
        if (sentcount == -1) 
        {
            //perror("Sending error");
        }
        else 
            cout << "Send: " << sndbuf << endl << flush;
        sleep(3);
        count++;
    }
    pthread_exit(ptr);
}

void *recieve(void* ptr)
{
	char rcvbuf[256];
	while ((int)(((attr*)ptr)->flag) == 0) 
    {
        memset(rcvbuf,0,sizeof(rcvbuf));
        int recvcount = recvfrom(*((attr*)ptr)->socket,rcvbuf,sizeof(rcvbuf),0,(struct sockaddr*)((attr*)ptr)->saddr,(((attr*)ptr)->saddrlen));
        if (recvcount == -1) 
        {
            //perror("Recieving error");
        }
        else 
            cout << "Recieved: " << rcvbuf << endl << flush;
        sleep(3);
	}
    pthread_exit(ptr);
}

int main()
{
    cout << "Client program starts" << endl << flush;
    //Создание сокета
	int mysock = mysock = socket(AF_INET,SOCK_DGRAM,0);
	if (mysock == -1 ) 
    {
        cout << "Socket creating error" << endl << flush;
        return -1;
    }

    //Установление неблокирующего режима
    fcntl(mysock,F_SETFL,O_NONBLOCK);

    //Привязывание адреса сокету
	struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_port = htons(7000);
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   
    int optval = 1;
    if (setsockopt(mysock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
  	      perror("Setsockopt error");
  	      close(mysock);
    }

	if (bind(mysock,(struct sockaddr*)&bindaddr,sizeof(bindaddr)) == -1)
    {
            cout << "Blind error" << endl << flush;
            return -1;
    }

    //Установка адреса клиента
    struct sockaddr_in saddr;
    socklen_t saddrlen;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddrlen = sizeof(saddr);

    //Создание потоков
    vector <string> msglist;
    pthread_mutex_t m_id;
    pthread_t thread_id_recieve;
    pthread_t thread_id_send;
    attr a1(0, &mysock, &saddr, &saddrlen, &m_id, &msglist);
    attr a2(0, &mysock, &saddr, &saddrlen, &m_id, &msglist);
	pthread_create(&thread_id_recieve, NULL, recieve, &a1);
	pthread_create(&thread_id_send, NULL, send, &a2);

    //Ожидание нажатия клавиши
    cout << "Press enter" << endl;
    getchar();
    cout << "Completing the work process" << endl;
    //Установление флага завершения потока
    a1.flag = 1;
    a2.flag = 1;
    //Ждать завершение потока
    pthread_join(thread_id_recieve, NULL);
    pthread_join(thread_id_send, NULL);
    close(mysock);			
}