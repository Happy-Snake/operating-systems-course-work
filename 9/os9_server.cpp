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
	while ((int)(((attr*)ptr)->flag) == 0) 
    {
        pthread_mutex_lock(((attr*)ptr)->m_id);
		if (!(*((attr*)ptr)->msglist).empty()) 
        {
            string S  = (*((attr*)ptr)->msglist).back();
            (*((attr*)ptr)->msglist).pop_back();
            pthread_mutex_unlock(((attr*)ptr)->m_id);
            struct passwd* strct = getpwuid(getuid());
            S += ' ';
            S += strct->pw_name;
            const char* sndbuf = S.c_str();;
            int sentcount = sendto(*((attr*)ptr)->socket,sndbuf,sizeof(char[256]),0,(struct sockaddr*)((attr*)ptr)->saddr,*(((attr*)ptr)->saddrlen));
            if (sentcount == -1) 
            {
                //perror("Sending error");
            }
            else 
                cout << "Send: " << S << endl << flush;
        }
        else 
            pthread_mutex_unlock(((attr*)ptr)->m_id);
        sleep(3);
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
        {
            cout << "Recieved: " << rcvbuf << " " << endl << flush;
            pthread_mutex_lock(((attr*)ptr)->m_id);
            (*((attr*)ptr)->msglist).push_back(string(rcvbuf));
            pthread_mutex_unlock(((attr*)ptr)->m_id);
        }
        sleep(3);
	}
    pthread_exit(ptr);
}


int main()
{
    cout << "Server program starts" << endl << flush;
    //Создание сокета
	int mysock = mysock = socket(AF_INET,SOCK_DGRAM,0);
	if (mysock == -1 ) 
    {
        cout << "Socket creating error" << endl << flush;
        return -1;
    }

    //Установление неблокирующего режима
    fcntl(mysock,F_SETFL,O_NONBLOCK);

    int optval = 1;
    setsockopt(mysock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    //Привязывание адреса сокету
	struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_port = htons(8000);
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);

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
    saddr.sin_port = htons(7000);
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