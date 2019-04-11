all : Sig MyCopy ForkCopy PipeCopy SockCopy MyShell MoreShell DupShell write_to_file
.PHONY : all
Sig: Sig.c
	gcc -o Sig Sig.c
MyCopy: MyCopy.c
	gcc -o MyCopy MyCopy.c
ForkCopy: ForkCopy.c
	gcc -o ForkCopy ForkCopy.c
PipeCopy : PipeCopy.c
	gcc -o PipeCopy PipeCopy.c
SockCopy : SockCopy.c
	gcc -o SockCopy SockCopy.c -lpthread
MyShell : MyShell.c  include/csapp.c 
	gcc -o MyShell MyShell.c include/csapp.c -lpthread
MoreShell: MoreShell.c  include/csapp.c
	gcc -o MoreShell MoreShell.c include/csapp.c -lpthread
DupShell : DupShell.c  include/csapp.c 
	gcc -o DupShell DupShell.c include/csapp.c -lpthread
write_to_file:write_to_file.c
	gcc -o write_to_file write_to_file.c
