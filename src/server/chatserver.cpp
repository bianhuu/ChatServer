#include "chatserver.hpp"
#include "json.hpp"
#include <functional>
#include <string>

#include "chatservice.hpp"
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop *loop,//ChatServer::告诉编译器这是在实现ChatServer类中的函数
           const InetAddress &listenAddr,
           const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    // 给服务器注册用户连接的创建和断开回调（不知道何时调用（由网络库上报），先定义触发后该如何做）
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

    // 给服务器注册用户读写事件回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    // 设置服务器端的线程数量 1个I/O线程（处理新用户的连接），worker线程数量默认为0（处理已连接用户的读写事件）
    _server.setThreadNum(4);
}

// 启动服务
void ChatServer::start()
{
    _server.start();
}

// 处理用户连接事件
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if(!conn->connected())//客户端断开连接
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 专门处理用户的读写事件
void ChatServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
{
    string buf=buffer->retrieveAllAsString(); //将缓存区里的内容转换成字符串
    json js = json::parse(buf); //数据的反序列化
    // 设计思路，将网络模块与业务模块进行解耦
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>()); //将json类型转换成整型
    // 回调消息ID绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn,js,time);
}