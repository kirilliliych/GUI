CC = g++
CXXFLAGS = -std=c++20
DLFLAGS = -Wl,-export-dynamic
SFMLFLAGS = -lsfml-window -lsfml-system -lsfml-graphics

SRCDIRS = . events events/signals geometry inbuilttools plugins raytracer sfmlwrap toolsplugins utilities widgets widgets/painteditor
BINDIR = bin

VPATH += $(SRCDIRS)

APPLICATION = WindowManager

SRC = $(wildcard $(addsuffix /*.cpp, $(SRCDIRS)))
OBJ = $(addprefix $(BINDIR)/, $(notdir $(SRC:.cpp=.o)))
# OBJ = $(addprefix $(BINDIR)/, $(patsubst %.cpp, %.o, $(notdir $(SRC))))

all: preparebuild $(APPLICATION)

$(APPLICATION): $(OBJ) 
	@$(CC) $(DLFLAGS) $^ -o $@ $(CXXFLAGS) $(SFMLFLAGS)

$(BINDIR)/%.o: %.cpp
	@$(CC) $< -c -MD -o $@ $(CXXFLAGS) $(SFMLFLAGS)

preparebuild:
	@mkdir -p $(BINDIR)

.PHONY: clean run info gdb valgrind

clean:
	@rm -rf $(BINDIR)

run:
	@./$(APPLICATION)

info:
	@echo [OBJ]: $(OBJ)
	@echo
	@echo [SRC]: $(SRC)

gdb:
	@gdb $(APPLICATION)

valgrind:
	@valgrind --leak-check=full $(APPLICATION)
