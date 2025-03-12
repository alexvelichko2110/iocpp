#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

namespace net {

class Socket
{
public:

    virtual ~Socket();

    Socket();
    Socket(int sock, const struct sockaddr_in* addr);

    int get_socket() {
        return _sock;
    }

    int send_data(char *data, int data_size) {

        int result = send(_sock, data, data_size, 0);

        return result;
    }

    int recv_data(char *data, int data_size) {

        int result = recv(_sock, data, data_size, 0);

        return result;
    }

private:

    // дескриптор сокета
    int _sock;

    // структура адресса для оконечной точки
    struct sockaddr_in _addr;
};


Socket* open (const std::string& hostname, int port);
Socket* open (int port);
Socket* accept(Socket* sock);

}

#endif // SOCKET_H
