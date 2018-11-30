#include <string>
#include <iostream>
#include <fstream>
#include <getopt.h>

#include "Market.h"

using namespace std;


int main(int argc, char* argv[])
{

    ios::sync_with_stdio(false);
    cin.tie(0);

    Market& market = Market::getInstance();

    bool verbose = false, median = false, midpoint = false, transfers = false;

    const option longOpts[] = {
            {"verbose",   no_argument,       nullptr, 'v'},
            {"median",    no_argument,       nullptr, 'm'},
            {"midpoint",  no_argument,       nullptr, 'p'},
            {"transfers", no_argument,       nullptr, 't'},
            {"ttt",       required_argument, nullptr, 'g'},
            {nullptr, 0, nullptr, 0}
    };
    const char* shortOpts = "vmptg:";
    int opt;

    while ((opt = getopt_long(argc, argv, shortOpts, longOpts, nullptr)) != -1)
    {
        switch (opt)
        {
            case 'v':
                verbose = true;
                break;
            case 'm':
                median = true;
                break;
            case 'p':
                midpoint = true;
                break;
            case 't':
                transfers = true;
                break;
            case 'g':
                market.initTimeTraveler(optarg);
                break;
            default:
                break;
        }
    }

    market.initOpts(verbose, median, midpoint, transfers);

    string stockOrder;
    while (!cin.eof())
    {
        getline(cin, stockOrder);
        if (!stockOrder.empty())
            market.parser(stockOrder);
    }
//    int count = 0;
//    while (count != 15)
//    {
//        getline(cin, stockOrder);
//        if (!stockOrder.empty())
//            market.parser(stockOrder);
//        count++;
//    }


    market.printEndOfDay();
    return 0;
}