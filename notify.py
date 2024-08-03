from aiogram import F
from aiogram import Router
from aiogram import types
from aiogram.filters.command import Command
from aiogram.filters.state import State, StatesGroup
from aiogram.utils.keyboard import InlineKeyboardBuilder

from config import Settings

router = Router()


class SubscribeState(StatesGroup):
    waiting_for_subscribe = State()


subscribed_users = set()


try:
    with open("files/users.csv", "r") as f:
        for line in f:
            subscribed_users.add(int(line.strip()))
        f.close()
except FileNotFoundError:
    pass


def save_users():
    with open("files/users.csv", "w") as f:
        for user_id in subscribed_users:
            f.write(f"{user_id}\n")
        f.close()


@router.message(Command("subscribe"))
async def start_handler(message: types.Message):
    if message.from_user.id not in Settings.users_id:
        await message.reply("У вас нет доступа к функционалу данного бота.")
    else:
        keyboard = InlineKeyboardBuilder()
        keyboard.add(types.InlineKeyboardButton(text="Отписаться",
                                                callback_data="unsubscribe"))
        keyboard.add(types.InlineKeyboardButton(text="Подписаться",
                                                callback_data="subscribe"))
        await message.answer("Хотите подписаться на уведомления?",
                             reply_markup=keyboard.as_markup())


@router.message(F.text.lower() == "уведомления 🔔")
async def start_handler_button(message: types.Message):
    if message.from_user.id not in Settings.users_id:
        await message.reply("У вас нет доступа к функционалу данного бота.")
    else:
        keyboard = InlineKeyboardBuilder()
        keyboard.add(types.InlineKeyboardButton(text="Отписаться",
                                                callback_data="unsubscribe"))
        keyboard.add(types.InlineKeyboardButton(text="Подписаться",
                                                callback_data="subscribe"))
        await message.answer("Хотите подписаться на уведомления?",
                             reply_markup=keyboard.as_markup())


@router.callback_query(F.data == "subscribe")
async def subscribe_handler(callback: types.CallbackQuery):
    user_id = callback.from_user.id
    if user_id in subscribed_users:
        await callback.message.answer("Вы уже подписаны на уведомления.")
    else:
        subscribed_users.add(user_id)
        save_users()
        await callback.message.answer("Вы успешно подписались на уведомления!")


@router.callback_query(F.data == "unsubscribe")
async def unsubscribe_handler(callback: types.CallbackQuery):
    user_id = callback.from_user.id
    if user_id in subscribed_users:
        subscribed_users.remove(user_id)
        save_users()
        await callback.message.answer("Вы успешно отписались от уведомлений.")
    else:
        await callback.message.answer("Вы не подписаны на уведомления.")
