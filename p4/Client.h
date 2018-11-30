//
// Created by wwt on 11/22/18.
//

#ifndef P4_CLIENT_H
#define P4_CLIENT_H

#include <iostream>

using namespace std;

class Client
{
    size_t buyNum;
    size_t sellNum;
    int netValue;
    string client;

public:
    explicit Client(string str);

    void update(bool isSell, size_t money, size_t quantity);

    void printTransfers();
};

#endif //P4_CLIENT_H
