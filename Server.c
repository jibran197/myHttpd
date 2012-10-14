#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#define SUCCESS 0


struct Config
{
    char logFile[1025];
    int  port;
    char rootDir[1025];
    int  queuingTime;
    int  threadNum;
    char schedPolicy[1025];
} sConfig;


//
//   listenerThread()
//     ListenerThread() accept the connection and add the request to the ready queue.
//
void listenerThread()
{
    int listen_fd, conn_fd;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    pthread_t listener_t, sched_t;

    listen_fd = 0;
    conn_fd   = 0;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(sConfig.port);

    bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listen_fd, 10);

    while(1)
    {
        conn_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);

        //Add the request to the ready queue here.
        // Select the appropriate data structure for the ready queue

        //the following code will just return the local time
        //in server to the client
        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(conn_fd, sendBuff, strlen(sendBuff));

        close(conn_fd);

        sleep(1);
     }

   close(listen_fd);
}


//
//  schedulerThread()
//     select the request from the ready queue and schedule it
//     for execution
//

void schedulerThread()
{
}


int init()
{

    sConfig.port = 8080;

    pthread_t listener_t, sched_t;

    //create the listener thread
    if(pthread_create(&listener_t, NULL, (void*)listenerThread, (void *)0) != 0)
    {
        //log the error message to log file and exit
        exit(0);
    }

    //create the scheduler thread
    if(pthread_create(&sched_t, NULL, (void*)schedulerThread, (void *)0) != 0)
    {
        //log the error message to log file and exit
        exit(0);
    }

    pthread_join(listener_t, NULL);
    pthread_join(sched_t, NULL);

    return SUCCESS;
}


int main(int argc, char *argv[])
{

    sConfig.port = 8080; //default port
    sConfig.threadNum = 4; //default thread count

    //Parse the input arguments here and set the configuration data structure
//  cout<<"\n Usage: %s [-d] [-h] [-l file] [-p port] [-r dir] [-t time] [-n threadnum] [-s sched] \n";

    init();

    return SUCCESS;
}