#include "Stock.h"
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

Stock::Stock(string c, string n, double p, double pe, string i)
    : code(c), name(n), price(p), peRatio(pe), industry(i), change(0), volume(0)
{
}

void Stock::setPrice(double p)
{
    price = p;
}

void Stock::updatePrice(double newPrice)
{
    // 计算涨跌幅
    if (price > 0)
    {
        change = ((newPrice - price) / price) * 100;
    }
    price = newPrice;
}

void Stock::display() const
{
    // 修改列宽
    cout << setw(8) << left << code;                          // 股票代码
    cout << setw(16) << left << name;                         // 股票名称
    cout << setw(12) << right << fixed << setprecision(2) << price; // 当前价格

    // 涨跌幅：调整为10字符宽，包含正负号和百分号
    cout << setw(10) << right;
    if (change > 0)
    {
        //ANSI颜色代码
        cout << "\033[31m+" << fixed << setprecision(2) << change << "%\033[0m"; // 红色上涨
    }
    else if (change < 0)
    {
        cout << "\033[32m" << fixed << setprecision(2) << change << "%\033[0m";  // 绿色下跌
    }
    else
    {
        cout << fixed << setprecision(2) << change << "%"; // 平盘，前面加空格对齐
    }

    // 市盈率：调整为10字符宽
    cout << setw(10) << right << fixed << setprecision(2) << peRatio;

    // 行业：调整为12字符宽，左对齐
    cout << setw(12) << right << industry;

    // 成交量：调整为12字符宽，右对齐
    cout << setw(12) << right << volume;

    cout << endl;
}
ofstream& operator<<(ofstream& fout, const Stock& stock)
{
    fout << stock.code << " ";
    fout << stock.name << " ";
    fout << stock.price << " ";
    fout << stock.change << " ";
    fout << stock.peRatio << " ";
    fout << stock.industry << " ";
    fout << stock.volume << " ";

    // 保存最近10天历史价格（每行末尾10个数字）
    const int WANT = 10;
    int h = static_cast<int>(stock.historyPrices.size());
    double pad = stock.price;
    // 如果不足10个，用当前价格填充到10个
    if (h >= WANT)
    {
        for (int i = h - WANT; i < h; ++i)
            fout << stock.historyPrices[i] << " ";
    }
    else
    {
        for (int i = 0; i < WANT - h; ++i)
            fout << pad << " ";
        for (int i = 0; i < h; ++i)
            fout << stock.historyPrices[i] << " ";
    }

    return fout;
}

ifstream& operator>>(ifstream& fin, Stock& stock)
{
    // 读取整行并解析：最后10个数字视为最近10天价格
    std::string line;
    if (!std::getline(fin, line))
        return fin;

    std::istringstream iss(line);
    iss >> stock.code;
    iss >> stock.name;
    iss >> stock.price;
    iss >> stock.change;
    iss >> stock.peRatio;
    iss >> stock.industry;
    iss >> stock.volume;

    // 读取余下所有数字作为历史价格候选
    std::vector<double> allPrices;
    double p;
    while (iss >> p)
    {
        allPrices.push_back(p);
    }

    stock.historyPrices.clear();
    const int WANT = 10;
    int m = static_cast<int>(allPrices.size());
    if (m >= WANT)
    {
        for (int i = m - WANT; i < m; ++i)
            stock.historyPrices.push_back(allPrices[i]);
    }
    else
    {
        // 如果不足10个，则用当前价格前置填充
        for (int i = 0; i < WANT - m; ++i)
            stock.historyPrices.push_back(stock.price);
        for (int i = 0; i < m; ++i)
            stock.historyPrices.push_back(allPrices[i]);
    }

    return fin;
}