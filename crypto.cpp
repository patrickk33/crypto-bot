#include <iostream>
#include <system_error>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "json.hpp"
#include "crypto.h"
using namespace std;
using json = nlohmann::json;

#define NUM_CRYPTOS 200

/**
 * Gets the most recent crypto prices from CoinMarketCap.com
**/
void getCryptoPrices()
{
    ofstream clear;
    clear.open("cryptoPrices.json", ios::out | ios::trunc);
    clear.close();
    string command = "curl -H \"X-CMC_PRO_API_KEY: 6347f641-eddb-4ad6-864d-2ef9879ac0f2\" -H \"Accept: application/json\" -d \"start=1&limit=200&convert=USD\" -G https://pro-api.coinmarketcap.com/v1/cryptocurrency/listings/latest >> cryptoPrices.json";
    system(command.c_str());
}

/**
 * Parses the searched cryptos into an iterable vector
 * 
 * Pre: search is not null
 * 
 * @param search The cryptos to search for
 * @returns A vector of all the cryptos to look up
**/
vector<string> parseCryptos(string search)
{
    vector<string> cryptos;
    transform(search.begin(), search.end(), search.begin(), ::tolower);
    string word = "and";
    if (search.find(word) != string::npos)
    {
        size_t p = -1;
        while ((p = search.find(word)) != string::npos)
        {
            search.replace(p, word.length(), "");
        }
    }

    string word2 = " ";
    if (search.find(word2) != string::npos)
    {
        size_t p = -1;
        while ((p = search.find(word2)) != string::npos)
        {
            search.replace(p, word2.length(), "");
        }
    }

    int start = 0;
    string del = ",";
    int end = search.find(del);
    while (end != -1)
    {
        cryptos.push_back(search.substr(start, end - start));
        start = end + del.size();
        end = search.find(del, start);
    }
    cryptos.push_back(search.substr(start, end - start));
    return (cryptos);
}

/**
 * Creates the names vector from the JSON of the data
 * 
 * @param fullCrypto A JSON of the data from CoinMarketCap.com
 * @returns A vector of all of the names of the coins
**/
vector<string> createNamesVector(json fullCrypto)
{
    vector<string> names;
    for (int i = 0; i < NUM_CRYPTOS; i++)
    {
        string name = fullCrypto["data"][i]["slug"];
        names.push_back(name);
    }
    return names;
}

/**
 * Creates the symbols vector from the JSON of the data
 * 
 * @param fullCrypto A JSON of the data from CoinMarketCap.com
 * @returns A vector of all of the symbols of the coins
**/
vector<string> createSymbolsVector(json fullCrypto)
{
    vector<string> symbols;
    for (int i = 0; i < NUM_CRYPTOS; i++)
    {
        string symbol = fullCrypto["data"][i]["symbol"];
        transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);
        symbols.push_back(symbol);
    }
    return symbols;
}

/**
 * Determines if the searched for string is in the array
 * 
 * @param vec The vector to search
 * @param search The string to search for
 * @returns The position in the array of the crypto
**/
int isIn(vector<string> vec, string search)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i].compare(search) == 0)
        {
            return i;
        }
    }
    return -1;
}

/**
 * The output string of the price of the cryptos being searched for
 * 
 * @param cryptos A vector of all the cryptos to search for
 * @param names A string array of all the crypto names in lowercase
 * @param symbols A string array of all the crypto symbols in lowercase
 * @param data A JSON of all the data from CoinMarketCap.com
 * @returns A formatted string of the info for each crypto
**/
string output(vector<string> cryptos, vector<string> names, vector<string> symbols, json data)
{
    stringstream ss;
    for (int i = 0; i < cryptos.size(); i++)
    {
        try
        {
            int inNames = isIn(names, cryptos[i]);
            int inSymbols = isIn(symbols, cryptos[i]);
            string name, symbol;
            float price, volume, marketCap, pctChng;
            if (inNames >= inSymbols)
            {
                name = data["data"][inNames]["name"];
                symbol = data["data"][inNames]["symbol"];
                price = data["data"][inNames]["quote"]["USD"]["price"];
                volume = data["data"][inNames]["quote"]["USD"]["volume_24h"];
                marketCap = data["data"][inNames]["quote"]["USD"]["market_cap"];
                pctChng = data["data"][inNames]["quote"]["USD"]["percent_change_24h"];
            }
            else
            {
                name = data["data"][inSymbols]["name"];
                symbol = data["data"][inSymbols]["symbol"];
                price = data["data"][inSymbols]["quote"]["USD"]["price"];
                volume = data["data"][inSymbols]["quote"]["USD"]["volume_24h"];
                marketCap = data["data"][inSymbols]["quote"]["USD"]["market_cap"];
                pctChng = data["data"][inSymbols]["quote"]["USD"]["percent_change_24h"];
            }
            ss << name << "|" << symbol << "|";
            ss << fixed << setprecision(4) << price;
            ss << "|" << fixed << setprecision(0) << volume;
            ss << "|" << fixed << setprecision(0) << marketCap;
            ss << "|" << fixed << setprecision(4) << pctChng << endl;
        }
        catch (length_error)
        {
            ss << "|||||" << endl;
        }
    }
    return ss.str();
}

int main()
{
    string input;
    cout << "Cryptos you want to get the info of: ";
    getline(cin, input);
    vector<string> vec = parseCryptos(input);
    getCryptoPrices();
    ifstream cryptoPrices("cryptoPrices.json");
    string s;
    getline(cryptoPrices, s);
    json prices = json::parse(s);
    vector<string> n = createNamesVector(prices);
    vector<string> sym = createSymbolsVector(prices);
    cout << output(vec, n, sym, prices) << endl;
    return 0;
}
