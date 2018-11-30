//
// Created by wwt on 11/22/18.
//

#include "Market.h"


Market::Market():
curTimeStamp(0), curID(0), commission(0), moneyTransferd(0), tradeNum(0), tradedShare(0) {}

Market& Market::getInstance()
{
    static Market market;
    return market;
}

void Market::initOpts(bool verbose, bool median, bool midpoint, bool transfers)
{
    this->verbose = verbose;
    this->median = median;
    this->midpoint = midpoint;
    this->transfers = transfers;
}

void Market::initTimeTraveler(string stockName)
{
    timeTraveler.push_back(std::move(stockName));
}


void Market::parser(const string& stockOrder)
{
    this->stringStream.clear();
    this->stringStream.str(stockOrder);

    int duration;
    size_t timeStamp;
    string clientName, action, stockName, priceString, quantityString;

    stringStream >> timeStamp >> clientName >> action
    >> stockName >> priceString >> quantityString >> duration;


    if (timeStamp != curTimeStamp)
    {
        if(median)
            printMedian();
        if(midpoint)
            printMidpoint();

        curTimeStamp = timeStamp;

        removeExpired();
    }

    if (midHelper.find(stockName) == midHelper.end())
    {
        auto pt = new sk{-1, -1, 0, 0};
        midHelper.emplace(stockName, pt);
    }

    enum action act = (action == "BUY") ? BUY : SELL; //why 'enum' need to write?

    auto price = (size_t)strtoul(priceString.c_str() + 1, nullptr, 10);
    auto quantity = (size_t)strtoul(quantityString.c_str() + 1, nullptr, 10);

    int expire = duration > 0 ? (int)(timeStamp + duration) : duration;


    for (auto it = timeTraveler.begin(); it != timeTraveler.end() ; ++it)
    {
        if ((*it) != stockName)
            continue;

        auto pt = stockMap.find((*it));

        bool isSell = (act == SELL);
        if (pt == stockMap.end())
        {
            string tempStock((*it));
            auto stock = new Stock(tempStock);
            stockMap.emplace(std::move(tempStock), stock);
            stock->addOrder(curTimeStamp, price, isSell);
        }
        else
        {
            (*pt).second->addOrder(curTimeStamp, price, isSell);
        }

    }


    size_t dealPrice;
    if(duration != 0)
    {
        auto order = new Order(timeStamp, clientName, act, stockName,
                 price,  quantity,  expire, curID);
        if (act == BUY)
            buyOrder.insert(order);
        else
            sellOrder.insert(order);

        //TODO : test different stackName

        if (!buyOrder.empty() && !sellOrder.empty())
        {
            if (act == BUY)
            {
                auto buyer = order; //actually it must be the new added buyer

                while (buyer->quantity > 0)
                {
                    if (sellOrder.empty())
                    {
                        ++curID;
                        return;
                    }

                    Order* seller = nullptr;
                    for(auto it : sellOrder)
                    {
                        if (it->stockName == stockName)
                        {
                            seller = it;
                            break;
                        }
                    }


                    if (seller == nullptr || seller->price > buyer->price)
                    {
                        ++curID;
                        return;
                    }
                    else
                    {
                        if(seller->id < buyer->id)
                            dealPrice = seller->price;
                        else
                            dealPrice = buyer->price;

                        if (seller->quantity >= buyer->quantity)
                        {
                            seller->quantity -= buyer->quantity;
                            endOfTrade(dealPrice, buyer->quantity, buyer->clientName, seller->clientName,
                                    buyer->stockName, false);

                            if (seller->quantity == 0)
                                sellOrder.erase(seller);

                            buyer->quantity = 0;
                        }
                        else
                        {
                            buyer->quantity -= seller->quantity;
                            endOfTrade(dealPrice, seller->quantity, buyer->clientName, seller->clientName,
                                    buyer->stockName, false);
                            sellOrder.erase(seller);
                        }
                    }
                }

                buyOrder.erase(buyer);
            }
            else
            {
                auto seller = order; //actually it must be the new added seller

                while (seller->quantity > 0)
                {
                    if (buyOrder.empty())
                    {
                        ++curID;
                        return;
                    }

                    Order* buyer = nullptr;
                    for(auto it : buyOrder)
                    {
                        if (it->stockName == stockName)
                        {
                            buyer = it;
                            break;
                        }
                    }

                    if (buyer == nullptr || buyer->price < seller->price)
                    {
                        ++curID;
                        return;
                    }
                    else
                    {
                        if(seller->id < buyer->id)
                            dealPrice = seller->price;
                        else
                            dealPrice = buyer->price;

                        if (seller->quantity >= buyer->quantity)
                        {
                            seller->quantity -= buyer->quantity;
                            endOfTrade(dealPrice, buyer->quantity, buyer->clientName,
                                    seller->clientName, buyer->stockName, true);
                            buyOrder.erase(buyer);
                        }
                        else
                        {
                            buyer->quantity -= seller->quantity;
                            endOfTrade(dealPrice, seller->quantity, buyer->clientName,
                                    seller->clientName, buyer->stockName, true);
                            seller->quantity = 0;
                        }
                    }
                }

                sellOrder.erase(seller);
            }
        }
    }
    else
    {
        if (act == BUY && !sellOrder.empty())
        {
            while (quantity > 0)
            {
                if (sellOrder.empty())
                {
                    ++curID;
                    return;
                }

                Order* seller = nullptr;
                for(auto it : sellOrder)
                {
                    if (it->stockName == stockName)
                    {
                        seller = it;
                        break;
                    }
                }

                if (seller == nullptr || seller->price > price)
                {
                    ++curID;
                    return;
                }
                else
                {
                    if (seller->id < curID)
                        dealPrice = seller->price;
                    else
                        dealPrice = price;

                    if (seller->quantity >= quantity)
                    {
                        seller->quantity -= quantity;
                        endOfTrade(dealPrice, quantity, clientName,
                                seller->clientName, stockName, false);

                        if (seller->quantity == 0)
                            sellOrder.erase(seller);

                        ++curID;
                        return;
                    }
                    else
                    {
                        quantity -= seller->quantity;
                        endOfTrade(dealPrice, seller->quantity, clientName,
                                seller->clientName, stockName, false);
                        sellOrder.erase(seller);
                    }
                }
            }
        }
        else if (act == SELL && !buyOrder.empty())
        {
            while (quantity > 0)
            {
                if (buyOrder.empty())
                {
                    ++curID;
                    return;
                }

                Order* buyer = nullptr;
                for(auto it : buyOrder)
                {
                    if (it->stockName == stockName)
                    {
                        buyer = it;
                        break;
                    }
                }

                if (buyer == nullptr || buyer->price < price)
                {
                    ++curID;
                    return;
                }
                else
                {
                    if (buyer->id < curID)
                        dealPrice = buyer->price;
                    else
                        dealPrice = price;

                    if (buyer->quantity >= quantity)
                    {
                        buyer->quantity -= quantity;
                        endOfTrade(dealPrice, quantity, clientName,
                                buyer->clientName, stockName, true);

                        if (buyer->quantity == 0)
                            buyOrder.erase(buyer);

                        ++curID;
                        return;
                    }
                    else
                    {
                        quantity -= buyer->quantity;
                        endOfTrade(dealPrice, buyer->quantity, clientName,
                                buyer->clientName, stockName, true);
                        buyOrder.erase(buyer);
                    }
                }
            }
        }
    }

    ++curID;
}

void Market::printEndOfDay()
{
    if(median)
        printMedian();
    if(midpoint)
        printMidpoint();


    cout << "---End of Day---" << "\n";
    cout << "Commission Earnings: $" << commission << "\n";
    cout << "Total Amount of Money Transferred: $" << moneyTransferd << "\n";
    cout << "Number of Completed Trades: " << tradeNum << "\n";
    cout << "Number of Shares Traded: " << tradedShare << endl;

    if (transfers)
    {
        for (auto it = clientMap.begin(); it != clientMap.end(); ++it) //TODO: change to for each loop
            (*it).second->printTransfers();
    }
    for (auto it = timeTraveler.begin(); it != timeTraveler.end(); ++it)
    {
        stockMap.find((*it))->second->printTimeTraveler();
    }

    //TODO
}



/*
 *
 *
 *
 * private functions
 *
 *
 */

void Market::printMedian()
{
    for (auto it = stockMap.begin(); it != stockMap.end(); ++it)
    {
        (*it).second->printMedian(curTimeStamp);
    }
}

void Market::printMidpoint()
{
    for (auto it = buyOrder.begin(); it != buyOrder.end() ; ++it)
    {
        auto & temp = midHelper.find((*it)->stockName)->second;
        if ((size_t)temp->buyTimeStamp != curTimeStamp)
        {
            temp->buyTimeStamp = (int)curTimeStamp;
            temp->highest = (*it)->price;
        }
    }


    for (auto it = sellOrder.begin(); it != sellOrder.end() ; ++it)
    {
        auto & temp = midHelper.find((*it)->stockName)->second;
        if ((size_t)temp->sellTimeStamp != curTimeStamp)
        {
            temp->sellTimeStamp = (int)curTimeStamp;
            temp->lowest = (*it)->price;
        }
    }

    for (auto it = midHelper.begin(); it != midHelper.end() ; ++it)
    {
        auto & temp = (*it).second;

        if ((size_t)temp->buyTimeStamp != curTimeStamp || (size_t)temp->sellTimeStamp != curTimeStamp)
        {
            cout << "Midpoint of " << (*it).first << " at time " << curTimeStamp
                 << " is undefined" << endl;
        }
        else
        {
            cout << "Midpoint of " << (*it).first << " at time " << curTimeStamp
                 << " is $" << ((temp->lowest + temp->highest) / 2) << endl;
        }
    }
}

void Market::removeExpired()
{
    for(auto it = buyOrder.begin(); it != buyOrder.end();)
    {
        if((*it)->expire == -1)
            ++it;
        else if ((int)curTimeStamp >= (*it)->expire)
            buyOrder.erase(it++); // set is implemented by red-black tree, so delete an item will not affect others
        else
            ++it;
    }

    for(auto it = sellOrder.begin(); it != sellOrder.end();)
    {
        if((*it)->expire == -1)
            ++it;
        else if ((int)curTimeStamp >= (*it)->expire)
            sellOrder.erase(it++); // set is implemented by red-black tree, so delete an item will not affect others
        else
            ++it;
    }

}

void Market::endOfTrade(size_t dealPrice, size_t quantity, const string& buyer, const string& seller,
        const string& stockName, bool isSell)
{
    auto result = dealPrice * quantity / 100;
    commission += 2 * result;
    moneyTransferd += dealPrice * quantity;
    ++tradeNum;
    tradedShare += quantity;

    if (verbose)
    {
        cout << buyer << " purchased " << quantity << " shares of " << stockName
        << " from " << seller << " for $" << dealPrice << "/share" << endl;
    }

    auto stockPtr = stockMap.find(stockName);

    if (stockPtr == stockMap.end())
    {
        string tempStock(stockName);
        auto stock = new Stock(tempStock);
        stockMap.emplace(std::move(tempStock), stock);
        stock->addDeal(curTimeStamp, dealPrice, quantity, isSell);
    }
    else
        ((*stockPtr).second)->addDeal(curTimeStamp, dealPrice, quantity, isSell);


    auto buyerPtr = clientMap.find(buyer);

    if (buyerPtr == clientMap.end())
    {
        string tempBuyer(buyer);
        auto buyClient = new Client(tempBuyer);
        clientMap.emplace(std::move(tempBuyer), buyClient);
        buyClient->update(false, dealPrice, quantity);
    }
    else
        ((*buyerPtr).second)->update(false, dealPrice, quantity);


    auto sellerPtr = clientMap.find(seller);

    if (sellerPtr == clientMap.end())
    {
        string tempSeller(seller);
        auto sellClient = new Client(tempSeller);
        clientMap.emplace(std::move(tempSeller), sellClient);
        sellClient->update(true, dealPrice, quantity);
    }
    else
        ((*sellerPtr).second)->update(true, dealPrice, quantity);

//    cout << commission << endl;
}







