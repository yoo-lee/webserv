
#include "webserv.hpp"
#include <string>
#include <iostream>
#include "split.hpp"
#include <stdlib.h>
#include "Config.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;

//std::string base_path = "/home/sano/work/42/webserv/github/webserv/srcs/dir";
std::string base_path = "srcs/dir";

void server(Webserv& webserv)
{
    try{
        webserv.communication();
    }catch(std::exception &e){
        cout << e.what() << endl;
    }
}

int main(int argc, char const* argv[]) 
{

    (void)argc;
    (void)argv;
    if (argc != 2) {
        std::cout << "Error: Specify config file" << std::endl;
        return (EXIT_FAILURE);
    }
    // config読み込み
    Config config(argv[1]);

    // ソケットの初期設定
    Webserv webserv = Webserv(config);
    // イベントループ開始
    while(1)
    {
        server(webserv);
    }
    // 終了時処理
    return 0;
}
