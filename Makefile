#	Makefile
# 	Auteur 	: Mael EOUZAN
#	Email 	: mael.eouzan@gmail.com
#	Date 	: 29/01/2025

#	Base Commands

CC 			= 	g++
RM 			= 	rm -f
ZIP 		= 	zip

#	TODO : Maybe not work on windows ? ima test it later
OS_NAME		= 	$(shell uname -s | tr A-Z a-z)

#	Options
CPPFLAGS 	=	-Wall
CXXFLAGS 	=	none
LDFLAGS		=	-lm -lglfw3
LFLAGS		=	-Llibs/linux

#	Files and Folders
PROGNAME 	= 	MaelGameEngine
VERSION 	= 	1.0
BIN			= 	bin/
HEADERS 	= 	batch.hpp context.hpp engine.hpp event_manager.hpp mesh.hpp object_array.hpp scene.hpp shader.hpp \
				texture_atlas.hpp texture.hpp vertex.hpp
SOURCES 	= 	batch.cpp context.cpp engine.cpp mesh.cpp scene.cpp shader.cpp texture_atlas.cpp texture.cpp
VNR_HEADER	= 	vendor/
VNR_SOURCES	= 	glad/glad.c stb_image/stb_image.c
MAIN_TEST 	= 	main.cpp
_OBJ 		= 	$(SOURCES:.cpp=.o)
OBJ 		= 	$(patsubst %, bin/%,$(_OBJ))
PKG_CONFIG	= 	$(shell pkg-config --cflags glfw3) \
				$(shell pkg-config --static --libs glfw3)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) $(LDFLAGS) -o $(BIN)$(PROGNAME)

bin/%.o: src/%.cpp includes/
	@env PKG_CONFIG_PATH=$(shell pwd)"/libs/linux" && \
	$(CC) $(CPPFLAGS) -Iincludes -I$(VNR_HEADER) $(PKG_CONFIG) $< -c -o $@

clean:
	rm bin/*.o