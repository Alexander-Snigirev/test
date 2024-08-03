from aiogram.types import ReplyKeyboardMarkup, KeyboardButton, \
    InlineKeyboardButton
from aiogram.utils.keyboard import InlineKeyboardBuilder


mkb = [
        [KeyboardButton(text="Портфель 💼"),
         KeyboardButton(text="История операций 📑")],
        [KeyboardButton(text="Уведомления 🔔"),
         KeyboardButton(text="График 📈")]
    ]

menu_keyboard = ReplyKeyboardMarkup(
            keyboard=mkb,
            resize_keyboard=True,
            input_field_placeholder="Выберите действие",
        )


graphic_keyboard = InlineKeyboardBuilder()
graphic_keyboard.add(
    InlineKeyboardButton(
        text="Час",
        callback_data="hour"),
    InlineKeyboardButton(
        text="День",
        callback_data="day"),
    InlineKeyboardButton(
        text="Неделя",
        callback_data="week")
    )
