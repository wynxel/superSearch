Program superSearch (v main.cpp) som testoval na 
malom adresárovom strome testData (je súčasťou repozitáru, 
55 txt súborov, každý ~ 1.1-1.5MB, hĺbka adresárového stromu 3). 
Testoval som to na native Ubuntu 16.04 s i7 4700MQ a ssd diskom.
.


Príklady: 
time ./superSearch ~/Desktop/testData "Union County Medi" -v -rb 1500000
time ./single ~/Desktop/testData "Union County Medi" -v -rb 1500000

time ./single ~/Desktop/testData "jul 270 s" -v
time ./superSearch ~/Desktop/testData "jul 270 s" -v
.
Na testovacích súboroch, ktoré sú aj tu na githube, sa mi pri multi-threadovom
nastavení nepodarilo dosiahnuť značne väčšiu rýchlosť. Ani zvýšením počtu
vlákien čítajúcich z ssd disku, ani zvíšením počtu vlákien vyhľadávajúcich v 
načítaných dátach. Program bežal viac-menej vždy podobne rýchlo. 

<h3> Pozn: </h3>
V repozitári je aj testovací single-thread program (single_thread.cpp).
Je pomalší ako jeho multithredová verzia (main.cpp) v single-threadovom
nastavení (ak main.cpp nedostane žiadne parametre navyše, beží
na jednom vlákne). Nie je mi celkom jasné prečo. 
Vhodným nastavením veľkosti read buffera sa však dá 
dosiahnuť, že single_thread.cpp beží podobne rýchlo,
ako main.cpp.
