#include "System.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

System::System()
{
    loadUsers();
    loadStocks();

    // 如果没有用户，创建默认管理员
    if (users.empty())
    {
        users.push_back(User("admin", "admin123", true));
        saveUsers();
    }
}

void System::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void System::waitForKey()
{
    cout << "\n按任意键继续...";
    cin.ignore();
    cin.get();
}

void System::loadUsers() // 读取 User.txt
{
    ifstream fin("users.txt");
    if (!fin)
        return;

    User user;
    while (fin >> user)
    {
        users.push_back(user);
    }
    fin.close();
}

void System::saveUsers() // 保存用户数据
{
    ofstream fout("users.txt");
    for (auto &user : users)
    {
        fout << user << endl;
    }
    fout.close();
}

void System::loadStocks()
{
    ifstream fin("stocks.txt");
    if (!fin)
    {
        cout << "注意：找不到 stocks.txt 文件！" << endl;
        cout << "请创建 stocks.txt 文件并添加股票数据。" << endl;
        cout << "格式：代码 名称 当前价格 涨跌幅 市盈率 行业 成交量 最近10天价格(10个数字)" << endl;
        cout << "示例：600519 贵州茅台 1800.50 0.00 40.20 白酒 1850000 1780.25 1785.00 1792.75 1795.60 1800.00 1802.10 1805.30 1798.20 1800.50 1803.80" << endl;
        waitForKey();
        return; // 不初始化任何数据，直接返回
    }

    // 清空现有股票列表
    stocks.clear();

    Stock stock;
    int count = 0;
    while (fin >> stock)
    {
        stocks.push_back(stock);
        count++;
    }
    fin.close();

    if (count == 0)
    {
        cout << "错误：stocks.txt 文件为空或格式不正确！" << endl;
        cout << "请确保文件包含至少一只股票的数据。" << endl;
        waitForKey();
    }
    else
    {
        cout << "系统初始化完成，一共存在 " << count << " 只股票。" << endl;
    }
}

void System::saveStocks()
{
    ofstream fout("stocks.txt");
    if (!fout)
    {
        cout << "错误：无法保存股票数据到文件！" << endl;
        return;
    }

    for (auto &stock : stocks)
    {
        fout << stock << endl;
    }
    fout.close();

    cout << "股票数据已保存到 stocks.txt" << endl;
}

Stock *System::findStockByCode(string code)
{
    for (auto &stock : stocks)
    {
        if (stock.getCode() == code)
        {
            return &stock;
        }
    }
    return nullptr;
}

// ===============帅气的分隔符-================账户管理================界面=====================
void System::login()
{
    clearScreen();
    cout << "============ 用户登录 ============\n";

    string username, password;
    int attemptCount = 0;
    const int MAX_ATTEMPTS = 5;

    cout << "              用户名: ";
    cin >> username;

    // 最多尝试5次密码
    while (attemptCount < MAX_ATTEMPTS)
    {
        if (attemptCount == 0) {
            cout << "              密码: ";
        }
        else {
            cout << "              密码(第" << (attemptCount + 1) << "次尝试): ";
        }

        cin >> password;
        attemptCount++;

        // 验证用户名和密码
        for (auto& user : users)
        {
            if (user.getUsername() == username && user.checkPassword(password))
            {
                currentUser = user;
                cout << "\n         登录成功！欢迎 " << username << endl;
                waitForKey();
                return;
            }
        }

        // 密码错误
        if (attemptCount < MAX_ATTEMPTS)
        {
            cout << "\n密码错误！还剩 " << (MAX_ATTEMPTS - attemptCount)
                << " 次尝试机会。" << endl;
        }
    }

    // 达到最大尝试次数
    cout << "\n密码错误次数超过" << MAX_ATTEMPTS << "次，系统将退出！" << endl;
    cout << "按任意键退出..." << endl;
    cin.ignore();
    cin.get();
    exit(0);  // 直接退出程序
}
void System::registerUser()
{
    clearScreen();
    cout << "============ 用户注册 ============\n";

    string username, password;
    cout << "           用户名: ";
    cin >> username;

    // 检查用户名是否存在
    for (auto &user : users)
    {
        if (user.getUsername() == username)
        {
            cout << "\n用户名已存在！" << endl;
            waitForKey();
            return;
        }
    }

    cout << "           密码: ";
    cin >> password;

    users.push_back(User(username, password, false));
    saveUsers();

    cout << "\n         注册成功！" << endl;
    waitForKey();
}
void System::setUserAsAdmin()
{
    clearScreen();
    cout << "============ 设置管理员账户 ============\n";

    // 检查当前用户是否为管理员
    if (!currentUser.getIsAdmin())
    {
        cout << "只有管理员可以设置其他账户为管理员！" << endl;
        waitForKey();
        return;
    }

    // 显示当前所有用户
    cout << "当前系统用户列表:\n";
    cout << "----------------------------------------\n";
    cout << "|用户名\t\t\t状态                        |\n";
    cout << "----------------------------------------\n";

    for (const auto &user : users)
    {
        cout << user.getUsername();
        if (user.getUsername().length() < 8)
            cout << "\t\t";
        else
            cout << "\t";

        if (user.getIsAdmin())
            cout << "管理员" << endl;
        else
            cout << "普通用户" << endl;
    }
    cout << "----------------------------------------\n";

    // 输入要设置的用户名
    string username;
    cout << "\n请输入要设置为管理员的用户名: ";
    cin >> username;

    // 检查是否是自己
    if (username == currentUser.getUsername())
    {
        cout << "不能修改自己的管理员状态！" << endl;
        waitForKey();
        return;
    }

    // 查找用户
    bool found = false;
    for (auto &user : users)
    {
        if (user.getUsername() == username)
        {
            found = true;

            // 检查是否已经是管理员
            if (user.getIsAdmin())
            {
                cout << "\n用户 " << username << " 已经是管理员！" << endl;
                waitForKey();
                return;
            }

            // 验证管理员密码
            string adminPassword;
            cout << "\n需要验证管理员权限!" << endl;
            cout << "请输入当前管理员密码进行验证: ";
            cin >> adminPassword;

            // 验证密码
            if (!currentUser.checkPassword(adminPassword))
            {
                cout << "\n密码错误!权限验证失败!" << endl;
                waitForKey();
                return;
            }

            // 确认操作
            cout << "\n密码验证通过!" << endl;
            cout << "\n确定要将用户 \"" << username << "\" 设置为管理员吗？(y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y')
            {
                user.setIsAdmin(true);
                saveUsers();
                cout << "\n用户 " << username << " 已成功设置为管理员！" << endl;

                // 显示设置后的用户状态
                cout << "\n----------------------------------------" << endl;
                cout << "用户名\t\t\t状态" << endl;
                cout << "----------------------------------------" << endl;
                cout << user.getUsername();
                if (user.getUsername().length() < 8)
                    cout << "\t\t";
                else
                    cout << "\t";
                cout << "       管理员 " << endl;
                cout << "----------------------------------------" << endl;
            }
            else
            {
                cout << "\n已取消操作。" << endl;
            }

            break;
        }
    }

    if (!found)
    {
        cout << "\n用户 \"" << username << "\" 不存在！" << endl;
    }

    waitForKey();
}

// ====================又是帅气的分隔符============================== 市场数据管理 ====================
void System::addStock()
{
    clearScreen();
    if (!currentUser.getIsAdmin())
    {
        cout << "只有管理员可以添加股票！" << endl;
        waitForKey();
        return;
    }

    cout << "============ 添加股票 ============\n";

    string code, name, industry;
    double price, pe;

    cout << "股票代码: ";
    cin >> code;

    if (findStockByCode(code))
    {
        cout << "股票代码已存在！" << endl;
        waitForKey();
        return;
    }

    cout << "股票名称: ";
    cin >> name;
    cout << "当前价格: ";
    cin >> price;
    cout << "市盈率: ";
    cin >> pe;
    cout << "所属行业: ";
    cin >> industry;

    // 要求输入最近10天价格
    vector<double> prices;
    prices.reserve(10);
    cout << "请输入最近10天价格(按时间顺序，最早到最近)，用空格或回车分隔:\n";
    for (int i = 0; i < 10; ++i)
    {
        double p;
        while (!(cin >> p))
        {
            cin.clear();
            string bad;
            cin >> bad;
            cout << "输入无效，请输入数字价格: ";
        }
        prices.push_back(p);
    }

    Stock s(code, name, price, pe, industry);
    s.setHistoryPrices(prices);
    stocks.push_back(s);
    saveStocks();

    cout << "\n股票添加成功!" << endl;
    waitForKey();
}

void System::modifyStock()
{
    clearScreen();
    if (!currentUser.getIsAdmin())
    {
        cout << "只有管理员可以修改股票！" << endl;
        waitForKey();
        return;
    }

    cout << "============ 修改股票 ============\n";

    string code;
    cout << "请输入要修改的股票代码: ";
    cin >> code;

    Stock *stock = findStockByCode(code);
    if (!stock)
    {
        cout << "股票不存在！" << endl;
        waitForKey();
        return;
    }

    cout << "\n当前信息:" << endl;
    stock->display();

    cout << "\n请输入新的信息:" << endl;
    string name, industry;
    double price, pe;

    cout << "股票名称: ";
    cin >> name;
    cout << "当前价格: ";
    cin >> price;
    cout << "市盈率: ";
    cin >> pe;
    cout << "所属行业: ";
    cin >> industry;

    stock->setName(name);
    stock->setPrice(price);
    stock->setPERatio(pe);
    stock->setIndustry(industry);

    saveStocks();
    cout << "\n修改成功！" << endl;
    // 输入最近10天价格并进行线性回归预测（只有管理员才拥有的程序）
    cout << "请输入最近10天价格 ";
    // char yn;
    // cin >> yn;

    vector<double> prices;
    prices.reserve(10);
    cout << "按时间顺序，最早到最近），用空格或回车分隔:\n";
    for (int i = 0; i < 10; ++i)
    {
        double p;
        while (!(cin >> p))
        {
            cin.clear();
            string bad;
            cin >> bad;
            cout << "输入无效，请输入数字价格: ";
        }
        prices.push_back(p);
    }

    // 保存历史价格到 Stock
    stock->setHistoryPrices(prices);

    // 保存到文件
    saveStocks();
    cout << "历史价格已保存到 stocks.txt" << endl;

    waitForKey();
}

void System::showAllStocks()
{
    clearScreen();
    cout << "------------------------------- 实时行情 ----------------------------------\n";
    if (stocks.empty())
    {
        cout << "股票列表为空！" << endl;
        cout << "请先添加股票数据或确保 stocks.txt 文件存在且格式正确。" << endl;
        waitForKey();
        return;
    }
    // 标题栏：调整列宽，确保对齐
    // 总宽度：8+16+12+10+10+12+12 = 80字符
    cout << setw(8) << left << "代码"
         << setw(16) << left << "名称"
         << setw(12) << right << "当前价格"
         << setw(10) << right << "涨跌幅"
         << setw(10) << right << "市盈率"
         << setw(12) << right << "行业"
         << setw(12) << right << "成交量" << endl; // 保持12

    // 分隔线：用"-"填充，总宽度80字符
    cout << string(80, '-') << endl;

    // 循环显示股票数据
    for (auto &stock : stocks)
    {
        stock.display();
    }

    // 底部统计信息
    cout << string(80, '-') << endl;
    cout << "总计: " << stocks.size() << " 只股票" << endl;
    waitForKey();
}
// ====================帅气的分隔符----------------------= 数据操作 ====================
void System::searchStock()
{
    clearScreen();

    if (stocks.empty())
    {
        cout << "股票列表为空，无法查询！" << endl;
        waitForKey();
        return;
    }

    cout << "1. 按代码查询\n";
    cout << "2. 按名称查询\n";
    cout << "3. 按行业查询\n";
    cout << "请选择: ";

    int choice;
    cin >> choice;

    string keyword;
    vector<Stock> results;

    switch (choice)
    {
    case 1:
        cout << "请输入股票代码: ";
        cin >> keyword;
        for (auto &stock : stocks) //
        {
            if (stock.getCode() == keyword)
            {
                results.push_back(stock);
            }
        }
        break;

    case 2:
        cout << "请输入股票名称: ";
        cin >> keyword;
        for (auto &stock : stocks)
        {
            if (stock.getName().find(keyword) != string::npos) //
            {
                results.push_back(stock);
            }
        }
        break;

    case 3:
        cout << "请输入行业名称: ";
        cin >> keyword;
        for (auto &stock : stocks)
        {
            if (stock.getIndustry() == keyword)
            {
                results.push_back(stock);
            }
        }
        break;

    default:
        cout << "无效选择！" << endl;
        waitForKey();
        return;
    }

    clearScreen();
    cout << "============ 查询结果 ============\n";
    if (results.empty())
    {
        cout << "未找到符合条件的股票。" << endl;
    }
    else
    {
        cout << "找到 " << results.size() << " 只符合条件的股票：\n";
        cout << "代码      名称               当前价格    涨跌幅    市盈率        行业      成交量\n";
        cout << "---------------------------------------------------------------------------------\n";
        for (auto &stock : results)
        {
            stock.display();
        }
        cout << "---------------------------------------------------------------------------------\n";
    }
    waitForKey();
}
// 删除股票
void System::deleteStock()
{
    clearScreen();
    if (!currentUser.getIsAdmin())
    {
        cout << "只有管理员可以删除股票！" << endl;
        waitForKey();
        return;
    }

    cout << "============ 删除股票 ============\n";

    string code;
    cout << "请输入要删除的股票代码: ";
    cin >> code;

    // 查找股票
    for (auto it = stocks.begin(); it != stocks.end(); ++it)
    {
        if (it->getCode() == code)
        {
            // 显示要删除的股票信息
            cout << "\n找到股票：" << endl;
            cout << "代码: " << it->getCode() << endl;
            cout << "名称: " << it->getName() << endl;
            cout << "价格: " << it->getPrice() << endl;
            cout << "行业: " << it->getIndustry() << endl;

            // 确认删除
            cout << "\n确定要删除这只股票吗？(y/n): ";
            char confirm;
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y')
            {
                stocks.erase(it);
                saveStocks();
                cout << "股票删除成功！" << endl;
            }
            else
            {
                cout << "取消删除。" << endl;
            }

            waitForKey();
            return;
        }
    }

    cout << "未找到股票代码为 \"" << code << "\" 的股票。" << endl;
    waitForKey();
}
void System::statistics() // 行业统计
{
    clearScreen();
    cout << "============ 数据统计 ============\n";

    if (stocks.empty())
    {
        cout << "股票列表为空，无法统计！" << endl;
        waitForKey();
        return;
    }

    // 统计股票数量
    cout << "股票总数: " << stocks.size() << endl;

    // 统计各行业股票数量
    cout << "\n行业分布:" << endl;
    cout << "行业     股票种类数量\n";
    cout << "----------------\n";

    vector<string> industries;
    for (auto &stock : stocks)
    {
        industries.push_back(stock.getIndustry());
    }

    // 去重
    sort(industries.begin(), industries.end());
    industries.erase(unique(industries.begin(), industries.end()), industries.end());

    // 统计并显示
    for (auto &industry : industries)
    {
        int count = 0;
        for (auto &stock : stocks)
        {
            if (stock.getIndustry() == industry)
                count++;
        }
        cout << setw(10) << left << industry;
        cout << setw(4) << right << count << endl;
    }

    // 价格区间统计
    cout << "\n价格区间分布:" << endl;
    int low = 0, mid = 0, high = 0;
    for (auto &stock : stocks)
    {
        double price = stock.getPrice();
        if (price < 10)
            low++;
        else if (price < 100)
            mid++;
        else
            high++;
    }

    // 使用图标显示
    cout << "0-10元:   ";
    for (int i = 0; i < low; i++)
        cout << "■";
    cout << " (" << low << ")\n";

    cout << "10-100元: ";
    for (int i = 0; i < mid; i++)
        cout << "■";
    cout << " (" << mid << ")\n";

    cout << "100元以上:";
    for (int i = 0; i < high; i++)
        cout << "■";
    cout << " (" << high << ")\n";

    waitForKey();
}
void System::searchHistoryPrice()
{
    clearScreen();
    cout << "============ 查询股票历史价格 ============\n";

    if (stocks.empty())
    {
        cout << "股票列表为空，无法查询！" << endl;
        waitForKey();
        return;
    }

    // 先查询股票
    string code;
    cout << "请输入要查询的股票代码: ";
    cin >> code;

    Stock *stock = findStockByCode(code);
    if (!stock)
    {
        cout << "股票不存在！" << endl;
        waitForKey();
        return;
    }

    // 显示股票基本信息
    cout << "\n股票: " << stock->getName() << " (" << code << ")" << endl;
    cout << "当前价格: " << fixed << setprecision(2) << stock->getPrice() << endl;

    // 获取历史价格数据
    vector<double> history = stock->getHistoryPrices();
    int historySize = static_cast<int>(history.size());

    if (historySize == 0)
    {
        cout << "该股票暂无历史价格数据！" << endl;
        waitForKey();
        return;
    }

    cout << "当前拥有 " << historySize << " 天的历史价格数据。" << endl;

    // 询问查询天数
    int daysAgo;
    cout << "\n你要查询多少天前的股票价格？（输入1-" << historySize << "）： ";
    cin >> daysAgo;

    // 验证输入
    if (daysAgo < 1 || daysAgo > historySize)
    {
        cout << "输入无效！请输入1-" << historySize << "之间的数字。" << endl;
        waitForKey();
        return;
    }

    // 计算索引（历史价格中，0是最早的价格，索引越大越近）
    // 假设历史价格按时间顺序存储：0=最早，n-1=最近
    int index = historySize - daysAgo; // 要查询n天前，就取倒数第n个

    if (index < 0)
        index = 0;

    // 获取价格
    double historicalPrice = history[index];

    // 显示结果
    cout << "\n═══════════════════════════════════════" << endl;
    cout << "        查询结果" << endl;
    cout << "═══════════════════════════════════════" << endl;
    cout << "股票代码: " << stock->getCode() << endl;
    cout << "股票名称: " << stock->getName() << endl;
    cout << "当前价格: " << fixed << setprecision(2) << stock->getPrice() << endl;
    cout << "查询天数: " << daysAgo << " 天前" << endl;
    cout << "历史价格: " << fixed << setprecision(2) << historicalPrice << endl;

    // 计算价格变化
    double currentPrice = stock->getPrice();
    double priceChange = currentPrice - historicalPrice;
    double changePercent = (priceChange / historicalPrice) * 100;

    cout << "价格变化: ";
    if (priceChange > 0)
    {
        cout << "\033[31m+" << fixed << setprecision(2) << priceChange;
        cout << " (+" << fixed << setprecision(2) << changePercent << "%)\033[0m" << endl;
    }
    else if (priceChange < 0)
    {
        cout << "\033[32m" << fixed << setprecision(2) << priceChange;
        cout << " (" << fixed << setprecision(2) << changePercent << "%)\033[0m" << endl;
    }
    else
    {
        cout << fixed << setprecision(2) << priceChange << " (0.00%)" << endl;
    }

    // 显示完整的历史价格时间线（假设索引0是最早，索引最大是最近）
    cout << "\n═══════════════════════════════════════" << endl;
    cout << "        历史价格时间线" << endl;
    cout << "═══════════════════════════════════════" << endl;
    cout << "时间顺序（从最早到最近）:" << endl;

    // 显示所有历史价格，并标注查询的那一天
    for (int i = 0; i < historySize; i++)
    {
        if (i == index)
        {
            cout << "\033[33m->" << (historySize - i) << "天前: "
                 << fixed << setprecision(2) << history[i] << " <-（查询位置）\033[0m" << endl;
        }
        else
        {
            cout << "  " << (historySize - i) << "天前: "
                 << fixed << setprecision(2) << history[i] << endl;
        }
    }

    cout << "═══════════════════════════════════════" << endl;

    waitForKey();
}
void System::sortStocks()
{
    clearScreen();
    cout << "============ 股票排序 ============\n";

    if (stocks.empty())
    {
        cout << "股票列表为空，无法排序！" << endl;
        waitForKey();
        return;
    }

    cout << "   1. 按价格从高到低\n";
    cout << "   2. 按价格从低到高\n";
    cout << "   3. 按涨跌幅从高到低\n";
    cout << "   4. 按市盈率从低到高\n";
    cout << "   请选择: ";

    int choice;
    cin >> choice;

    vector<Stock> sortedStocks = stocks;

    switch (choice)
    {
    case 1:
        sort(sortedStocks.begin(), sortedStocks.end(),
             [](const Stock &a, const Stock &b)
             { return a.getPrice() > b.getPrice(); });
        break;

    case 2:
        sort(sortedStocks.begin(), sortedStocks.end(),
             [](const Stock &a, const Stock &b)
             { return a.getPrice() < b.getPrice(); });
        break;

    case 3:
        sort(sortedStocks.begin(), sortedStocks.end(),
             [](const Stock &a, const Stock &b)
             { return a.getChange() > b.getChange(); });
        break;

    case 4:
        sort(sortedStocks.begin(), sortedStocks.end(),
             [](const Stock &a, const Stock &b)
             { return a.getPERatio() < b.getPERatio(); });
        break;

    default:
        cout << "无效选择！" << endl;
        waitForKey();
        return;
    }

    cout << "\n排序结果:" << endl;
    cout << "代码        名称            当前价格    涨跌幅     市盈率        行业      成交量\n";
    cout << "----------------------------------------------------------------------------------\n";
    for (auto &stock : sortedStocks)
    {
        stock.display();
    }
    waitForKey();
}

// ==================== 投资分析 ====================
void System::analysis()
{
    clearScreen();
    cout << "============ 投资分析 ============\n";

    if (stocks.empty())
    {
        cout << "股票列表为空，无法分析！" << endl;
        waitForKey();
        return;
    }

    // 市盈率分析
    cout << "市盈率分析:" << endl;
    int lowPE = 0, normalPE = 0, highPE = 0;

    for (auto &stock : stocks)
    {
        double pe = stock.getPERatio();
        if (pe < 15)
            lowPE++;
        else if (pe < 30)
            normalPE++;
        else
            highPE++;
    }

    cout << "低市盈率(<15):  " << lowPE << " 只\n";
    cout << "中市盈率(15-30):" << normalPE << " 只\n";
    cout << "高市盈率(>30):  " << highPE << " 只\n";

    // 涨跌幅分析
    cout << "\n涨跌幅分析:" << endl;
    int rise = 0, fall = 0, flat = 0;

    for (auto &stock : stocks)
    {
        double change = stock.getChange();
        if (change > 0)
            rise++;
        else if (change < 0)
            fall++;
        else
            flat++;
    }

    cout << "上涨: " << rise << " 只\n";
    cout << "下跌: " << fall << " 只\n";
    cout << "平盘: " << flat << " 只\n";

    waitForKey();
}

void System::recommendation()
{
    clearScreen();
    cout << "===================================== 股票推荐 ======================================\n";

    if (stocks.empty())
    {
        cout << "                           股票列表为空，无法推荐！" << endl;
        waitForKey();
        return;
    }

    // 推荐低市盈率股票
    cout << "低市盈率推荐（价值投资）:" << endl;
    cout << "代码        名称            当前价格      涨跌幅     市盈率       行业      成交量\n";
    cout << "-------------------------------------------------------------------------------------\n";

    int count = 0;
    for (auto &stock : stocks)
    {
        if (stock.getPERatio() < 20 && stock.getPERatio() > 0)
        {
            stock.display();
            count++;
            if (count >= 5)
                break; // 只显示前5个
        }
    }

    if (count == 0)
    {
        cout << "暂无符合条件的股票。" << endl;
    }

    waitForKey();
}

// 线性回归预测未来三天价格

vector<double> System::predictLinear(const vector<double> &prices)
{
    vector<double> predictions;
    int n = static_cast<int>(prices.size()); // 获取总天数

    if (n < 2)
    {
        // 数据不足，用最后一天价格填充
        double lastPrice = prices.empty() ? 0.0 : prices.back();
        predictions.push_back(lastPrice);
        predictions.push_back(lastPrice);
        predictions.push_back(lastPrice);
        return predictions;
    }

    // 线性回归计算     b = (sumXY - n*meanX*meanY) / (sumXX - n*meanX*meanX)
    double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
    for (int i = 0; i < n; ++i)
    {
        double x = static_cast<double>(i); // 时间变量
        double y = prices[i];              // 时间对应的价格变量
        sumX += x;                         // x的和
        sumY += y;                         // y的和
        sumXY += x * y;                    // xy的和
        sumXX += x * x;                    // x?的和
    }

    double meanX = sumX / n;              // x的平均数
    double meanY = sumY / n;              // y的平均数
    double b = sumXX - n * meanX * meanX; // 斜率b的值

    double slope = 0.0;
    if (fabs(b) > 1e-12)
    {
        slope = (sumXY - n * meanX * meanY) / b;
    }
    double a = meanY - slope * meanX; // 截距a的值

    // 预测未来三天
    for (int i = 1; i <= 3; ++i)
    {
        double nextX = static_cast<double>(n - 1 + i);
        double predicted = a + slope * nextX;
        predictions.push_back(predicted);
    }

    return predictions;
}

// 加权移动平均预测未来三天价格(近期的数据权重比较大，趋势影响更大)

vector<double> System::predictWMA(const vector<double> &prices)
{
    // 假设已经有至少10天数据
    int n = static_cast<int>(prices.size());

    if (n < 10)
    {
        // 万一数据不够，保守处理
        double lastPrice = prices.empty() ? 0.0 : prices.back();
        return vector<double>(3, lastPrice);
    }

    // 使用最近10天数据计算加权平均
    double weightedSum = 0.0;
    double weightSum = 0.0;

    // 权重：最近的天数权重高 (1,2,3,...,10)
    for (int i = n - 10; i < n; ++i)
    {
        double weight = (i - (n - 10) + 1); // 权重从1到10
        weightedSum += prices[i] * weight;  // 加权价格和
        weightSum += weight;                // 权重和
    }

    double basePrediction = weightedSum / weightSum; // WMA值

    // 以WMA为基准价格，下面使用最近三天的数据计算趋势，进行调整预测价格。

    // 计算趋势：用最近3天的平均变化（比只用最后一天更稳定）
    double recentTrend = 0.0;
    if (n >= 3)
    {
        // 最近3天的平均日变化
        double change1 = prices[n - 1] - prices[n - 2]; // 昨天的和前天的
        double change2 = prices[n - 2] - prices[n - 3]; // 前天的和大前天的
        recentTrend = (change1 + change2) / 2.0;        // 平均每天变化
    }

    // 预测未来三天
    vector<double> predictions;
    // 预测价格=基准价格+趋势*衰减因子*天数
    for (int day = 1; day <= 3; ++day)
    {
        // 趋势衰减：预测越远，趋势影响越小
        double decayFactor = 1.0 / (1.0 + day * 0.3); // 衰减因子
        double predicted = basePrediction + recentTrend * decayFactor * day;

        predictions.push_back(predicted);
    }

    return predictions;
}
// 计算涨跌幅百分比

double System::calculateChangeRate(double currentPrice, double predictedPrice)
{
    if (fabs(currentPrice) < 1e-12)
        return 0.0;
    return ((predictedPrice - currentPrice) / currentPrice) * 100.0;
}
// ==================== 投资预测 ====================
void System::prediction()
{
    clearScreen();
    cout << "============ 投资预测 ============\n";

    if (stocks.empty())
    {
        cout << "股票列表为空，无法预测！" << endl;
        waitForKey();
        return;
    }

    string code;
    cout << "请输入股票代码: ";
    cin >> code;

    Stock *stock = findStockByCode(code);
    if (!stock)
    {
        cout << "股票不存在！" << endl;
        waitForKey();
        return;
    }

    cout << "\n股票: " << stock->getName() << " (" << code << ")" << endl;
    cout << "当前价格: " << fixed << setprecision(2) << stock->getPrice() << endl;

    // 获取历史价格
    vector<double> prices = stock->getHistoryPrices();
    int historySize = static_cast<int>(prices.size());

    if (historySize < 5)
    {
        cout << "\n历史数据不足（当前仅有 " << historySize << " 天数据）" << endl;
        cout << "至少需要5天历史数据才能进行有效预测。" << endl;
        cout << "\n是否手动输入最近10天价格进行预测？(y/n): ";

        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            prices.clear();
            cout << "\n请输入最近10天价格（按时间顺序，最早到最近）:" << endl;
            for (int i = 0; i < 10; ++i)
            {
                cout << "第" << (i + 1) << "天价格: ";
                double p;
                while (!(cin >> p))
                {
                    cin.clear();
                    string bad;
                    cin >> bad;
                    cout << "输入无效，请输入数字价格: ";
                }
                prices.push_back(p);
            }
            historySize = 10;
        }
        else
        {
            waitForKey();
            return;
        }
    }

    // 显示最近价格
    cout << "\n最近" << min(10, historySize) << "天价格: ";
    int startIdx = historySize - min(10, historySize);
    for (int i = startIdx; i < historySize; ++i)
    {
        cout << fixed << setprecision(2) << prices[i] << " ";
    }
    cout << endl;

    // 选择预测模型
    cout << "\n============ 选择预测模型 ============\n";
    cout << "           1. 线性回归模型\n";
    cout << "           2. 加权移动平均模型\n";
    cout << "           请选择 (1/2): ";

    int modelChoice;
    cin >> modelChoice;

    if (modelChoice != 1 && modelChoice != 2)
    {
        cout << "使用默认模型（线性回归）" << endl;
        modelChoice = 1;
    }

    // 计算预测结果
    vector<double> predictions;

    if (modelChoice == 1)
    {
        cout << "\n使用线性回归模型预测..." << endl;
        predictions = predictLinear(prices);
    }
    else
    {
        cout << "\n使用加权移动平均模型预测..." << endl;
        predictions = predictWMA(prices);
    }

    double currentPrice = stock->getPrice();

    // 显示预测结果
    cout << "\n============ 预测结果 ============\n";
    cout << "当前价格: " << fixed << setprecision(2) << currentPrice << endl;
    cout << "---------------------------------\n";

    // 显示三天预测
    string dayNames[] = {"明天", "后天", "大后天"};

    for (int i = 0; i < 3; ++i)
    {
        double predPrice = predictions[i];
        double changeRate = ((predPrice - currentPrice) / currentPrice) * 100.0;

        cout << dayNames[i] << "预测: ";
        cout << fixed << setprecision(2) << predPrice;

        cout << " (";
        if (changeRate > 0)
        {
            cout << "+" << fixed << setprecision(2) << changeRate << "%)";
        }
        else if (changeRate < 0)
        {
            cout << fixed << setprecision(2) << changeRate << "%)";
        }
        else
        {
            cout << "0.00%)";
        }
        cout << endl;
    }

    // 简单投资建议
    cout << "\n============ 投资建议 ============\n";

    // 基于预测结果给出建议
    double avgChange = 0.0;
    for (int i = 0; i < 3; ++i)
    {
        avgChange += ((predictions[i] - currentPrice) / currentPrice) * 100.0;
    }
    avgChange /= 3.0;

    if (avgChange > 2.0)
    {
        cout << "预测整体上涨趋势明显，可考虑关注。" << endl;
    }
    else if (avgChange > 0)
    {
        cout << "预测小幅上涨，建议谨慎观望。" << endl;
    }
    else if (avgChange > -2.0)
    {
        cout << "预测变化不大，建议保持观望。" << endl;
    }
    else
    {
        cout << "预测呈下跌趋势，建议注意风险。" << endl;
    }

    // 基于市盈率的建议
    double peRatio = stock->getPERatio();
    if (peRatio > 0)
    {
        cout << "\n市盈率分析：" << endl;
        if (peRatio < 15)
            cout << "估值较低，具备投资价值" << endl;
        else if (peRatio < 30)
            cout << "估值合理" << endl;
        else if (peRatio < 50)
            cout << "估值偏高，需谨慎" << endl;
        else
            cout << "估值过高，注意风险" << endl;
    }

    cout << "\n提示：预测仅供参考，不构成投资建议!\n理财有风险，投资需谨慎！！！" << endl;
    cout << "===================================\n";

    waitForKey();
}
// ==================== 主菜单 ====================
void System::showMainMenu()
{
    clearScreen();
    cout << "============ 证券投资决策支持系统 ============\n";
    cout << "               当前用户: " << currentUser.getUsername();
    if (currentUser.getIsAdmin())
        cout << " (管理员)";
    cout << endl;
    cout << "系统状态: " << stocks.size() << " 只股票" << endl;
    cout << "==============================================\n";
    cout << "|                1. 账户管理                 |\n";
    cout << "|                2. 市场数据                 |\n";
    cout << "|                3. 股票数据                 |\n";
    cout << "|                4. 投资分析                 |\n";
    cout << "|                5. 投资预测                 |\n";
    cout << "|                0. 退出系统                 |\n";
    cout << "==============================================\n";
    cout << "                请选择 (0-5): ";
}

// ==================== 主菜单 ====================
void System::run()
{
    // 先登录
    while (currentUser.getUsername().empty())
    {
        clearScreen();
        cout << "========== ";  
        cout << "\033[32m";       
        cout << "证券投资决策支持系统";  
        cout << "\033[0m";       
        cout << " ===========" << endl;  
        cout << "|                  1. 登录                |\n";
        cout << "|                  2. 注册                |\n";
        cout << "|                  3. 退出                |\n";
        cout << "===========================================\n";
        cout << "                   请选择: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            login();
            break;
        case 2:
            registerUser();
            break;
        case 3:
            return;
        default:
            cout << "无效选择！" << endl;
            waitForKey();
        }
    }

    // 主菜单循环
    while (true)
    {
        showMainMenu();

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 0: // 退出
            cout << "感谢使用，再见!\n本程序由AAA张师傅完成！" << endl;
            return;

        case 1: // 账户管理
            clearScreen();
            cout << "============ 账户管理 ============\n";
            cout << "当前用户: " << currentUser.getUsername();
            if (currentUser.getIsAdmin())
                cout << " (管理员)";
            cout << endl;
            cout << "==================================\n";
            cout << "           1. 重新登录\n";
            cout << "           2. 注册新用户\n";

            // 只有管理员能看到这个选项
            if (currentUser.getIsAdmin())
            {
                cout << "           3. 设置用户为管理员\n";
            }

            cout << "           请选择: ";
            cin >> choice;

            if (choice == 1)
            {
                login();
            }
            else if (choice == 2)
            {
                // 注册新用户
                registerUser();
            }
            else if (choice == 3 && currentUser.getIsAdmin())
            {
                setUserAsAdmin();
            }
            else
            {
                cout << "无效选择！" << endl;
                waitForKey();
            }
            break;

        case 2: // 市场数据管理
            if (!currentUser.getIsAdmin())
            {
                showAllStocks();
            }
            else
            {
                clearScreen();
                cout << "1. 查看实时行情\n";
                cout << "2. 添加股票\n";
                cout << "3. 修改股票\n";
                cout << "4. 删除股票\n";

                cout << "请选择: ";
                cin >> choice;
                if (choice == 1)
                    showAllStocks();
                else if (choice == 2)
                    addStock();
                else if (choice == 3)
                    modifyStock();
                else if (choice == 4)
                    deleteStock();
            }
            break;

        case 3: // 数据操作
            clearScreen();
            cout << "1. 股票查询\n";
            cout << "2. 数据统计\n";
            cout << "3. 股票排序\n";
            cout << "4. 查询历史价格\n";
            cout << "请选择: ";
            cin >> choice;
            if (choice == 1)
                searchStock();
            else if (choice == 2)
                statistics();
            else if (choice == 3)
                sortStocks();
            else if (choice == 4)
                searchHistoryPrice();
            break;

        case 4: // 投资分析
            clearScreen();
            cout << "1. 投资分析\n";
            cout << "2. 股票推荐\n";
            cout << "请选择: ";
            cin >> choice;
            if (choice == 1)
                analysis();
            else if (choice == 2)
                recommendation();
            break;

        case 5: // 投资预测
            prediction();
            break;

        default:
            cout << "无效选择！" << endl;
            waitForKey();
        }
    }
}