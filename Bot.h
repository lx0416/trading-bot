#pragma once
#include <vector>
#include "Wallet.h"
#include "MerkelMain.h"
#include "CSVReader.h"

//current_time = currentTime;

class Bot
{
public:	
	Bot(); 
	/** generates prediction using linear regression with input parameters of average bids/asks in that time frame*/
	static double linearReg(double a, double b); //R1B: Generate predictions using linear regression 

private:
	std::vector <OrderBookEntry> orders;	
};

