from modules.ip_hash_table import HashTableIp, is_ip


HELP = 'Добро пожаловать в программу для работы с IP адресами и доменными именами!\nПравила ввода:\n\nIP адреса имеют формат ipv4: "num.num.num.num", где 0<=num<=255\nДоменные имена domain - строки\n\nДоступные команды:\
\n\n/help - вывод мануала\n/add ip domain - добавление пары ip-domain\n/delete ip|domain - удаление пары по ключу\n/search ip|domain - поиск domain|ip по ключу и вывод на экран\n/print - вывод всех пар domain-ip\n/exit - выход из программы\n/save \"filename\" - сохранение в файл'


def start():
    tbl = HashTableIp()
    print(''.join(HELP))    
    
    while True:
        command = input()
        if command == "/exit":
            print("Exit..")
            break
        elif command == "/help":
            print(HELP)
        elif command=='/print':
            tbl.print()    
        else:
            operands = list(command.split())
            if operands[0]=='/add':
                if is_ip(operands[1])==False:
                    print("Ircorrect IP")
                else:
                    tbl.add(operands[1], operands[2])
            elif operands[0]=='/delete':
                tbl.delete_pair(operands[1])
            elif operands[0]=='/search':
                print(tbl.search(operands[1]))      
            else:
                print("Error input")    
               


