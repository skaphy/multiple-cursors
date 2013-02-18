include Makefile.common

default: all

clean:
	$(MAKE) -C multiple-cursors clean
	$(RM) -f multiple-cursors.exe
	$(MAKE) -C config clean
	$(RM) -f multiple-cursors-config.exe

all:
	$(MAKE) -C multiple-cursors
	$(CP) multiple-cursors/multiple-cursors.exe .
	$(MAKE) -C config
	$(CP) config/multiple-cursors-config.exe .

