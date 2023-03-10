#ifndef HTTP_HTTP_CGI_RESPONSE_HPP_
#define HTTP_HTTP_CGI_RESPONSE_HPP_

#include "cgi/cgi_process.hpp"
#include "http/http_response.hpp"
#include "http/http_status.hpp"

namespace http {

class HttpCgiResponse : public HttpResponse {
 private:
  cgi::CgiProcess *cgi_process_;

  static const unsigned long kMaxChunkSize = 1024;  // 1KB
  static const std::string kLastChunk;

 public:
  HttpCgiResponse(const config::LocationConf *location, server::Epoll *epoll,
                  server::ConnSocket *socket);
  ~HttpCgiResponse();

 private:
  CreateResponsePhase ExecuteRequest(server::ConnSocket *conn_sock);
  Result<CreateResponsePhase> MakeResponseBody();

  CreateResponsePhase MakeDocumentResponse(server::ConnSocket *conn_sock);
  CreateResponsePhase MakeLocalRedirectResponse(server::ConnSocket *conn_sock);
  CreateResponsePhase MakeClientRedirectResponse(server::ConnSocket *conn_sock);

  Result<void> SetStatusFromCgiResponse();
  void SetHeadersFromCgiResponse();

  // LocalRedirect の結果に基づき新しいリクエストを作成
  HttpRequest CreateLocalRedirectRequest(const HttpRequest &request);
};

}  // namespace http

#endif
