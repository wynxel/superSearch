# superSearch
Compile main:
=============
    g++ -std=c++17 -Wall -pthread main.cpp file_iterate.cpp file_read.cpp string_search.cpp -o superSearch
    or make

Compile single thread test version with:
========================================
    g++ -std=c++17 -Wall single_thread.cpp -o single
    or make single
    
Required:
=========
.. boost library (used only in one string operation...)
.. c++ 17 support
