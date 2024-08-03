import os

from aiogram import F
from aiogram import Router
from aiogram import types
from aiogram.filters.state import State, StatesGroup
from aiogram.fsm.context import FSMContext
from aiogram.types import Message, FSInputFile

from functions import read_csv_table_and_draw_picture
import kb


router = Router()


args_for_graphic = [0, 0]
ERROR_STRING = "Введенный вами период некорректен или данные по одной из крайних дат отсутствуют. Попробуйте другой период"


class SMessage(StatesGroup):
    period = State()
    draw = State()


@router.message(F.text.lower() == "график 📈")
async def graphic(message: Message, state: FSMContext):
    await state.set_state(SMessage.period)
    await message.answer("Введите период по шаблону:\ndd.mm hour:minute\ndd.mm hour:minute\nНапример:\n24.07 10:00\n27.07 19:00")


@router.message(SMessage.period)
async def process_period(message: Message, state: FSMContext):
    await state.update_data(name=message.text)
    await message.answer(f"Вы ввели:{message.text}\n\nВыберите шаг",
                         reply_markup=kb.graphic_keyboard.as_markup())
    args_for_graphic[0] = message.text


@router.callback_query(F.data == "hour")
async def set_step_hour(callback: types.CallbackQuery):
    args_for_graphic[1] = 1
    read_csv_table_and_draw_picture('unstandart', args_for_graphic[0], args_for_graphic[1])
    graphic = FSInputFile("temp/Picture.png")
    await callback.bot.send_photo(chat_id=callback.message.chat.id,
                                  photo=graphic)
    os.remove("temp/Picture.png")


@router.callback_query(F.data == "day")
async def set_step_day(callback: types.CallbackQuery):
    args_for_graphic[1] = 24
    read_csv_table_and_draw_picture('unstandart', args_for_graphic[0], args_for_graphic[1])
    graphic = FSInputFile("temp/Picture.png")
    await callback.bot.send_photo(chat_id=callback.message.chat.id,
                                  photo=graphic)
    os.remove("temp/Picture.png")


@router.callback_query(F.data == "week")
async def set_step_week(callback: types.CallbackQuery):
    args_for_graphic[1] = 24*7
    read_csv_table_and_draw_picture('unstandart', args_for_graphic[0], args_for_graphic[1])
    graphic = FSInputFile("temp/Picture.png")
    await callback.bot.send_photo(chat_id=callback.message.chat.id,
                                  photo=graphic)
    os.remove("temp/Picture.png")
