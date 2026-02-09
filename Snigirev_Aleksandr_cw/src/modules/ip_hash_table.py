from .config import config
from .ip_tree import Tree_ip
from .domain_tree import Tree_domain


def int_to_ip(string)->str:
    num = int(string)
    return '.'.join(str((num >> (8 * i)) & 0xFF) for i in range(3, -1, -1))

def ip_to_int(ip: str) -> int:
    parts = map(int, ip.split('.'))
    result = 0
    for part in parts:
        result = (result << 8) + part
    return result

def ip_hash(ip: str, table_size: int) -> int:
    ip_int = ip_to_int(ip)
    return ip_int % table_size

def fnv1a_hash(domain: str, table_size: int) -> int:
    fnv_prime = 16777619
    hash_value = 2166136261  # Начальное значение (FNV offset basis)

    for char in domain:
        hash_value ^= ord(char)  # XOR текущего символа
        hash_value *= fnv_prime  # Умножение на фиксированное простое число
        hash_value &= 0xffffffff  # Ограничение до 32 бит (аналог переполнения)

    return hash_value % table_size


def is_ip(string: str):
    l = len(string)
    result = (string.count('.')==3 and l<16 and l>6)
    return result
    


class HashTableIp:
    def __init__(self):
        self.table_ip = [Tree_ip() for _ in range(config.table_size)]
        self.table_domain = [Tree_domain() for _ in range(config.table_size)]
        self.max_len = config.table_size
    def add(self, key:str, value:str):
        ip_hsh = ip_hash(key,config.table_size)
        domain_hash = fnv1a_hash(value, config.table_size)
        key_lst = list(map(int, key.split('.')))
        try:
            self.table_ip[ip_hsh].insert_value(val=value,key=key_lst)
            self.table_domain[domain_hash].insert_value(key=value, val=key)
        except ValueError:
            print("Пары должны быть уникальными")       

    def delete_ip(self, key:str, flag=True):
        key_hash = ip_hash(key,config.table_size)
        key_lst = list(map(int, key.split('.')))
        domain = self.table_ip[key_hash].search(key_lst)
        self.table_ip[key_hash].delete_value(key_lst)
        if flag==True:
            self.delete_domain(domain)

    def delete_domain(self, domain, flag = True):
        domain_hash = fnv1a_hash(domain, config.table_size)
        ip = self.table_domain[domain_hash].search(domain)
        self.table_domain[domain_hash].delete_value(domain)
        if flag == True:
            self.delete_ip(ip, False)

    def delete_pair(self, key:str):
        try:
            if is_ip(key):
                self.delete_ip(key)
            else:
                self.delete_domain(key)
        except AttributeError:
            print("Error input")                    

    def search(self, key: str):
        if is_ip(key):
            key_hash = ip_hash(key,config.table_size)
            key = list(map(int, key.split('.')))
            return self.table_ip[key_hash].search(key)
        key_hash = fnv1a_hash(key,config.table_size)
        return self.table_domain[key_hash].search(key)

    def print(self):
        print("Domain Name      IP")
        for tree in self.table_domain:
            print("Elem:")
            tree.print_tree()
        print('\n')    

    def save_in_file(self, bash):
        with open(bash,"w") as file:
            for tree in self.table_domain:
                file.write(tree.to_string())
        print('\n')

    def load_from_file(self, bash):
        with open(bash, "r") as file:
            lines = file.readlines()
        commands = []
        for line in lines:
            comnds = line.split('-')
            commands.append([comnds[1], comnds[0]])
        for arg in commands:
            if is_ip(arg[0]):
                self.add(arg[0], arg[1])
            else:
                self.add(int_to_ip(arg[0]), arg[1])    


  

