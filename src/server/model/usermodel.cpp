#include "usermodel.hpp"
#include "db.h"
#include <iostream>

using namespace std;

#include <muduo/base/Logging.h>
using namespace muduo;

// user表的增加方法
bool UserModel::insert(User &user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str()); // 拼接字符串，并将string转换成char *
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            // 获取插入成功的用户ID
            user.setId(mysql_insert_id(mysql.getConnection())); //mysql.getConnection()返回的是MYSQL*
            return true;
        }
    }
    return false;
}

// 根据用户ID查询用户信息
User UserModel::query(int id)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id = %d", id);

    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res); // 释放内存空间
                return user;
            }
        }
    }

    return User(); // 如果没有ID对应的用户，则返回默认的用户User(int id=-1, string name="", string pwd="", string state="offline")
}

bool UserModel::updateState(User user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d", user.getState().c_str(), user.getId()); // 拼接字符串，并将string转换成char *

    LOG_INFO << "Update SQL: " << sql;
    
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            return true;
        }
        else {
             // 如果 update 返回 false，说明 SQL 执行出错
             LOG_ERROR << "Update failed for id: " << user.getId();
        }
    }
    return false;
}

// 重置用户的状态信息
void UserModel::resetState()
{
    // 1.组装sql语句
    char sql[1024] = "update user set state = 'offline' where state = 'online'";
    
    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
    }
}