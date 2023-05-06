#ifndef RESPONSE_HPP
#define RESPONSE_HPP

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
    // HTTP レスポンスを構築するメソッド
    void buildResponse();
    // ステータスコードを設定するメソッド
    void setStatus(int code);
    // HTTP レスポンスを文字列で返す
    string getRes() const;
    // HTTP レスポンスのバイト数を返す
    int getLen() const;
    string getStatusMessage(int code);
    METHOD identify_method(string method);
    string identify_method(METHOD method);

    string methodToString(METHOD method);
    string get_file_path();

  private:
    string _file_path;
    Request& _request;
    string _res;
    string _body;
    int _code;
};

#endif /* RESPONSE_HPP */
