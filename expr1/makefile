cc=g++
exe=scanner
obj=scanner.o table_printer.o

$(exe):$(obj)
	$(cc) -o $(exe) $(obj)
scanner.o:scanner.cpp
	$(cc) -c scanner.cpp
table_printer.o:table_printer.cpp
	$(cc) -c table_printer.cpp
clean:
	erase .\*.o