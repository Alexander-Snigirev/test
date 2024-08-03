import asyncio
import csv
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sb

from math import ceil
from datetime import datetime, timedelta
from tinkoff.invest import Client

from config import Settings


with Client(Settings.tinkoff_token) as client:
    id = client.users.get_accounts().accounts[0].id


async def get_operations_history():
    with Client(Settings.tinkoff_token) as client:
        r = client.operations.get_operations(
            account_id=id,
            from_=datetime.now() - timedelta(days=365.0),
            to=datetime.now(),
        )
        df = pd.DataFrame([{
            'figi': p.figi,
            'date': f"{p.date.day}.{p.date.month}.{p.date.year}",
            'type': p.type,
            'quantity': str(p.quantity),
            'price': p.payment.units if p.quantity == 0
                    else str(p.price.units + round(p.price.nano/1e9, 1)),
        }for p in r.operations])

        df['type'] = df['type'].replace('Продажа ценных бумаг', 'Продажа')
        df['type'] = df['type'].replace('Покупка ценных бумаг', 'Покупка')
        df['type'] = df['type'].replace('Пополнение брокерского счёта', 'Пополнение')
        df['type'] = df['type'].replace('Покупка ценных бумаг с карты', 'Пополнение')
        df['type'] = df['type'].replace('Экспирация опциона', 'Экспирация')
        df = df[df['type'] != 'Удержание комиссии за операцию']

        list_of_figs = df['figi'].to_list()
        tickers = []
        for i in range(0, len(list_of_figs)):
            if (len(list_of_figs[i]) == 12):
                tickers.append(get_ticker(list_of_figs[i]))
            else:
                tickers.append("    ")

        df.insert(0, 'tickers', tickers)
        df = df.drop('figi', axis=1)
        df.reset_index(drop=True, inplace=True)
        return df


def get_ticker(figi):
    with Client(Settings.tinkoff_token) as client:
        ticker = client.instruments.find_instrument(query=str(figi)).instruments[0].ticker
        return ticker


async def get_value():
    with Client(Settings.tinkoff_token) as client:
        balance = client.operations.get_portfolio(account_id=id).total_amount_portfolio.units
        return balance


async def midnight_balance():
    while True:
        now = datetime.now()
        df = await get_operations_history()
        df.to_csv("temp/history.csv", index=False)
        if now.hour == 0:
            value = await get_value()
            with open("files/midnight_data.txt", "w") as f:
                f.write(str(value))
                f.close()
            print(f"Значение сохранено в файл в полночь: {value}")
        await asyncio.sleep(300)


async def balance_in_csv():
    while True:
        now = datetime.now()
        if now.minute == 0:
            data = await get_value()
            with open("files/data.csv", "a", newline="") as f:
                writer = csv.writer(f)
                writer.writerow([now.strftime("%d.%m %H:00"), data])
                print("Данные записаны успешно")
                f.close()
        await asyncio.sleep(60)


def read_csv_table_and_draw_picture(mode, period="None", step=0):
    graphic_df = pd.read_csv("files/data.csv")
    title = "График динамики баланса за введенный вами период"
    if mode == 'day':
        graphic_df = graphic_df.tail(24)
        title = "График за последние 24 часа"
        filtered_data = graphic_df
        x = 'Дата'
    else:
        per = period.split('\n')
        start_date_input = per[0].split(':')[0]
        end_date_input = per[1].split(':')[0]
        df = pd.read_csv("files/data.csv")
        df['date'] = pd.to_datetime(df['Дата'], format="%d.%m %H:00")
        start_date = pd.to_datetime(start_date_input, format="%d.%m %H")
        end_date = pd.to_datetime(end_date_input, format="%d.%m %H")

        min_date = df['date'].min()
        max_date = df['date'].max()

        if start_date < min_date:
            nearest_start = min_date
        elif start_date > max_date:
            nearest_start = max_date
        else:
            nearest_start = start_date

        if end_date < min_date:
            nearest_end = min_date
        elif end_date > max_date:
            nearest_end = max_date
        else:
            nearest_end = end_date
        filtered_data = df[(df['date'] >= nearest_start) & (df['date'] <= nearest_end)]
        filtered_data = filtered_data.iloc[::step]
        x = 'Дата'

    st = ceil(len(filtered_data)/6)
    plt.figure(facecolor="white")
    sb.set_theme(style='whitegrid')
    sb.lineplot(data=filtered_data, x=x, y='Цена', color="red")
    plt.title(title, fontweight="bold", color='black')
    ticks = filtered_data[x][::st]
    plt.xticks(ticks=ticks, labels=ticks, fontsize=5, rotation=45, color="black")
    plt.xlabel("Дата", fontsize=10, color="black")
    plt.ylabel("Баланс, руб", fontsize=10, color="black")
    plt.yticks(fontsize=10, color="black")
    plt.savefig('temp/Picture.png', bbox_inches='tight', dpi=300)
    plt.close()
