import asyncio
import logging
import os
import uvicorn
import json
import signal

from aiogram import Bot, Dispatcher
from aiogram import F
from aiogram.filters.command import Command
from aiogram.types import Message, FSInputFile
from fastapi import FastAPI
from fastapi.responses import ORJSONResponse
from multiprocessing import Process
from pydantic import BaseModel, ValidationError
from tinkoff.invest import Client


from config import Settings
from functions import midnight_balance, balance_in_csv
from functions import read_csv_table_and_draw_picture
import history
import kb
import graphics
import notify


logging.basicConfig(level=logging.INFO)


bot = Bot(Settings.bot_token)
dp = Dispatcher()
app = FastAPI()


class Notify(BaseModel):
    notify_text: str


async def cmd_send_notify(text: str):
    user_ids = set()
    with open("files/users.csv", "r") as f:
        for line in f:
            user_ids.add(int(line.strip()))
        f.close()
    for user_id in user_ids:
        try:
            await bot.send_message(user_id, text)
        except Exception as e:
            print(f"Не удалось отправить сообщение пользователю {user_id}: {e}")


@app.post("/notify", response_class=ORJSONResponse)
async def Notifications(data: Notify) -> ORJSONResponse:
    text = data.notify_text
    await cmd_send_notify(text)
    return ORJSONResponse({"detail": "ok"})


@dp.message(Command("start"))
async def cmd_start(message: Message):
    if message.from_user.id not in Settings.users_id:
        await message.reply("У вас нет доступа к функционалу данного бота.")
    else:
        await message.answer("Выберите действие", reply_markup=kb.menu_keyboard)


@dp.message(Command("help"))
async def cmd_help(message: Message):
    await message.answer("Команды:\n/start - начало\n/subscribe - действия с подпиской")


@dp.message(F.text.lower() == "портфель 💼")
async def balance(message: Message):
    with Client(Settings.tinkoff_token) as client:
        id = client.users.get_accounts().accounts[0].id
        balance = client.operations.get_portfolio(account_id=id).total_amount_portfolio.units
        user_balance = round(balance * Settings.ids_proportions_dict.get(str(message.from_user.id)), 2)
        try:
            f = open('files/midnight_data.txt', 'r')
            advance = round(user_balance - int(f.read()) * Settings.ids_proportions_dict.get(str(message.from_user.id)),2)
            f.close()
        except FileNotFoundError:
            advance = "Expect midnight"
        read_csv_table_and_draw_picture('day')
        graphic = FSInputFile("temp/Picture.png")
        await bot.send_photo(chat_id=message.chat.id, photo=graphic)
        await message.answer(f"Ваша доля: {str(user_balance)} rub\nПрирост за день:{str(advance)}rub")
        os.remove("temp/Picture.png")



async def main_bot():
    dp.include_routers(notify.router, history.router, graphics.router)
    await dp.start_polling(bot)
    await bot.delete_webhook(drop_pending_updates=True)


async def run_fastapi():
    config = uvicorn.Config(app, host="0.0.0.0", port=8089)
    server = uvicorn.Server(config)
    await server.serve()


async def main():
    '''task_history = asyncio.create_task(midnight_balance())
    task_for_graphics = asyncio.create_task(balance_in_csv())
    task_api = asyncio.create_task(run_fastapi())
    task_bot = asyncio.create_task(main_bot())
    await asyncio.gather(task_history, task_for_graphics, task_api, task_bot)'''
    await main_bot()


if __name__ == '__main__':
    '''p_fastapi = Process(target=uvicorn.run, args=("app:app", "--host", "0.0.0.0", "--port", 8089))'''
    p_history = Process(target=asyncio.run, args=(midnight_balance(),))
    p_graphics = Process(target=asyncio.run, args=(balance_in_csv(),))
    p_history.start()
    p_graphics.start()

    # Запустить бота
    loop = asyncio.get_event_loop()
    loop.add_signal_handler(signal.SIGINT, lambda: loop.stop())
    loop.run_until_complete(main())

    # Ожидать завершения других процессов
    p_history.join()
    p_graphics.join()
