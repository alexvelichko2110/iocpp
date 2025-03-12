
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>

#include <string.h>
#include <iostream>

#include "buffer.h"

namespace net {

Buffer::Buffer()
{
    _buffer_size = 256;

    _buffer_ptr = (char* )malloc(_buffer_size);

    _data = _buffer_ptr;
    _data_size = 0;
}

Buffer::~Buffer()
{
    free(_buffer_ptr);
}

bool Buffer::load_from_socket(int socket)
{
    int data_size_readed = 0;

    int size = _buffer_size - _data_size - (_data - _buffer_ptr);

    // дополняем буффер данными из канала
    data_size_readed = recv(socket, _data + _data_size,  size, 0);

    std::cout << " recv data -> data_size = " << data_size_readed << std::endl;

    if (data_size_readed > 0)
        _data_size += data_size_readed;

    return (data_size_readed > 0);
}

void Buffer::reset()
{
    _data_size = 0;
    _data = _buffer_ptr;
}

void Buffer::sync()
{
    if (_data_size > 0)
    {
        int used = (_data - _buffer_ptr);
        int size = _buffer_size - _data_size - (_data - _buffer_ptr);

        if (used > size)
        {
            // если в буфере есть данные переносим их в вначало буфера
            for (int i = 0; i < _data_size; i++)
                _buffer_ptr[i] = _data[i];

            _data = _buffer_ptr;
        }
    }
}

bool Buffer::match_str(char* str)
{
    int charindex = 0;

    while (charindex < _data_size)
    {
        if (_data[charindex] == '\n') {

            int charcount = charindex+1;

            memcpy(str, _data, charcount);

            _data += charcount;
            _data_size -= charcount;
            
            return true;
        }

        charindex++;
    }

    return false;
}

}