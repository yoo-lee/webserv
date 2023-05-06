#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#define NEVENTS 16

int main() {
    int sock1, sock2;
    struct sockaddr_in addr1, addr2;
    int epfd;
    struct epoll_event ev, ev_ret[NEVENTS];
    char buf[2048];
    int i;
    int nfds;
    int n;

    /* 受信ソケットを2つ作ります */
    sock1 = socket(AF_INET, SOCK_DGRAM, 0);
    sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    addr1.sin_family = AF_INET;
    addr2.sin_family = AF_INET;

    /* "127.0.0.1"の部分は必要に応じて変更して下さい */
    inet_pton(AF_INET, "127.0.0.1", &addr1.sin_addr.s_addr);
    inet_pton(AF_INET, "127.0.0.1", &addr2.sin_addr.s_addr);

    /* 2つの別々のポートで待つために別のポート番号をそれぞれ設定します */
    addr1.sin_port = htons(11111);
    addr2.sin_port = htons(22222);

    /* 2つの別々のポートで待つようにbindします */
    bind(sock1, (struct sockaddr*)&addr1, sizeof(addr1));
    bind(sock2, (struct sockaddr*)&addr2, sizeof(addr2));

    epfd = epoll_create(NEVENTS);
    if (epfd < 0) {
        perror("epoll_create");
        return 1;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sock1;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock1, &ev) != 0) {
        perror("epoll_ctl");
        return 1;
    }

    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sock2;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock2, &ev) != 0) {
        perror("epoll_ctl");
        return 1;
    }
    /*
    無限ループです
    このサンプルでは、この無限ループを抜けません
    */
    while (1) {
        printf("before epoll_wait\n");
        // epoll_waitは一旦受け取ると動く。何回も受け取りたい。
        nfds = epoll_wait(epfd, ev_ret, NEVENTS, -1); //fdに動きがあるまで待って、動きがあったfdを返す
        printf("%d\n",nfds);
        if (nfds <= 0) {
            perror("epoll_wait");
        }

        printf("after epoll_wait\n");

        for (i = 0; i < nfds; i++) {
            if (ev_ret[i].data.fd == sock1) {
                /* sock2に読み込み可能データがある場合 */
                /* sock1からデータを受信して表示します */
                n = recv(sock1, buf, sizeof(buf), 0);
                write(fileno(stdout), buf, n);

            } else if (ev_ret[i].data.fd == sock2) {
                // 別プロセス(CGI)起動
                /* sock2に読み込み可能データがある場合 */
                /* sock2からデータを受信して表示します */
                n = recv(sock2, buf, sizeof(buf), 0);
                write(fileno(stdout), buf, n);
            }
        }
    }

    /* このサンプルでは、ここへは到達しません */
    close(sock1);
    close(sock2);

    return 0;
}