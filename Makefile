win32yank.exe: main.o clipboard.o ciconv.o ccharbuf.o
	g++ -Wall -O2 --std=c++11 -o win32yank.exe main.o clipboard.o ciconv.o ccharbuf.o -liconv

main.o: main.cc
	g++ -c -Wall -O2 --std=c++11 main.cc

clipboard.o: clipboard.cc clipboard.h
	g++ -c -Wall -O2 --std=c++11 clipboard.cc

ciconv.o: ciconv.cc ciconv.h
	g++ -c -Wall -O2 --std=c++11 ciconv.cc

ccharbuf.o: ccharbuf.cc ccharbuf.h
	g++ -c -Wall -O2 --std=c++11 ccharbuf.cc

.PHONY: clean
clean:
	rm -f win32yank.exe main.o clipboard.o ciconv.o ccharbuf.o
