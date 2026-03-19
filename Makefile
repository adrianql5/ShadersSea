CXX ?= g++
TARGET := shaderssea

SRC := \
	src/main.cpp \
	src/Objeto.cpp \
	src/camara.cpp \
	src/cruceta.cpp \
	src/dibujo.cpp \
	src/skybox.cpp

OBJ := $(SRC:.cpp=.o)

CPPFLAGS += -Isrc $(shell pkg-config --cflags glfw3 2>/dev/null)
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
LDLIBS += $(shell pkg-config --libs glfw3 2>/dev/null) -lGL -ldl -lm -lpthread

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

src/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
