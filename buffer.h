#ifndef BUFFER_H
#define BUFFER_H

#include <string>

namespace net {

class Buffer
{
public:

    virtual ~Buffer ();
    Buffer ();

    bool load_from_socket(int socket);

    void sync();

    void reset();

    bool is_full()
    {
        if (_data_size >= _buffer_size)
            return true;
        
        return false;
    }

    bool is_empty()
    {
        if (_data_size == 0)
            return true;
        
        return false;
    }

    bool match_str(char* str);

private:

    // buffer memmory pointer
    char *_buffer_ptr;

    int _buffer_size;

    int _data_size;
    char *_data;
};

}


#endif // BUFFER_H
