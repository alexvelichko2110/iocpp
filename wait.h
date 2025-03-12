#ifndef WAIT_H
#define WAIT_H

#include <sys/time.h>

class Wait
{
public:
    Wait();

    // reset - clean memory struct to zero
    void clear();

    // lock process thread for waiting of event on sockets or file handels or timeout is up.
    bool lock_for_waiting(long max_lock_timeout);

    // setup socket handel to wait mask
    void set_handle_to_recv(int handle);

    // check sock or file handels with mask
    bool is_handle_to_recv(int handle);

private:

    int _maxfd;

    fd_set _rset;
    fd_set _wset;
    fd_set _xset;
};

#endif // WAIT_H
