import requests
import json
import time
import datetime as dt
import pandas as pd


def find_start(coin_pair):
    first = 1451606400
    last = 1635120000
    isFound = False
    while not isFound:
        midpoint = (last + first) // 2
        s = midpoint
        e = midpoint + 60
        url1 = "https://api.exchange.coinbase.com/products/{}/candles?granularity=60&start={}&end={}".format(coin_pair,
                                                                                                             dt.datetime.utcfromtimestamp(
                                                                                                                 s),
                                                                                                             dt.datetime.utcfromtimestamp(
                                                                                                                 e))
        header = {"Accept": "application/json"}
        r1 = requests.get(url1, headers=header)
        dump = json.dumps(r1.json(), indent=4)
        load = json.loads(dump)
        if first == midpoint or midpoint == last:
            isFound = True
        elif len(load) > 0:
            last = midpoint
        else:
            first = midpoint
        time.sleep(0.1)
    if first % 10 == 0:
        return first
    elif midpoint % 10 == 0:
        return midpoint
    elif last % 10 == 0:
        return last


def sort_csv(filename):
    filepath = "./CSVs/Unsorted/{}.csv".format(filename)
    df = pd.read_csv(filepath)
    sorted_df = df.sort_values(by=['Time'], ascending=True)
    sorted_df.to_csv('./CSVs/Sorted/{}_sorted.csv'.format(filename), index=False)


def create_csv(coin_pair):
    for coin in coin_pair:
        start = find_start(coin)
        end = start + 18000
        step = 1
        url = "https://api.exchange.coinbase.com/products/{}/candles?granularity=60&start={}&end={}".format(coin,
                                                                                                            dt.datetime.utcfromtimestamp(
                                                                                                                start),
                                                                                                            dt.datetime.utcfromtimestamp(
                                                                                                                end))
        headers = {"Accept": "application/json"}
        r = requests.get(url, headers=headers)
        data = r.json()
        df = pd.DataFrame(columns=['Time', 'Low', 'High', 'Open', 'Close', 'Volume'], data=data)
        print("\r{} : {}".format(coin, step), end="")
        step += 1
        start = end + 60
        end = start + 18000
        while end < 1635120000:  # 1451692800: (for 5 steps)
            url = "https://api.exchange.coinbase.com/products/{}/candles?granularity=60&start={}&end={}".format(coin,
                                                                                                                dt.datetime.utcfromtimestamp(
                                                                                                                    start),
                                                                                                                dt.datetime.utcfromtimestamp(
                                                                                                                    end))
            headers = {"Accept": "application/json"}
            try:
                r = requests.get(url, headers=headers)
            except:
                print(requests.get(url, headers=header))
            data = r.json()
            df2 = pd.DataFrame(columns=['Time', 'Low', 'High', 'Open', 'Close', 'Volume'], data=data)
            df = df.append(df2, ignore_index=True)
            start = end + 60
            end = start + 18000
            print("\r{}: {}".format(coin, step), end="")
            step += 1
            time.sleep(0.1)
        df.to_csv("./CSVs/Unsorted/{}_candles.csv".format(coin), index=False)
        sort_csv("{}_candles".format(coin))
        print("\n")
        time.sleep(5)

if __name__ == '__main__':
    f = open("coinbase pairs.json", "w")
    f1 = open("cb data.json", "w")
    url = "https://api.exchange.coinbase.com/products"
    headers = {"Accept": "application/json"}
    response = requests.get(url, headers=headers)

    info = response.json()
    data = json.dumps(info, indent=4)
    f.write(data)
    pairs = json.loads(data)
    usdPairs = []
    usdtPairs = []
    usdcPairs = []
    btcPairs = []
    ethPairs = []

    for pair in pairs:
        if pair['quote_currency'] == 'USD':
            usdPairs.append(pair['id'])
        elif pair['quote_currency'] == 'USDT':
            usdtPairs.append(pair['id'])
        elif pair['quote_currency'] == 'USDC':
            usdcPairs.append(pair['id'])
        elif pair['quote_currency'] == 'BTC':
            btcPairs.append(pair['id'])
        elif pair['quote_currency'] == 'ETH':
            ethPairs.append(pair['id'])

    time.sleep(1)
    # create_csv(usdPairs)
    # create_csv(usdtPairs)
    # create_csv(usdcPairs)
    create_csv(btcPairs)
    create_csv(ethPairs)
    f.close()
    f1.close()

