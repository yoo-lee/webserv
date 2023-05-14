/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 11:57:34 by hsano             #+#    #+#             */
/*   Updated: 2023/05/14 01:35:52 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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

std::map< std::pair< std::string, std::string>, Server const *> Utility::_cfg_servers;
std::map< std::pair<std::string, std::string>, std::vector<std::string> > Utility::_cfg_locations;
std::map<pair<std::pair<std::string, std::string>, std::string> , std::map<std::string, std::vector<std::string> > > Utility::_cfg_locations_content;

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

    std::string por = "11112";
    std::string sname = "";
    Server const* servers = Utility::get_cfg_server(config, por, sname);
    cout << "server.listen:" << servers->listen << endl;
    cout << "server.default_server:" << servers->is_default_server << endl;

    std::string port = "11112";
    std::vector<std::string> test = Utility::get_cfg_locations(config, port, sname);
    std::vector<std::string> test2 = Utility::get_cfg_locations(config, port, sname);
    cout << "location size:" << test.size() << endl;
    for(size_t i=0;i<test.size();i++){
        cout << "location[" << i << "]:" << test[i] << endl;
        cout << "location[" << i << "]:" << test2[i] << endl;

        std::map<std::string, std::vector<std::string > > con1 =  Utility::get_cfg_locations_contents(config, port, sname, test[i]);
        std::map<std::string, std::vector<std::string > > con2 =  Utility::get_cfg_locations_contents(config, port, sname, test2[i]);
        std::map<std::string, std::vector<std::string > >::iterator ite1 = con1.begin();
        std::map<std::string, std::vector<std::string > >::iterator end1 = con1.end();
        std::map<std::string, std::vector<std::string > >::iterator ite2 = con2.begin();
        //std::map<std::string, std::vector<std::string > >::iterator end2 = con2.end();
        for(; ite1 != end1; ite1++){
            cout << "ite1:" << ite1->first << endl;
            cout << "ite2:" << ite2->first << endl;
            ite2++;
        }
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
