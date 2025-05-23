#include "wait.h"

Wait::Wait()
{
    clear();
}

void Wait::clear()
{
    _maxfd = 0;

    FD_ZERO (&_rset);
    FD_ZERO (&_wset);
    FD_ZERO (&_xset);
}

bool Wait::lock_for_waiting(long max_lock_timeout)
{
    struct timeval tv;

    tv.tv_sec = max_lock_timeout / 1000;
    tv.tv_usec = (max_lock_timeout % 1000) * 1000;

    int rval = select (_maxfd + 1, &_rset, &_wset, &_xset, (max_lock_timeout == -1) ? nullptr : &tv);

    return (rval > 0);
}

void Wait::set_handle_to_recv(int handle)
{
    FD_SET(handle, &_rset);

    if (_maxfd < handle)
        _maxfd = handle;
}

bool Wait::is_handle_to_recv(int handle)
{
    bool check  = FD_ISSET(handle, &_rset);

//        else
//        if (mask & _SEND)
//            check = FD_ISSET(handle, &_wset);
//        else
//        if (mask & _ERROR)
//            check = FD_ISSET(handle, &_xset);

    return check;
}

bool Wait::is_handle_to_send(int handle)
{
    bool check  = FD_ISSET(handle, &_wset);

//        else
//        if (mask & _SEND)
//            check = FD_ISSET(handle, &_wset);
//        else
//        if (mask & _ERROR)
//            check = FD_ISSET(handle, &_xset);

    return check;
}
