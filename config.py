import configparser
import json


class Settings:
    config = configparser.ConfigParser()
    config.read('files/config.ini')
    tinkoff_token = config.get('Tokens', 'tinkoff')
    bot_token = config.get('Tokens', 'bot')
    loaded_list_id = json.loads(config.get("Settings", "users"))
    users_id = list(map(int, loaded_list_id))
    proportions = json.loads(config.get("Proportions", "test_proportions"))
    ids_proportions_dict = dict(zip(loaded_list_id, proportions))
