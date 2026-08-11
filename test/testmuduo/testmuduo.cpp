#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

class ChatServer
{
public:
    ChatServer(EventLoop *loop, //事件循环
               const InetAddress &listenAddr, //IP+Port
               const string &nameArg) //服务器的名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调（不知道何时调用（由网络库上报），先定义触发后该如何做）
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

        // 设置服务器端的线程数量 1个I/O线程，worker线程数量默认为0
        _server.setThreadNum(4);
    }
    void start(){
        _server.start();
    }
private:
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort()
                 << " state: online" << endl;
        }
        else
        {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort()
                 << " state: offline" << endl;
            conn->shutdown(); // close(fd)
            // _loop->quit();
        }
    }

    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data: " << buf << " time: " << time.toString() << endl;
        conn->send(buf);
    }

    TcpServer _server;
    EventLoop *_loop; //epoll
};

int main()
{
    EventLoop loop; // epoll

    InetAddress addr("192.168.238.128", 6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start(); // listenfd epoll_ctl=>epoll

    loop.loop();    // epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等

    return 0;
}