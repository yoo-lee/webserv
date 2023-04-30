
#include "webserv.hpp"
//#include "config.hpp"
#include <string>
#include <iostream>
#include "split.hpp"


using std::string;
using std::vector;
using std::cout;
using std::endl;
int main(int argc, char const* argv[]) {

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
	
    // ソケットの初期設定
    Webserv webserv = Webserv(ports);
    webserv.communication();
    // イベントループ開始
    // 終了時処理
    return 0;
}
