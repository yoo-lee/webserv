#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

int main(int argc, char **argv)
{
    int fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
    int epfd = epoll_create1(0);
	// Epollは、このファイルディスクリプタ集合を監視し、指定されたイベントが発生した場合に通知します
	// event は、epoll_waitによって生成されたイベントに関する情報を保持する構造体です
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);

    struct epoll_event events[10];
    while (1) {
        int nfds = epoll_wait(epfd, events, 10, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            return 1;
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == fd) {
                printf("Data is available on file descriptor %d\n", fd);
            }
        }
    }

    close(fd);
    close(epfd);
    return 0;
}
