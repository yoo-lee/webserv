
#include "webserv.hpp"

int main(int argc, char const* argv[]) {
    // config読み込み
    // ソケットの初期設定
    Webserv webserv = Webserv();
    webserv.communication();
    // イベントループ開始
    // 終了時処理
    return 0;
}
