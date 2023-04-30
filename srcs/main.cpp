
#include "webserv.hpp"
//#include "config.hpp"
#include <string>
#include <iostream>
#include "split.hpp"


using std::string;
using std::vector;
using std::cout;
using std::endl;

//std::string base_path = "/home/sano/work/42/webserv/github/webserv/srcs/dir";
std::string base_path = "srcs/dir";

int main(int argc, char const* argv[]) {




    (void)argc;
    (void)argv;
    /*
    if (argc != 2) {
        std::cout << "Error: Specify config file" << std::endl;
        return (EXIT_FAILURE);
    }
    */
    // config読み込み
    //Config config(std::string(argv[1]));

    //todo
    std::vector<std::string> ports;

    ports.push_back("11111");
    ports.push_back("11112");
    ports.push_back("11113");
    ports.push_back("11114");
    ports.push_back("11115");
    // ソケットの初期設定
    Webserv webserv = Webserv(ports);
    webserv.communication();
    // イベントループ開始
    // 終了時処理
    return 0;
}
