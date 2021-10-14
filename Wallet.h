#pragma once
#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet
{
public:
	Wallet();
	/** Insert currency to wallet*/
	void insertCurrency(std::string type, double amount);

	/** Remove currency from wallet*/
	bool removeCurrency(std::string type, double amount);

	/** Check if wallet contains this much currency*/
	bool containsCurrency(std::string type, double amount);

	/** Checks if wallet can cope with this ask or bid*/
	bool canFulfillOrder(OrderBookEntry order);

	/** Update contents of wallet, assuming order was made by owner of the wallet*/
	void processSale(OrderBookEntry& sale);

	/** Generate a string representation of the wallet*/
	std::string toString();

private:
	std::map<std::string, double> currencies;
};

