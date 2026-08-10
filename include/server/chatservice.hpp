#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpConnection.h>
using namespace std;
using namespace muduo;
using namespace muduo::net;

#include <functional>
#include <unordered_map>

#include <mutex>

#include "json.hpp"
using json = nlohmann::json;

#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"

// 为等号右边定义一个别名MsgHandler
// std::function通用函数包装器，当前函数类型必须接收所定义的三个参数
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 聊天服务器业务类，单例模式，当前类在全局中只有一个实例化对象
class ChatService{
public:
    // 获取单例对象的接口函数
    static ChatService* instance();
    // 处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp);
    // 处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp);
    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp);
    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp);
    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    // 处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 从redis消息队列中获取订阅消息
    void handleRedisSubscribeMessage(int userid, string msg);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    // 服务器异常退出，业务重置方法
    void reset();
private:
    ChatService(); //将构造函数私有化，防止外部代码创建对象
    // 存储消息ID及其对应的业务处理方法
    unordered_map<int,MsgHandler> _msgHandlerMap;

    // 存储在线用户的通信连接 (要注意线程安全)
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    // 定义互斥锁，保证_userConnMap的线程安全
    mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    // redis操作对象
    Redis _redis;
};

#endif