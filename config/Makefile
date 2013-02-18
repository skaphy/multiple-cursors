include ../Makefile.common
SOURCES=config.cpp HotkeyWindow.cpp ../multiple-cursors/preference.cpp
TARGET=multiple-cursors-config.exe
CFLAGS=-D_WIN32_WINNT=0x0600
LDFLAGS=-static
LIBS=

default: $(TARGET)

clean:
	$(RM) $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) -mwindows $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

