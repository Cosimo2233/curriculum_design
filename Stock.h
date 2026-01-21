#ifndef __STOCK_H
#define __STOCK_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

class Stock {
private:
    string code;           // 股票代码
    string name;           // 股票名称
    double price;          // 当前价格
    double change;         // 涨跌幅
    double peRatio;        // 市盈率
    string industry;       // 行业
    long volume;          // 成交量
    vector<double> historyPrices;  // 历史价格数组最近10天

public:
    Stock(){}
    Stock(string c, string n, double p, double pe, string i);

    // 获取股票信息
    string getCode() const { return code; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    double getChange() const { return change; }//获取涨跌幅
    double getPERatio() const { return peRatio; }//获取市盈率
    string getIndustry() const { return industry; }//获取行业信息
    long getVolume() const { return volume; }//获取成交量
    vector <double> getHistoryPrices() const { return historyPrices; }  
    
    // 设置股票信息
    void setCode(string c) { code = c; }
    void setName(string n) { name = n; }
    void setPrice(double p);
    void setPERatio(double pe) { peRatio = pe; }
    void setIndustry(string i) { industry = i; }
    void setVolume(long v) { volume = v; }//设置成交量
    void setHistoryPrices(const vector<double>& prices) { historyPrices = prices; }  

    // 添加历史价格
    void addHistoryPrice(double price)  
    { 
        historyPrices.push_back(price);
        // 只保留最近10天的数据
        if (historyPrices.size() > 10) 
        {
            historyPrices.erase(historyPrices.begin());
        }
    }

    // 更新价格并计算涨跌幅
    void updatePrice(double newPrice);

    // 显示股票信息
    void display() const;

    // 显示历史价格
    void displayHistory() const 
    {  
        cout << "最近" << historyPrices.size() << "天历史价格: ";
        for (double p : historyPrices) {
            cout << fixed << setprecision(2) << p << " ";
        }
        cout << endl;
    }

    friend ofstream& operator<<(ofstream& fout, const Stock& stock);// 输出股票信息到文件
    friend ifstream& operator>>(ifstream& fin, Stock& stock);       //从为文件获取股票信息
};

#endif