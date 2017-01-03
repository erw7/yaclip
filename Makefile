win32yank.exe: main.o cclipboard.o ciconv.o cnlconv.o
	g++ -Wall -O2 --std=c++11 -o win32yank.exe main.o cclipboard.o ciconv.o cnlconv.o -liconv

main.o: main.cc
	g++ -c -Wall -O2 --std=c++11 main.cc

cclipboard.o: cclipboard.cc cclipboard.h
	g++ -c -Wall -O2 --std=c++11 cclipboard.cc

ciconv.o: ciconv.cc ciconv.h cmembuf.h cmembuf_private.h
	g++ -c -Wall -O2 --std=c++11 ciconv.cc

cnlconv.o: cnlconv.cc cnlconv.h
	g++ -c -Wall -O2 --std=c++11 cnlconv.cc

.PHONY: clean
clean:
	rm -f win32yank.exe main.o cclipboard.o ciconv.o cnlconv.o
