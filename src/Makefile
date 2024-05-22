CPPFLAGS = -I../include -DEBUG
LDFLAGS = 
EXE = mhe

all : mhe 

rebuild : clean mhe

debug: main.o
	$(CC) $(CFLAGS) -g3 $(CPPFLAGS) -o $(EXE) $^ $(LDFLAGS)

mhe : main.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o  $@ $^ $(LDFLAGS)

main.o : main.c ../include/main.h 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

clean : 
	@rm -rf *.o $(EXE)

help : 
	echo "Usage : "
	@echo "  make [all]\t\tBuild the software"
	@echo "  make clean\t\tRemove all files and outdated software"
	@echo "  make help\t\tDisplay this help"
	
.PHONY : all clean help