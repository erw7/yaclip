yaclip.exe: main.o clipboard.o iconv_wrapper.o
	g++ -Wall -O2 --std=c++11 -o yaclip.exe main.o clipboard.o iconv_wrapper.o -liconv -lboost_iostreams-mt

main.o: main.cc yaclip.h clipboard.h
	g++ -c -Wall -O2 --std=c++11 main.cc

clipboard.o: clipboard.cc clipboard.h iconv_wrapper.h
	g++ -c -Wall -O2 --std=c++11 clipboard.cc

iconv_wrapper.o: iconv_wrapper.cc iconv_wrapper.h
	g++ -c -Wall -O2 --std=c++11 iconv_wrapper.cc

.PHONY: clean
clean:
	rm -f yaclip.exe main.o clipboard.o iconv_wrapper.o
