CFLAGS = -std=c11 -Wall -Wextra -pedantic -O2 -ggdb3
CPPFLAGS = -I../include -DEBUG
LDFLAGS += -L/usr/lib/x86_64-linux-gnu -lgmp 
EXE = mhe

all : $(EXE) 

rebuild : clean all

debug : CFLAGS += -g3
debug: $(EXE)

$(EXE) : main.o gram-schmidt.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o  $@ $^ $(LDFLAGS)

main.o : main.c ../include/main.h 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

gram-schmidt.o : gram-schmidt.c ../include/gram-schmidt.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean : 
	@rm -rf *.o $(EXE)

help : 
	echo "Usage : "
	@echo "  make [all]\t\tBuild the software"
	@echo "  make clean\t\tRemove all files and outdated software"
	@echo "  make help\t\tDisplay this help"
	
.PHONY : all clean help rebuild
