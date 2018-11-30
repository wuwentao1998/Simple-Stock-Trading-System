//
// Created by wwt on 11/22/18.
//

#ifndef P4_MARKET_H
#define P4_MARKET_H
#include <map>
#include<unordered_map>
#include <set>
#include <vector>

#include <string>
#include <iostream>
#include <sstream>
#include "Client.h"
#include "Stock.h"
#include "Order.h"

using namespace std;



class Market {
private:
    struct sk
    {
        int buyTimeStamp;
        int sellTimeStamp;
        size_t highest;
        size_t lowest;
    };

    size_t curTimeStamp;
    size_t curID;
    size_t commission;
    size_t moneyTransferd;
    size_t tradeNum;
    size_t tradedShare;

    bool verbose;
    bool median;
    bool midpoint;
    bool transfers;

    stringstream stringStream;


    map<string, sk*> midHelper;
    map<string, Stock*> stockMap;
    map<string, Client*> clientMap;
    set<Order*, buyCmpOrder> buyOrder;
    set<Order*, sellCmpOrder> sellOrder;
    vector<string> timeTraveler;

public:
    Market();

    static Market& getInstance();

    void initTimeTraveler(string stockName);

    void parser(const string& stockOrder);

    void initOpts(bool verbose, bool median, bool midpoint, bool transfers);

    void printEndOfDay();


private:

    void printMedian();

    void printMidpoint();

    void removeExpired();

    void endOfTrade(size_t dealPrice, size_t quantity, const string& buyer,
            const string& seller, const string& stockName, bool isSell);
};
#endif //P4_MARKET_H
