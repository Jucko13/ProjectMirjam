
DEBUG	= 	-O2
CC	= 	g++
INCLUDE	= 	-I'src'
FLAGS   = 	$(DEBUG) $(INCLUDE) -g 
FILES	=	$(wildcard src/*.cpp)
LFILE	=	$(wildcard *.cpp)
OBJ	=	$(subst src/, obj/, $(FILES:.cpp=.o)) $(addprefix obj/, $(LFILE:.cpp=.o))
LIBS	=	-lrt -lpthread -lknx -lboost_system

#  $(wildcard src/*.cpp)
#  $(addprefix obj/,$(notdir $(SOURCE:.cpp=.o)))
#main.cpp IvySox.cpp arduPi.cpp functions.cpp door.cpp
#-lboost_system -lboost_program_options -lboost_thread -lboost_date_time -lknx

all:		HomeAutomation

HomeAutomation:	$(OBJ)
		@echo [Link]
		@$(CC) -o $@ $(OBJ) $(LIBS)

obj/%.o:src/%.cpp		
		@echo [Compile] $<
		@$(CC) -c $(FLAGS) -o $@ $<

obj/%.o:%.cpp		
		@echo [Compile] $<
		@$(CC) -c $(FLAGS) -o $@ $<
		
clean:
		@echo "[Clean]"
		@rm -f $(OBJ) HomeAutomation

install:
		@echo "[Install]"

uninstall:
		@echo "[UnInstall]"
