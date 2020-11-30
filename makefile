all: superSearch

superSearch: main.cpp file_iterate.cpp file_read.cpp string_search.cpp
	g++ -std=c++17 -Wall -pthread main.cpp file_iterate.cpp file_read.cpp string_search.cpp -o superSearch

single: single_thread.cpp
	g++ -std=c++17 -Wall single_thread.cpp -o single