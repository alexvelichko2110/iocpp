#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <netpacket/packet.h>

#include <string>

namespace net {

class Socket
{
public:

    virtual ~Socket();

    Socket()
    {
        _sock = -1;
    }

    Socket(int sock, const struct sockaddr_in* addr);

    bool connect (const std::string& hostname, int port);
    bool listen (int port);
    Socket accept();

    bool open(int port)
    {
        // socket address used for the server
        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;

        // htons: host to network short: transforms a value in host byte
        // ordering format to a short value in network byte ordering format
        server_address.sin_port = htons(port);

        // htons: host to network long: same as htons but to long
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);

        // create a UDP socket, creation returns -1 on failure
        int sock;
        if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
            printf("could not create socket\n");
            return false;
        }

        // bind it to listen to the incoming connections on the created server
        // address, will return -1 on error
        if ((bind(sock, (struct sockaddr *)&server_address,
                sizeof(server_address))) < 0) {
            printf("could not bind socket\n");
            return false;
        }

        _sock = sock;

        return true;
    }
    
    bool open()
    {
        // open socket
        int sock = socket(PF_INET, SOCK_DGRAM, 0);
        
        if (sock < 0)
        {
            printf("could not create socket\n");
            return false;
        }

        _sock = sock;

        return true;
    }

    int send_to(const std::string& hostname, int port, unsigned char *data, int data_size)
    {
        struct sockaddr_in server_address;

        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
    
        // creates binary representation of server name
        // and stores it as sin_addr
        // http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
        inet_pton(AF_INET, hostname.c_str(), &server_address.sin_addr);
    
        // htons: port in network order format
        server_address.sin_port = htons(port);
    
        // send data
        int len = sendto(_sock, data, data_size, 0, (struct sockaddr*)&server_address, sizeof(server_address));

        return len;
    }

    int recv_from(unsigned char *data, int data_size)
    {
        // socket address used to store client address
        struct sockaddr_in client_address;

        socklen_t client_address_len = sizeof(client_address);
        // int client_address_len = 0;

		// read content into buffer from an incoming client
		int len = recvfrom(_sock, data, data_size, 0,
		                   (struct sockaddr *)&client_address, &client_address_len);


        // // received echoed data back
        // // char buffer[100];
        // int len = recvfrom(_sock, data, data_size, 0, NULL, NULL);

        return len;

        // buffer[len] = '\0';
        // printf("recieved: '%s'\n", buffer);
    }

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

    bool has_data()
    {
        struct timeval tv;

        int timeout = 0;

        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;

        fd_set _rset;
        FD_ZERO (&_rset);
        FD_SET(_sock, &_rset);

        select (_sock, &_rset, NULL, NULL, &tv);

        bool check  = FD_ISSET(_sock, &_rset);

        return check;
    }


private:

    // дескриптор сокета
    int _sock;

    // структура адресса для оконечной точки
    struct sockaddr_in _addr;
};

}

#endif // SOCKET_H
