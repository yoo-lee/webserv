import urllib.request
import socket
import http
import time

from . import response_class as res

TIMEOUT = 10
TIMEOUT_MSG = "\nTIMEOUT\n"


def send_req(req_path, port, method="GET", body=None):
    if req_path[0] != "/":
        print("send_req invalid req_path", req_path)
        return res.response(-1, "invalid req_path")

    url = "http://127.0.0.1:" + str(port) + req_path

    if body != None:
        body = body.encode()

    req = urllib.request.Request(url, method=method, data=body)

    try:
        with urllib.request.urlopen(req, timeout=TIMEOUT) as r:
            code = r.code
            body = r.read().decode()
    except urllib.error.HTTPError as err:
        # print("send_req err :", err, url, port)
        code = err.code
        body = err.read().decode()
    except urllib.error.URLError as err:
        # print("send_req err :", err, url, port)
        code = -1
        body = str(err)
    except socket.timeout as err:
        # print("send_req err :", err, url, port)
        code = -1
        body = str(err)
    except http.client.RemoteDisconnected as err:
        # print("send_req err :", err, url, port)
        code = -1
        body = str(err)
    except http.client.IncompleteRead as err:
        code = -1
        body = err.partial.decode()
    return res.response(code, body)


# socket 通信でデータ送信
# req.txt などから読み込んで request を送信
def send_socket_request(send_data, port):
    timeout = TIMEOUT
    try:
        s = socket.socket(socket.AF_INET)
        s.settimeout(timeout)
        s.connect(("localhost", port))
        s.send(send_data.encode("utf-8"))
        time.sleep(0.1)
        return s.recv(10000).decode("utf-8")
    except socket.timeout:
        print(TIMEOUT_MSG)
        return TIMEOUT_MSG
