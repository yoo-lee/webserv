#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "Config.hpp"
#include "request.hpp"
#include "socket_data.hpp"
#include <string>

using std::string;

class Response : public SocketData
{
  public:
    Response();
    // コンストラクタ
    Response(Request& request);
    // ステータス付与用コンストラクタ
    Response(Request& request, int status_code);
    // HTTP レスポンスを構築するメソッド
    void build_response();
    // ステータスコードを設定するメソッド
    void set_status(int code);
    // HTTP レスポンスを文字列で返す
    string get_response_string() const;
    // HTTP レスポンスのバイト数を返す
    int getLen() const;
    string get_status_message(int code);

  private:
    string _file_path;
    Request& _request;
    string _res;
    string _body;
    int _code;
};

#endif /* RESPONSE_HPP */
