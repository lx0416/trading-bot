#include "Bot.h"
#include "CSVReader.h"
#include "Wallet.h"
#include "CSVReader.h"
#include "OrderBook.h"
#include "OrderBookEntry.h"
#include "MerkelMain.h"

Bot::Bot() {
	
}

double Bot::linearReg(double av1, double av2) { //based on the concept of y=mx+c
	double gradient = av1 - av2;
	double c = av1 - gradient;
	double prediction = gradient * 3 + c; 
	return prediction;
}