namespace server 
{
class ConnSocket;
}
//epoll 等

class HttpResponse 
{
protected:
  // レスポンスの種類
enum EResponseType
{ kHttpResponse, kHttpErrorResponse, kHttpCgiResponse };

  // レスポンスの作成状況
  enum CreateResponsePhase
  {
    kExecuteRequest,
    kStatusAndHeader,
    kBody,
    kComplete
  };

  CreateResponsePhase ExecuteGetRequest(const http::HttpRequest &request);
  CreateResponsePhase ExecutePostRequest(const server::ConnSocket *conn_sock, const http::HttpRequest &request);
  CreateResponsePhase ExecuteDeleteRequest(const http::HttpRequest &request);
}
// ------       ↑header↑        --------------

HttpResponse::HttpResponse(const config::LocationConf *location, server::Epoll *epoll, EResponseType response_type)
    : location_(location),
      epoll_(epoll),
      phase_(kExecuteRequest),
      http_version_(kDefaultHttpVersion),
      status_(OK),
      status_message_(StatusCodes::GetMessage(OK)),
	  //303 etc
      headers_(),
      write_buffer_(),
      file_fd_(-1),
      response_type_(response_type) {assert(epoll_ != NULL);
}

HttpResponse::CreateResponsePhase
HttpResponse::ExecuteRequest(server::ConnSocket *conn_sock)
{
  http::HttpRequest &request = conn_sock->GetRequests().front();
  const std::string method = request.GetMethod();

  if (method == method_strs::kGet)
    return ExecuteGetRequest(request);
  else if (method == method_strs::kPost)
    return ExecutePostRequest(conn_sock, request);
  else if (method == method_strs::kDelete)
    return ExecuteDeleteRequest(request);
  else
    assert(false);
}