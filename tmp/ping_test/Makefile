ifeq ($(TYPE), RELEASE)
	FLAGS=-O2 -DNDEBUG
else
	FLAGS=-g -O0
endif

ping_server: ping_server.cc ping_server.h common.h constant.h
	g++ -o ping_server $(FLAGS) -Wall -Werror ping_server.cc -std=c++11 -luv

clean:
	rm -rf ping_server *.o
