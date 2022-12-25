class HttpR
{
	
}


HttpResponse::CreateResponsePhase HttpResponse::ExecuteRequest(
    server::ConnSocket *conn_sock) {
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