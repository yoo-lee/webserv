#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include "request.hpp"
#include "socket_data.hpp"

class Response : public SocketData{
public:
	Response();
    // コンストラクタ
    Response(Request& request);
    // HTTP レスポンスを構築するメソッド
    void buildResponse();
    // ステータスコードを設定するメソッド
    void setStatus(int code);
    // HTTP レスポンスを文字列で返す
    std::string getRes() const;
    // HTTP レスポンスのバイト数を返す
    int getLen() const;
    std::string getStatusMessage(int code);
	METHOD identify_method(std::string method);
	std::string identify_method(METHOD method);

	std::string methodToString(METHOD method);
    std::string get_file_path();

private:
    std::string file_path;
    Request &_request;
    std::string _res;
    std::string _body;
    int _code;

};

#endif /* RESPONSE_HPP */
