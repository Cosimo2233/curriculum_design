#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <vector>
#include <string>
#include <algorithm>
#include "User.h"
#include "Stock.h"

class System
{
private:
    vector<User> users;   // 用户列表
    vector<Stock> stocks; // 股票列表
    User currentUser;     // 当前登录用户

    // 文件操作
    void loadUsers();  // 加载User.txt文件
    void saveUsers();  // 保存用户数据
    void loadStocks(); // 加载邮票文件
    void saveStocks(); // 保存邮票

    // 辅助函数
    void clearScreen();                  // 清屏
    void waitForKey();                   // 按任意键继续
    Stock *findStockByCode(string code); // 通过股票代码在股票类的对象中查找股票对象
    // 查询历史价格
    void searchHistoryPrice();

public:
    System(); // 构造函数

    // 账户管理
    void login();          // 登录
    void registerUser();   // 注册用户
    void setUserAsAdmin(); // 设置为管理员账户

    // 市场数据管理
    void addStock();      // （管理员）添加股票数据
    void modifyStock();   // （管理员）修改股票数据
    void showAllStocks(); // 展示所有股票数据

    // 数据操作
    void searchStock(); // 查询股票的菜单
    void statistics();  // 数据统计功能的总菜单
    void sortStocks();  // 对股票进行排序的总菜单
    // 删除股票
    void deleteStock();

    // 投资分析
    void analysis();
    void recommendation(); // 投资建议

    // 投资预测
    void prediction();
    // 预测辅助函数
    vector<double> predictLinear(const vector<double> &prices);    // 使用线性回归模型预测
    vector<double> predictWMA(const vector<double> &prices);       // 使用加权移动平均模型进行预测
    double calculateChangeRate(double currentPrice, double predictedPrice); // 计算涨跌幅

    // 主菜单
    void showMainMenu();
    void run();
};

#endif