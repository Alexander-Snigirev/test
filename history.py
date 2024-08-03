import os
import pandas as pd

from aiogram import F
from aiogram import Router
from aiogram import types
from aiogram.filters.state import State, StatesGroup
from aiogram.fsm.context import FSMContext
from aiogram.types import Message, InlineKeyboardButton
from aiogram.utils.keyboard import InlineKeyboardBuilder
from tabulate import tabulate

router = Router()


class SMessage(StatesGroup):
    show_history = State()


counter = 0
current_page = 0


@router.message(F.text.lower() == "история операций 📑")
async def history(message: Message, state: FSMContext):
    global df, current_page, counter
    df = pd.read_csv("temp/history.csv")
    os.remove("temp/history.csv")
    current_page = 0
    counter = 0
    # df.to_csv("files/current.csv", index=False)
    await state.set_state(SMessage.show_history)
    await show_more_strings(message, state)


async def show_more_strings(message: Message, state: FSMContext):
    global df, current_page, counter
    start_index = current_page * 10
    end_index = min(start_index + 10, len(df))
    df_to_show = df.iloc[start_index:end_index]
    heads = df.columns.to_list()

    keyboard = InlineKeyboardBuilder()
    if current_page > 0:
        keyboard.add(InlineKeyboardButton(text="prev", callback_data="prev"))
    if end_index < len(df):
        keyboard.add(InlineKeyboardButton(text="next", callback_data="next"))
    if counter == 0:
        await message.answer("История операций:")
        res_table = tabulate(df_to_show,
                             tablefmt='double_grid',
                             stralign='center',
                             numalign='center',
                             headers=heads,
                             showindex='never')
        await message.answer(f"<pre>{res_table}</pre>",
                             parse_mode="html",
                             reply_markup=keyboard.as_markup())
    else:
        res_table = tabulate(df_to_show,
                             tablefmt='double_grid',
                             stralign='center',
                             numalign='center',
                             headers=heads,
                             showindex='never')
        await message.edit_text(f"<pre>{res_table}</pre>",
                                parse_mode="html",
                                reply_markup=keyboard.as_markup())


@router.callback_query(F.data == "next", SMessage.show_history)
async def next_handler(call: types.CallbackQuery, state: FSMContext):
    global current_page, counter
    current_page += 1
    counter += 1
    await show_more_strings(call.message, state)


@router.callback_query(F.data == "prev", SMessage.show_history)
async def prev_handler(call: types.CallbackQuery, state: FSMContext):
    global current_page, counter
    current_page -= 1
    counter += 1
    await show_more_strings(call.message, state)
