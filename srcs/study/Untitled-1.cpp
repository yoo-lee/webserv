

#include <sys/types.h>
#include <sys/socket.h>

int send(int fd, const void *buf, size_t len, int flags)
{
    return sendto(fd, buf, len, flags, NULL, 0);
}
