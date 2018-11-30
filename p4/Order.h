//
// Created by wwt on 11/22/18.
//

#ifndef P4_ORDER_H
#define P4_ORDER_H

#include <iostream>

using namespace std;

enum action{BUY, SELL};

class Order{

public:
    int expire;
    size_t timeStamp, price, quantity, id;
    string clientName, stockName;
    action act;


    Order(size_t TimeStamp, const string& ClientName, action Act, const string& StockName,
          size_t Price, size_t Quantity, int Expire, size_t Id);

};


class buyCmpOrder{
public:
    bool operator()(Order* a, Order* b)
    {
        if (a->price != b->price)
            return a->price > b->price;
        else
            return a->id  < b->id;
    }
};

class sellCmpOrder{
public:
    bool operator()(Order* a, Order* b)
    {
        if (a->price != b->price)
            return a->price < b->price;
        else
            return a->id  < b->id;
    }
};



#endif //P4_ORDER_H
