#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "OrderBook.h"
#include "Bot.h"
#include <fstream>

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 1000);
    wallet.insertCurrency("USDT", 1000);
    wallet.insertCurrency("DOGE", 1000);
    wallet.insertCurrency("ETH", 1000);

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an ask " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue   
    std::cout << "6: Continue " << std::endl;
    //7 automate bid/ask bot
    std::cout << "7: Activate Automated Bot Mode" << std::endl;  
    //8 prints out all bids/asks
    std::cout << "8: Print all sales " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
            p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "Spread: " << OrderBook::getSpread(OrderBook::getHighPrice(entries), OrderBook::getLowPrice(entries)) << std::endl;                
    }
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe)) {
                std::cout << "Wallet looks good" << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds" << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }
    }    
}

void MerkelMain::enterBid()
{
    std::cout << "Make a bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe)) {
                std::cout << "Wallet looks good" << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds" << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }    
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << "\n" << std::endl;    
    for (std::string& p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime); //matching engine function
        std::cout << "Sales: " << sales.size() << "\n" << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser") {
                //update wallet
                wallet.processSale(sale);
            }
        }
    }    
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "Current time: " << currentTime << std::endl;
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-8" << std::endl;
    std::getline(std::cin, line);
    try {
        userOption = std::stoi(line);
    }
    catch (const std::exception& e)
    {
        // 
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-8" << std::endl;
    }
    if (userOption == 1)
    {
        printHelp();
    }
    if (userOption == 2)
    {
        printMarketStats();
    }
    if (userOption == 3)
    {
        enterAsk();
    }
    if (userOption == 4)
    {
        enterBid();
    }
    if (userOption == 5)
    {
        printWallet();
    }
    if (userOption == 6)
    {
        gotoNextTimeframe();
    }        
    if (userOption == 7)
    {
        BotMode();
    }
    if (userOption == 8)
    {
        generateSalesFile();
    }
}

void MerkelMain::BotMode() {
    std::cout << "Automated Bot Mode activated" << std::endl;
    bool proceed = true;        
    std::ofstream MySalesFile("mySalesFile.txt"); //creates a file to output all bids/asks
    currentTime = orderBook.getEarliestTime();
    gotoNextTimeframe();    
    MySalesFile << "My Sales: " << std::endl;
    MySalesFile << "Context: All bids/asks made are in the context of being relatively the lowest ask/highest bid" << std::endl;    
    while (proceed) {     
        MySalesFile << "Timeframe: " << currentTime << std::endl;
        for (std::string const& p : orderBook.getKnownProducts()) {                        
            std::string previousTime = orderBook.getPreviousTime(currentTime);
            //creates an orderbook separated into bids and asks in current and previous timeframe
            std::vector <OrderBookEntry> currBidBook = orderBook.getOrders(OrderBookType::bid, p, currentTime);
            std::vector <OrderBookEntry> prevBidBook = orderBook.getOrders(OrderBookType::bid, p, previousTime);            
            std::vector <OrderBookEntry> currAskBook = orderBook.getOrders(OrderBookType::ask, p, currentTime);
            std::vector <OrderBookEntry> prevAskBook = orderBook.getOrders(OrderBookType::ask, p, previousTime);
            
            //creates variables regarding the highest and lowest bids/asks in current and previous timeframe
            double currHighBid = orderBook.getHighPrice(currBidBook);
            double currLowBid = orderBook.getLowPrice(currBidBook);
            double prevHighBid = orderBook.getHighPrice(prevBidBook);
            double prevLowBid = orderBook.getLowPrice(prevBidBook);

            double currHighAsk = orderBook.getHighPrice(currAskBook);
            double currLowAsk = orderBook.getLowPrice(currAskBook);
            double prevHighAsk = orderBook.getHighPrice(prevAskBook);
            double prevLowAsk = orderBook.getLowPrice(prevAskBook);
            
            //creates variables on how much to bid/ask in the current timeframe
            double bidLowAmount = currLowAsk + 0.1; 
            double askHighAmount = currHighBid - 0.1; 
            if (askHighAmount < 0) {
                askHighAmount = -askHighAmount + 0.1;
            }            
            
            //creates variables regarding the average bids/asks in current and previous timeframe, and their respective predictions using the averages
            double currBidAv = (currHighBid + currLowBid) / 2;
            double prevBidAv = (prevHighBid + prevLowBid) / 2;
            double bidPrediction = Bot::linearReg(prevBidAv, currBidAv);

            double currAskAv = (currHighAsk + currLowAsk) / 2;
            double prevAskAv = (prevHighAsk + prevLowAsk) / 2;
            double askPrediction = Bot::linearReg(prevAskAv, currAskAv);

            std::cout << "Product: " << p << std::endl;
            MySalesFile << "Product: " << p << std::endl;
            std::cout << "Bid Prediction: " << bidPrediction << std::endl;
            std::cout << "---------------------------------------" << std::endl;            
            if (bidPrediction > currBidAv) { //user will bid in current timeframe if prediction for next timeframe is higher 
                std::cout << "Good time to bid!" << "\n" << std::endl;                
                    OrderBookEntry obe = CSVReader::stringsToOBE(
                        std::to_string(bidLowAmount),
                        "1",
                        currentTime,
                        p,
                        OrderBookType::bid
                    );
                    obe.username = "simuser";

                    if (wallet.canFulfillOrder(obe)) {                                                                       
                        orderBook.insertOrder(obe); 
                        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime); //matching engine function                    
                        std::cout << "my bid: " << bidLowAmount << std::endl;
                        for (OrderBookEntry& sale : sales) {
                            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
                            MySalesFile << "My bid: " << std::endl << "Sale price " << sale.price << " for " << sale.amount << std::endl;
                            //update wallet
                            wallet.processSale(sale);
                            MySalesFile << "My assets: " << std::endl << wallet.toString() << std::endl;
                        }
                    }
                    else {
                        std::cout << "Wallet has insufficient funds" << std::endl;
                    }
                }            
            else {
                std::cout << "Bad time to bid!" << "\n" << std::endl;
                MySalesFile << "No bids made in this timeframe for this product" << "\n" << std::endl;
            }

            std::cout << "Product: " << p << std::endl;
            MySalesFile << "Product: " << p << std::endl;
            std::cout << "Ask Prediction: " << askPrediction << std::endl;
            std::cout << "---------------------------------------" << std::endl;
            if (askPrediction < currAskAv) { //user will ask in current timeframe if prediction for next timeframe is lower
                std::cout << "Good time to ask!" << "\n" << std::endl;                
                    OrderBookEntry obe = CSVReader::stringsToOBE(
                        std::to_string(askHighAmount),
                        "1",
                        currentTime,
                        p,
                        OrderBookType::ask
                    );
                    obe.username = "simuser";

                    if (wallet.canFulfillOrder(obe)) {                        
                        orderBook.insertOrder(obe); 
                        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime); //matching engine function                    
                        std::cout << "my ask: " << askHighAmount << std::endl;
                        for (OrderBookEntry& sale : sales) {
                            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;               
                            MySalesFile << "My ask: " << std::endl << "Sale price " << sale.price << " for " << sale.amount << std::endl;
                            //update wallet
                            wallet.processSale(sale);
                            MySalesFile << "My assets: " << std::endl << wallet.toString() << std::endl;
                        }
                    }
                    else {
                        std::cout << "Wallet has insufficient funds" << std::endl;
                    }
                }            
            else {
                std::cout << "Bad time to ask!" << "\n" << std::endl;
                MySalesFile << "No asks made in this timeframe for this product" << "\n" << std::endl;
            }
       }               
        gotoNextTimeframe();        
        if (currentTime == orderBook.getLatestTime()) { //allows user to bid/ask manually in last timeframe            
            break;
            getUserOption();
        }
    }    
    MySalesFile.close();
}

void MerkelMain::generateSalesFile() { //prints out all bids/asks made
    std::string myText;
    std::ifstream readFile("mySalesFile.txt");

    while (std::getline(readFile, myText)) {
        std::cout << myText << std::endl;
    }
    readFile.close();
}