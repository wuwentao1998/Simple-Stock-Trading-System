//
// Created by wwt on 11/22/18.
//

#ifndef P4_STOCK_H
#define P4_STOCK_H

#include <iostream>
#include <set>
#include <vector>

using namespace std;

class Stock
{
    struct Order
    {
        size_t timeStamp;
        size_t price;
        bool isSell;
    };

//    struct cmpBuy
//    {
//        bool operator()(order *a, order *b)
//        {
//            if (a->price != b->price)
//                return a->price > b->price;
//        }
//    };
//
//    struct cmpSell
//    {
//        bool operator()(order *a, order *b)
//        {
//
//        }
//    };

public:
    string stockName;
    multiset<size_t> medianSet;
    vector<Order*> allOrder;

    explicit Stock(string str);

    void printMedian(size_t timeStamp);

    void addDeal(size_t timeStamp, size_t price, size_t quantity, bool isSell);

    void printTimeTraveler();

    void addOrder(size_t timeStamp, size_t price, bool isSell);
};







#endif //P4_STOCK_H
