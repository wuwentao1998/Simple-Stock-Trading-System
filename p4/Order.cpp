//
// Created by wwt on 11/22/18.
//

#include "Order.h"


 Order::Order(size_t TimeStamp, const string& ClientName, action Act, const string& StockName,
        size_t Price, size_t Quantity, int Expire, size_t Id):
        timeStamp(TimeStamp), clientName(ClientName), act(Act), stockName(StockName),
        price(Price), quantity(Quantity), expire(Expire), id(Id) {}



