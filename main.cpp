// Server side C/C++ program to demonstrate Socket programming



#ifdef linux
#include <sys/socket.h>
    #include <netinet/in.h>
	#include <netdb.h>
#endif

#ifdef _WIN32
#include <WS2tcpip.h>
/*
    #pragma comment(lib,"ws2_32")
    #include <WinSock2.h>

    #include <Windows.h>
    #include <ws2def.h>
    #include <io.h>
    #include <sys/types.h>*/
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <pthread.h>
#define PORT 808028
#define N_THREAD 2

void* server(void*);
void* client(void*);

int main() {

    pthread_t thread[N_THREAD];
    int porta = PORT;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    if (!pthread_create(&thread[0], &attr, server, &porta)) {
        std::cout << "[Main] : Server creato\n";
    }

    if (!pthread_create(&thread[1], &attr, client, &porta)) {
        std::cout << "[Main] : Client creato\n";
    }

     for(int i = 0; i < N_THREAD; i++){
         pthread_join(thread[i], NULL);
         std::cout<<"[Main] : Thread "<<i<<" terminato.\n";
     }

    return 0;

}


void error(const char* msg)
{
    perror(msg);
    #ifdef _WIN32
        std::cout << WSAGetLastError();
    #endif
    exit(1);

}

void* server(void* arg)
{
    int server_socket_id, client_socket_id, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    int iResult;

    #ifdef WIN32
        WSADATA wsaData;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            std::cout << "[Server] : WSAStartup failed: " << iResult << std::endl;

        }
    #endif

    server_socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_id < 0)
        error("[Server] : ERROR opening socket in server");
    else
        std::cout<<"[Server] : Socket aperta con id "<< server_socket_id <<"\n";

    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = PORT;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    int bind_return = bind(server_socket_id, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if ( bind_return < 0 )
        error("[Server] : ERROR on binding in server.");
    else
        std::cout << "[Server] : In ascolto sulla porta " << PORT << ".\n";

    listen(server_socket_id, 5);


    clilen = sizeof(cli_addr);
    client_socket_id = accept(server_socket_id, (struct sockaddr*)&cli_addr, &clilen);
    if (client_socket_id < 0)
        error("[Server] : ERROR on accept in server.\n");

    std::cout<<"[Server] : Nuova connessione in ingresso, in attesa del messaggio.\n";

    bzero(buffer,256);
    n = recv(client_socket_id, buffer, 256, 0);

    if (n < 0)
        error("[Server] : ERROR reading from socket");

    std::cout<<"[Server] : Here is the message: "<<buffer<<"\n";
    bzero(buffer, sizeof(buffer));
    strcpy(buffer, "Ciao sono il server. I got your message");
    n = send(client_socket_id, buffer, sizeof(buffer), 0);
    if (n < 0)
        error("[Server] : ERROR writing to socket");

    #ifdef _WIN32
        closesocket(client_socket_id);
        closesocket(server_socket_id);
        WSACleanup();

    #else
        close(client_socket_id);
        close(server_socket_id);
    #endif

    pthread_exit(0);

}


void* client(void* arg)
{
    int socket_client_id, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char host[] = "25.59.37.39";

    char buffer[256] = "Ciao sono il client";
    portno = PORT;

    #ifdef WIN32
        // Initialize Winsock
        int iResult;
        WSADATA wsaData;
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            std::cout << "[Client] : WSAStartup failed: " << iResult << std::endl;
        }
    #endif

    socket_client_id = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_client_id < 0)
        error("[Client] : ERROR opening socket");
    else
        std::cout<<"[Client] : Socket aperta con id "<< socket_client_id <<"\n";

    server = gethostbyname(host);
    if (server == NULL) {
        std::cout << "[Client] : ERROR, no such host\n";
        exit(0);
    }
    else{
        std::cout <<"[Client] : Tentativo di connessione su "<<host<<":"<<portno<<"\n";
    }
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(socket_client_id, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("[Client] : ERROR connecting");
    else
        std::cout << "[Client] : Connesso al server. Preparazione all'invio del messaggio: '"<<buffer<<"'.\n";

    n = send(socket_client_id, buffer, sizeof(buffer), 0);
    if (n < 0)
        error("[Client] : ERROR writing to socket");
    else
        std::cout << "[Client] : Messaggio inviato correttamente.\n";

    bzero(buffer, sizeof(buffer));

    n = recv(socket_client_id, buffer, 255, 0);
    if (n < 0)
        error("[Client] : ERROR reading from socket");
    else
        std::cout<< "[Client] : Messaggio ricevuto dal server: '"<<buffer<<"'.\n";

    #ifdef _WIN32
    closesocket(socket_client_id);
    WSACleanup();

    #else
        close(socket_client_id);
    #endif


    std::cout << "[Client] : Socket chiusa. Terminazione.\n";
    pthread_exit(0);
}