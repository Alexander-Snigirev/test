# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2

# Целевой исполняемый файл
TARGET = game

# Список исходных файлов
SRC = main.cpp \
      GameField.cpp \
      Ship.cpp \
      ShipManager.cpp\
	  skills/DoubleDamage.cpp\
	  skills/Scanner.cpp\
	  skills/Bombardment.cpp\
	  skills/AbilityManager.cpp\
	  Game.cpp\
	  in_out/Input.cpp\
	  GameState.cpp\
	  control/InputHandler.cpp\
	  control/GameRenderer.cpp\
	  control/GameController.cpp\
	  control/FieldRenderer.cpp\
	  control/AttackCommand.cpp\
	  control/AbilityCommand.cpp\
	  control/EndCommand.cpp\
	  control/SaveCommand.cpp\
	  control/LoadCommand.cpp\



# Список объектных файлов (меняем расширение .cpp на .o)
OBJ = $(SRC:.cpp=.o)

# Правило по умолчанию — сборка исполняемого файла
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Правило для компиляции .cpp файлов в .o файлы
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка объектных файлов и исполняемого
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
