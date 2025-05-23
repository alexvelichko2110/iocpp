
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"

namespace net  {

void build (struct sockaddr_in& addr, const std::string& hostname, int port)
{
    memset((void *)&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(hostname.c_str());
}

void build (struct sockaddr_in& addr, int port)
{
    memset((void *)&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    addr.sin_addr.s_addr = htonl (INADDR_ANY);
}


Socket::~Socket()
{
    if (_sock)
        close(_sock);
}

Socket::Socket(int sock, const struct sockaddr_in* addr)
    : _sock(sock), _addr(*addr)
{

}


bool Socket::connect (const std::string& hostname, int port)
{
    // create socket ...
    int sock = socket( AF_INET, SOCK_STREAM, 0);

    if ( sock == -1 )
    {
        printf("error: create socket fd\n");

        return false;
    }

    struct sockaddr_in addr;

    build(addr, hostname, port);

    // connect ...
    int result = ::connect(sock, (struct sockaddr *)&addr, sizeof(addr) );

    if (result != 0)
    {
        printf("error: connect fail\n");

        close(sock);

        return false;
    }

    _sock = sock;
    _addr = addr;

    return true;
}

bool Socket::listen (int port)
{
    int max_queue = 5;

    int sock = socket( AF_INET, SOCK_STREAM, 0);

    if ( sock == -1 )
    {
        printf("error: create listen socket \n");

        return false;
    }

    const int on = 1;

    // Set REUSEADDR so we can reuse the port ...
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*) &on, sizeof(on) ) != 0)
    {
        printf("error: can't set reuse on socket \n");

        return false;
    }


    struct sockaddr_in addr;

    build(addr, port);

    // устанавливаем адрес для сокета ...
    if ( bind(sock, (struct sockaddr *)&addr, sizeof(addr) ) == -1)
    {
        printf("error: bind socket \n");

        close(sock);
    }

    // ставим сокет на прослушку входящих запросов клиентов ...
    if ( ::listen(sock, max_queue) == -1)
    {
        printf("error: listen \n");

        close(sock);
    }

    _sock = sock;
    _addr = addr;

    return true;
}

Socket Socket::accept()
{
    struct sockaddr_in addr;

    socklen_t n = sizeof(addr);


    int new_client_sock = ::accept(_sock, (struct sockaddr *)&addr, &n);

    if ( new_client_sock == -1 )
    {
        printf("error: accept socket \n");
    }


    // printf("conn: <port = %d> <host = %s> \n", port, host);

    return Socket(new_client_sock, &addr);
}

}
