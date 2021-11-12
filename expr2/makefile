cc=g++
exe=parser
obj=parser.o table_printer.o

$(exe):$(obj)
	$(cc) -o $(exe) $(obj)
parser.o:parser.cpp
	$(cc) -c parser.cpp
table_printer.o:table_printer.cpp
	$(cc) -c table_printer.cpp
clean:
	erase .\*.o