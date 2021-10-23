/**
 * This command is going to be similar to the stock
 * command, except for cryptocurrencies. The commander
 * will only be able to look up crypto info by symbol
**/

#ifndef CRYPTO_H
#define CRYPTO_H
#include <iostream>
#include <system_error>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

/**
 * Gets the most recent crypto prices from CoinMarketCap.com
**/
void getCryptoPrices();

/**
 * Parses the searched cryptos into an iterable vector
 * 
 * Pre: search is not null
 * 
 * @param search The cryptos to search for
 * @returns A vector of all the cryptos to look up
**/
vector<string> parseCryptos(string search);

/**
 * Creates the names vector from the JSON of the data
 * 
 * @param fullCrypto A JSON of the data from CoinMarketCap.com
 * @returns A vector of all of the names of the coins
**/
vector<string> createNamesVector(json fullCrypto);

/**
 * Creates the symbols vector from the JSON of the data
 * 
 * @param fullCrypto A JSON of the data from CoinMarketCap.com
 * @returns A vector of all of the symbols of the coins
**/
vector<string> createSymbolsVector(json fullCrypto);

/**
 * Determines if the searched for string is in the array
 * 
 * @param vec The vector to search
 * @param search The string to search for
 * @returns The position in the array of the crypto
**/
int isIn(vector<string> vec, string search);

/**
 * The output string of the price of the cryptos being searched for
 * 
 * @param cryptos A vector of all the cryptos to search for
 * @param names A string array of all the crypto names in lowercase
 * @param symbols A string array of all the crypto symbols in lowercase
 * @param data A JSON of all the data from CoinMarketCap.com
 * @returns A formatted string of the info for each crypto
**/
string output(vector<string> cryptos, vector<string> names, vector<string> symbols, json data);

#endif