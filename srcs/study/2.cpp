#include <cstddef>
#include <cstdint>
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
    char message[] = "Hello, World!";
    size_t message_size = sizeof(message);

    if (my_send(sockfd, message, message_size, 0) == -1) {
        std::cout << "Failed to send message." << std::endl;
    } else {
        std::cout << "Message sent successfully." << std::endl;
    }

    close(sockfd);

    return 0;
}

