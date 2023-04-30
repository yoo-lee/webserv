#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include "request.hpp"

class Response {
public:
	Response();
    // コンストラクタ
    Response(Request &request);
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
	bool send();

private:
    Request _request;
    std::string _res;
    std::string _body;
    int _code;

};

#endif /* RESPONSE_HPP */
