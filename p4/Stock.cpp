//
// Created by wwt on 11/22/18.
//

#include "Stock.h"


Stock::Stock(string str):
stockName(std::move(str)) {}

void Stock::addOrder(size_t timeStamp, size_t price, bool isSell)
{
    auto order = new Order{timeStamp, price, isSell};
    allOrder.push_back(order);
}

void Stock::addDeal(size_t timeStamp, size_t price, size_t quantity, bool isSell)
{
    medianSet.insert(price);

}

void Stock::printMedian(size_t timeStamp)
{
    size_t price = 0;
    size_t len = medianSet.size();
    if (len == 0)
        return;

    if ((len % 2) == 0)
    {
        size_t price1 = 0, price2 = 0;
        size_t count = 0;
        size_t pos = len / 2;

        for(auto it = medianSet.begin(); it != medianSet.end(); ++it)
        {
            if (count == pos - 1)
            {
                price1 = (*it);
                price2 = (*(++it));
                break;
            }
            ++count;
        }
        price = (price1 + price2) / 2;
    }
    else
    {
        size_t count = 0;
        size_t pos = len / 2;

        for(auto it = medianSet.begin(); it != medianSet.end(); ++it)
        {
            if (count == pos)
            {
                price = (*it);
                break;
            }
            ++count;
        }
    }

    cout << "Median match price of " << stockName << " at time " << timeStamp
         << " is $" << price << endl;
}

void Stock::printTimeTraveler()
{
    int buyTime = -1, sellTime = -1;
    size_t curMin = 9999999;
    int bestDeal = -9999999;

    for (auto it = allOrder.begin(); it != allOrder.end() ; ++it)
    {
        auto pt = (*it);
        if (pt->isSell)
        {
            auto buyPrice = pt->price;
            if (buyPrice >= curMin)
                continue;
            else
            {
                size_t curMax = 0;
                for (auto sellIt = it; sellIt != allOrder.end(); ++sellIt)
                {
                    auto sellPt = (*sellIt);
                    if (!(sellPt->isSell))
                    {
                        auto sellPrice = sellPt->price;
                        if (sellPrice <= curMax)
                            continue;
                        else
                        {
                            int curDeal = (int)(sellPrice - buyPrice);
                            if (curDeal > bestDeal)
                            {
                                bestDeal = curDeal;
                                sellTime = (int)sellPt->timeStamp;
                                buyTime = (int)pt->timeStamp;
                                curMin = buyPrice;
                                curMax = sellPrice;
                            }
                        }
                    }
                }
            }
        }
    }

    cout << "Time travelers would buy " <<  stockName << " at time: "
    << buyTime << " and sell it at time: " << sellTime << endl;
}


