#ifndef __USER_H
#define __USER_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class User
{
private:
    string username;
    string password;
    bool isAdmin;       //是否为管理员

public:
    User() : isAdmin(false) {}                      //检查是否使管理员
    User(string u, string p, bool admin = false);

    // 获取用户数据
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    bool getIsAdmin() const { return isAdmin; }

    // 设置用户数据
    void setUsername(string u) { username = u; }
    void setPassword(string p) { password = p; }
    void setIsAdmin(bool admin) { isAdmin = admin; }

    // 验证密码
    bool checkPassword(string p) const { return password == p; }

    // 文件操作
    friend ofstream &operator<<(ofstream &fout, const User &user);//输出信息到文件
    friend ifstream &operator>>(ifstream &fin, User &user);         //读取文件信息
};

#endif