#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain
{
public:
    MerkelMain();
    /** Call this to start the sim */
    void init();
private:
    void printMenu();
    void printHelp();
    void printMarketStats();
    void enterAsk();
    void enterBid();
    void printWallet();
    void gotoNextTimeframe();    
    int getUserOption();
    void processUserOption(int userOption);
    /** Automates bidding and selling of currencies */
    void BotMode(); //R2: Bidding and buying functionality, R3: Offering and selling
    /** Generates all sales made in each time frame, up til the last time frame, where the user is required to manually bid/ask*/
    void generateSalesFile(); //R4: Logging

    std::string currentTime;    

    OrderBook orderBook{ "20200319.csv" };  
    
    Wallet wallet;

};
