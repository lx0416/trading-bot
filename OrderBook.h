#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
public:
    /** construct, reading a csv data file */
    OrderBook(std::string filename);
    /** return vector of all know products in the dataset*/
    std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters*/
    std::vector<OrderBookEntry> getOrders(OrderBookType type,
        std::string product,
        std::string timestamp);

    /** returns the earliest time in the orderbook*/
    std::string getEarliestTime();
    /** returns the previous time given the timestamp input*/
    std::string getPreviousTime(std::string timestamp);
    /** returns the next time after the
     * sent time in the orderbook
     * If there is no next timestamp, wraps around to the start
     * */
    std::string getNextTime(std::string timestamp);
    /** returns the lastest time in the orderbook*/
    std::string getLatestTime();

    void insertOrder(OrderBookEntry& order);

    std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

    /** returns the highest price in the orderbook*/
    static double getHighPrice(std::vector<OrderBookEntry>& orders);
    /** returns the lowest price in the orderbook*/
    static double getLowPrice(std::vector<OrderBookEntry>& orders);
    /** returns the spread between the lowest and highest price in the orderbook*/
    static double getSpread(double max, double min);    

private:
    std::vector<OrderBookEntry> orders;

};
