import matplotlib.pyplot as plt

def plot_points_and_lines(points, ordered_points):

    # Распаковываем точки
    x_points, y_points = zip(*points)
    x_ordered, y_ordered = zip(*ordered_points)
    
    # Настраиваем график
    plt.figure(figsize=(8, 6))
    
    # Рисуем произвольные точки
    plt.scatter(x_points, y_points, color='blue', label='Точки')
    
    # Рисуем соединенные линии по упорядоченным точкам
    plt.plot(x_ordered, y_ordered, color='red', marker='o', label='Соединенные точки')
    
    # Добавляем подписи
    plt.title("График с точками и соединенными линиями")
    plt.xlabel("X координаты")
    plt.ylabel("Y координаты")
    plt.legend()
    plt.grid()
    plt.savefig("Pict.png")
    
    # Показываем график
    #plt.show()

