CC=g++

OBJDIR 		= obj
TARGETDIR 	= bin
TARGET 		= $(TARGETDIR)/RazorWire
INCLUDES   += -Iinclude
LDFLAGS    += -Llib/linux -Llib
LIBS 	   += lib/linux/libunittest.a lib/linux/libGWEN-Renderer-SFML.a lib/linux/libgwen_static.a -lGL -lsfml-window -lsfml-graphics -lsfml-system
SOURCES		= main.cpp Player.cpp Object.cpp Track.cpp
CFLAGS=-c -Wall -fno-rtti
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=RazorWire

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -o $(TARGET) $(LDFLAGS) $(OBJECTS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
