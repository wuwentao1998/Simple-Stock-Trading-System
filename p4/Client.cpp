//
// Created by wwt on 11/22/18.
//

#include "Client.h"

Client::Client(string str):
buyNum(0), sellNum(0), netValue(0), client(std::move(str)) {}

void Client::update(bool isSell, size_t money, size_t quantity)
{
    if (isSell)
    {
        sellNum += quantity;
        netValue += money * quantity;
    }
    else
    {
        buyNum += quantity;
        netValue -= money * quantity;
    }
}

void Client::printTransfers()
{
    cout << client << " bought " << buyNum << " and sold " << sellNum
    << " for a net transfer of $" << netValue << endl;
}