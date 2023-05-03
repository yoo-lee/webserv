#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int sockfd;
    int client_sockfd;
    struct sockaddr_in addr;

    socklen_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in from_addr;

    char buf[1024];

    // 受信バッファ初期化
    memset(buf, 0, sizeof(buf));

    // ソケット生成
    printf("gen socket\n");
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
    }

    // 待ち受け用IP・ポート番号設定
    printf("setting socket\n");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY;

    // サーバー=(listen)=socket=(bind)=IPアドレス<=========== クライアント

    // バインド
    printf("bind socket\n");
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
    }

    // 受信待ち
    printf("wait\n");
    if (listen(sockfd, SOMAXCONN) < 0) {
        printf("sockfd = %d\n", sockfd);
        perror("listen");
    }

    // クライアントからのコネクト要求待ち
    printf("waiting connect\n");
    if ((client_sockfd = accept(sockfd, (struct sockaddr*)&from_addr, &len)) <
        0) {
        perror("accept");
    }

    // 受信
    printf("receive\n");
    int rsize;
    while (1) {
        rsize = recv(client_sockfd, buf, sizeof(buf), 0);

        if (rsize == 0) {
            break;
        } else if (rsize == -1) {
            perror("recv");
        } else {
            printf("receive:%s\n", buf);
            sleep(1);

            // 応答
            printf("send:%s\n", buf);
            write(client_sockfd, buf, rsize);
        }
    }

    // ソケットクローズ
    close(client_sockfd);
    close(sockfd);

    return 0;
}