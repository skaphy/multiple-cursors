include Makefile.common

default: all

clean:
	$(MAKE) -C multiple-cursors clean
	$(RM) -f multiple-cursors.exe

all:
	$(MAKE) -C multiple-cursors
	$(CP) multiple-cursors/multiple-cursors.exe .

