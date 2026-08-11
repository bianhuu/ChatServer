#include "friendmodel.hpp"
#include "db.h"

// 添加好友关系，好友表为联合主键，防止重复建立好友关系
void FriendModel::insert(int userid, int friendid)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into friend values(%d, %d)", userid, friendid);

    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
    return;
}

// 返回用户好友列表 由friend表返回好友id，再查询user表中好友id对应的信息（name，state），实现表的联合查询
vector<User> FriendModel::query(int userid)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select u.id,u.name,u.state from friend f join user u on f.friendid=u.id where f.userid=%d", userid);

    vector<User> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            // 把userid用户的所有离线消息放入vec中返回
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}