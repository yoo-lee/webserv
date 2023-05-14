
#include <stdlib.h>

#include <iostream>
#include <string>

#include "Config.hpp"
#include "base64.hpp"
#include "splitted_string.hpp"
#include "utility.hpp"
#include "webserv.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

// std::string base_path = "/home/sano/work/42/webserv/github/webserv/srcs/dir";
std::string base_path = "srcs/dir";

void server(Webserv& webserv)
{
    // try {
    webserv.process_communication();
    // } catch (std::exception& e) {
    // cout << e.what() << endl;
    // }
}

std::map<std::pair<std::string, std::string>, Server const*> Utility::_cfg_servers;
std::map<std::pair<std::string, std::string>, std::vector<std::string> > Utility::_cfg_locations;
std::map<pair<std::pair<std::string, std::string>, std::string>, std::map<std::string, std::vector<std::string> > >
    Utility::_cfg_locations_content;

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
    while (1) {
        server(webserv);
    }
    // 終了時処理
    return 0;
}
