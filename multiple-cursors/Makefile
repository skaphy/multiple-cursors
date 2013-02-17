include ../Makefile.common
SOURCES=multiple-cursors.cpp cursormanager.cpp dummycursor.cpp MainWindow.cpp preference.cpp
TARGET=multiple-cursors.exe
CFLAGS=-D_WIN32_WINNT=0x0600 -D_WIN32_IE=0x0300
LDFLAGS=-static
LIBS=

default: $(TARGET)

clean:
	$(RM) resource.o
	$(RM) $(TARGET)

$(TARGET): $(SOURCES) resource.rc
	$(WINDRES) -o resource.o resource.rc
	$(CXX) -mwindows $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCES) resource.o $(LIBS)

