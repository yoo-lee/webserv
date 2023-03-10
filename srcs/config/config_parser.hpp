#ifndef CONFIG_ONFIG_PARSER_HPP_
#define CONFIG_ONFIG_PARSER_HPP_

#include <climits>
#include <list>
#include <string>

#include "config/config.hpp"
#include "config/location_conf.hpp"
#include "config/virtual_server_conf.hpp"
#include "result/result.hpp"

namespace config {

using namespace result;

class Parser {
 private:
  std::string file_content_;
  size_t buf_idx_;

  std::set<std::string> location_set_directives_;

  // ピリオドを含むドメイン全体の長さ
  static const int kMaxDomainLength = 253;
  // ドメインの各ラベル(ピリオド区切りの文字列)の最大長
  static const int kMaxDomainLabelLength = 63;
  // ポート番号の最大値
  static const unsigned long kMaxPortNumber = 65535;

 public:
  Parser();
  Parser(const Parser &rhs);
  Parser &operator=(const Parser &rhs);
  ~Parser();

  // filepath のファイルデータを file_content_ に載せる
  void LoadFile(const std::string &filepath);

  // data の内容を file_content_ に載せる｡テストとかで使う｡
  void LoadData(const std::string &data);

  // config: server (NEWLINE server)*;
  Config ParseConfig();

  class ParserException : public std::exception {
   private:
    static const int MAX_ERROR_LEN = 1024;

   public:
    ParserException(const char *errfmt = "Parser error.", ...);
    const char *what() const throw();

   private:
    char errmsg_[MAX_ERROR_LEN];
  };

 private:
  // 文法の詳細は docks/configuration.g4 に書いてある｡

  // server block
  // server: 'server' '{' directive+ '}';
  void ParseServerBlock(Config &config);

  // listen_directive: 'listen' WHITESPACE NUMBER END_DIRECTIVE;
  void ParseListenDirective(VirtualServerConf &vserver);

  // servername_directive: 'server_name' WHITESPACE DOMAIN_NAME+ END_DIRECTIVE;
  void ParseServerNameDirective(VirtualServerConf &vserver);

  // location block
  // location_directive: 'location' PATH '{' directive_in_location+ '}';
  void ParseLocationBlock(VirtualServerConf &vserver,
                          bool is_location_back = false);

  void ParseAllowMethodDirective(LocationConf &location);

  void ParseClientMaxBodySizeDirective(LocationConf &location);

  void ParseRootDirective(LocationConf &location);

  void ParseIndexDirective(LocationConf &location);

  void ParseAutoindexDirective(LocationConf &location);

  void ParseErrorPageDirective(LocationConf &location);

  void ParseIscgiDirective(LocationConf &location);

  void ParseCgiExecutorDirective(LocationConf &location);

  void ParseReturnDirective(LocationConf &location);

  // Parser utils

  // 1文字content_buffer_[buf_idx_]を返して buf_idx_ を1進める
  char GetC();

  // buf_idx_ を1戻してbuffer_[buf_idx_]返す
  char UngetC();

  // 空白文字以外が見つかるまで buf_idx_ を進める
  void SkipSpaces();

  // GetC() して返り値がセミコロンかどうかをboolで返す
  bool GetCAndExpectSemicolon();

  // 次のスペース(改行等含む)までの文字列を取得し返す｡
  // そしてその文字数分 buf_idx_ を進める｡
  // 現在の buf_idx_ の位置が 'server {' の1文字目だった場合､ "server" を返す｡
  std::string GetWord();

  // 符号なし整数かどうか
  bool IsDigits(const std::string &str);

  // ドメイン名の条件に合っているか
  // https://www.nic.ad.jp/ja/dom/system.html
  //
  // DOMAIN_NAME: DOMAIN_LABEL ('.' DOMAIN_LABEL)* [:PORT];
  bool IsDomainName(std::string domain_name);

  // ドメインのラベル(ドメイン内の'.'で区切られた文字列のこと)
  // DOMAIN_LABEL: (ALPHABET | NUMBER)+
  // 	| (ALPHABET | NUMBER)+ (ALPHABET | NUMBER | HYPHEN)* (
  // 		ALPHABET
  // 		| NUMBER
  // 	)+;
  bool IsDomainLabel(const std::string &label);

  // webservで利用可能なメソッドか
  // METHOD: 'GET' | 'POST' | 'DELETE';
  bool IsHttpMethod(const std::string &method);

  // 正しいHTTPステータスコードか
  bool IsValidHttpStatusCode(const std::string &code);

  // "<IP>:<Port>" もしくは "<Port>"
  bool IsValidListenArgument(const std::string &word);

  // <num>.<num>.<num>.<num> の形式か｡
  // num は 0~255 の数字
  bool IsValidIp(const std::string &ip);

  std::pair<std::string, std::string> SplitToIpAndPort(
      const std::string &ip_port);

  // portが符号なし整数であり､ポート番号の範囲に収まっているかチェックする
  bool IsValidPort(const std::string &port);

  // "on" なら true, "off" なら false を返す｡
  bool ParseOnOff(const std::string &on_or_off);

  // file_content_ をすべて読み込んだか
  bool IsEofReached();

  // location_set_directives_を参照し､そのディレクティブが今見ているlocation内で既にセットされたかどうかを返す
  bool IsDirectiveSetInLocation(const std::string &directive);
};

}  // namespace config

#endif
