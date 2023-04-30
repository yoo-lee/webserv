// #include <cstddef>
// #include <cstdint>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

int my_send(int sockfd, const void *buf, size_t len, int flags) {
    // sockfd: 送信先のソケットファイルディスクリプタ
    // buf: 送信するデータの先頭アドレス
    // len: 送信するデータのバイト数
    // flags: フラグ

    int sent_bytes = 0;
    int total_bytes = 0;

    while (total_bytes < len) {
        sent_bytes = send(sockfd, (void*)((uintptr_t)buf + total_bytes), len - total_bytes, flags);
        if (sent_bytes < 0) {
            perror("send");
            return -1;
        }
        total_bytes += sent_bytes;
    }

    return total_bytes;
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(0);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("172.217.161.206"); // GoogleのIPアドレス
    server_addr.sin_port = htons(80); // HTTPのポート番号

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    char message[] = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
    size_t message_size = sizeof(message);

    if (my_send(sockfd, message, message_size, 0) == -1) {
        std::cout << "Failed to send message." << std::endl;
    } else {
        std::cout << "Message sent successfully." << std::endl;
    }

    close(sockfd);

    return 0;
}