<h2>COPY</h2>
<h2>Testovanie:</h2>
Program superSearch (v main.cpp) som testoval na 
malom adresárovom strome testData (je súčasťou repozitáru, 
55 txt súborov (anglický text + html tagy), každý ~ 1.1-1.5MB, hĺbka adresárového stromu 3). 
Testoval som ho aj na tom istom adresárovom strome, po pridaní dvoch 
väčších súborom (400+MB a 100+MB). 
Program som spúšťal na native Ubuntu 16.04 s i7 4700MQ a ssd diskom.
.
<h2>Zopár výsledkov:</h2>
<h3>Testy na testData/ adresari:</h3>
*(predvolene buffre su read buffer: 6MB, segment buffer: 65536B)*

time ./superSearch ~/testData/ "and is" -v  
**real	0m0.635s**  
user	0m0.604s  
sys	0m0.028s  

time ./single ~/testData/ "and is" -v  
**real	0m2.568s**  
user	0m2.496s  
sys	0m0.060s  

time ./superSearch ~/testData/ "and is" -v -t1 2  
**real	0m0.353s**  
user	0m0.660s  
sys	0m0.032s  

time ./superSearch ~/testData/ "and is" -v -t1 3  
**real	0m0.263s**  
user	0m0.696s  
sys	0m0.060s  

time ./superSearch ~/testData/ "and is" -v -t1 2 -t2 2  
**real	0m0.226s**  
user	0m0.740s  
sys	0m0.064  

time ./superSearch ~/testData/ "and is" -v -t1 2 -t2 4  
**real	0m0.158s**  
user	0m0.852s  
sys	0m0.036s  

<h3>Skúška zmena buffru:</h3>

time ./superSearch ~/testData/ "and is" -v -t1 3 -t2 3 -rb 2097152  
**real	0m0.158s**  
user	0m0.924s  
sys	0m0.068s  

time ./superSearch ~/testData/ "and is" -v -t1 4 -t2 4 -rb 1048576 -sb 262144  
**real	0m0.158s**  
user	0m0.860s  
sys	0m0.060s  

<h3>Testy na testData/ adresari po pridani jedneho 400+MB suboru a jedneho 100+MB suboru:</h3>
*(predvolene buffre su read buffer: 6MB, segment buffer: 65536B)*

time ./superSearch ~/testData/ "and is" -v  
**real	0m4.724s**  
user	0m4.500s  
sys	0m0.196s  

time ./superSearch ~/testData/ "and is" -v -t1 2  
**real	0m3.589s**  
user	0m4.708s  
sys	0m0.152s  

time ./superSearch ~/testData/ "and is" -v -t1 3  
**real	0m3.573s**  
user	0m4.880s  
sys	0m0.220s  

time ./superSearch ~/testData/ "and is" -v -t1 2 -t2 2  
**real	0m2.131s**  
user	0m5.320s  
sys	0m0.256s  

time ./superSearch ~/testData/ "and is" -v -t1 2 -t2 4  
**real	0m1.364s**  
user	0m6.452s  
sys	0m0.208s  

time ./superSearch ~/Desktop/searchNew/superSearch/testData/ "and is" -v -t1 3 -t2 4 -rb 2097152  
**real	0m1.351s**  
user	0m6.376s  
sys	0m0.232s  

time ./superSearch ~/Desktop/searchNew/superSearch/testData/ "and is" -v -t1 3 -t2 4 -rb 2097152 -sb 32768  
**real	0m1.276s**  
user	0m6.080s  
sys	0m0.236s  

time ./superSearch ~/Desktop/searchNew/superSearch/testData/ "and is" -v -t1 3 -t2 10 -rb 2097152 -sb 32768  
**real	0m1.130s**  
user	0m7.316s  
sys	0m0.236s  

time ./superSearch ~/Desktop/searchNew/superSearch/testData/ "and is" -v -t1 3 -t2 10  
**real	0m1.144s**  
user	0m7.444s  
sys	0m0.244s  

<h3> Pozn:</h3>
V repozitári je aj testovací single-thread program (single_thread.cpp).
Je pomalší ako jeho multithredová verzia (main.cpp) v single-threadovom
nastavení (ak main.cpp nedostane žiadne parametre navyše, beží
na jednom vlákne). Nie je mi celkom jasné prečo. 
Vhodným nastavením veľkosti read buffera sa však dá 
dosiahnuť, že single_thread.cpp beží podobne rýchlo,
ako single-threadový main.cpp.
