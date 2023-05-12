
#include <stdlib.h>

#include <iostream>
#include <string>

#include "Config.hpp"
#include "splitted_string.hpp"
#include "webserv.hpp"
#include "base64.hpp"
#include "utility.hpp"

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

std::map<std::string, std::vector<std::string> > Utility::_cfg_locations;
//std::map<std::pair<std::string, std::string>, std::map<std::string, std::vector<std::string> > > _cfg_locations_contents;
//std::map<pair<std::string, std::string> , std::map<std::string, std::vector<std::string> > > Utility::_cfg_locations_contents;
std::map<pair<std::string, std::string> , std::vector<std::map<std::string, std::vector<std::string> > > > Utility::_cfg_locations_contents;
std::map<pair<std::string, std::string> , std::map<std::string, std::vector<std::string> > > Utility::_cfg_locations_content;
//std::vector<std::string> Utility::_cfg_root_dir;
//.std::vector<std::string> Utility::_cfg_extension;
//std::map<std::string, std::vector<std::string> > Utility::_cfg_root_dir;
//std::map<std::string, std::vector<std::string> > Utility::_cfg_extension;
//std::map<std::pair<std::string, std::string>, std::vector<std::map<std::string, std::string> > > Utility::_cfg_locations_contents;
//int Utility::i;
//std::vector<int> Utility::ii;
//int Utility::tmp;
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

    std::string port = "11111";
    std::vector<std::string> test = Utility::get_cfg_locations(config, port);
    std::vector<std::string> test2 = Utility::get_cfg_locations(config, port);
    cout << test.size() << endl;
    for(size_t i=0;i<test.size();i++){
        cout << test[i] << endl;
        cout << test2[i] << endl;
    }

    // ソケットの初期設定
    Webserv webserv = Webserv(config);
    // イベントループ開始
    while (1) {
        server(webserv);
    }
    // 終了時処理
    return 0;
}
