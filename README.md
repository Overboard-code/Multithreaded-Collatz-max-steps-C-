### This code will fing the longext Collatz sequence below the command line argument or 1,000,000 as a default.  
#### to 1 billion took about 5 seconds on my old i7 
```text
$g++ -o collatz2 -O4 collatz_omp.cpp
$collatz2 1000000000
Starting Collatz calculations from 1 up to 1,000,000,000 with 12 threads.
[[2025-12-05 17:07:27: TH-3] Started Thread for 83,333,334 to 166,666,666
2025-12-05 17:07:27: TH-5] Started Thread for 250,000,000 to 333,333,332
[2025-12-05 17:07:27: TH-2] Started Thread for 1 to 83,333,333
[2025-12-05 17:07:27: TH-12] Started Thread for 833,333,331 to 916,666,663
[2025-12-05 17:07:27: TH-13] Started Thread for 916,666,664 to 1,000,000,000
[2025-12-05 17:07:27: TH-7] Started Thread for 416,666,666 to 499,999,998
[2025-12-05 17:07:27: TH-11] Started Thread for 749,999,998 to 833,333,330
[2025-12-05 17:07:27: TH-8] Started Thread for 499,999,999 to 583,333,331[
2025-12-05 17:07:27: TH-4] Started Thread for 166,666,667 to 249,999,999
[2025-12-05 17:07:27: TH-6] Started Thread for 333,333,333 to 416,666,665
[2025-12-05 17:07:27: TH-10] Started Thread for 666,666,665 to 749,999,997
[2025-12-05 17:07:27: TH-9] Started Thread for 583,333,332 to 666,666,664
[2025-12-05 17:07:29: TH-2] Done. 1 to 83,333,333 was 63,728,127 steps:949
[2025-12-05 17:07:29: TH-3] Done. 83,333,334 to 166,666,666 was 127,456,255 steps:950
[2025-12-05 17:07:29: TH-4] Done. 166,666,667 to 249,999,999 was 226,588,897 steps:956
[2025-12-05 17:07:30: TH-5] Done. 250,000,000 to 333,333,332 was 268,549,803 steps:964
[2025-12-05 17:07:30: TH-6] Done. 333,333,333 to 416,666,665 was 402,824,705 steps:962
[2025-12-05 17:07:30: TH-7] Done. 416,666,666 to 499,999,998 was 453,177,795 steps:957
[2025-12-05 17:07:31: TH-8] Done. 499,999,999 to 583,333,331 was 537,099,607 steps:965
[2025-12-05 17:07:31: TH-9] Done. 583,333,332 to 666,666,664 was 604,237,059 steps:960
[2025-12-05 17:07:31: TH-12] Done. 833,333,331 to 916,666,663 was 848,749,995 steps:976
[2025-12-05 17:07:31: TH-10] Done. 666,666,665 to 749,999,997 was 670,617,279 steps:986
[2025-12-05 17:07:32: TH-11] Done. 749,999,998 to 833,333,330 was 805,649,415 steps:963
[2025-12-05 17:07:32: TH-13] Done. 916,666,664 to 1,000,000,000 was 954,843,751 steps:971

All Done. Longest chain below 1,000,000,000 was 670,617,279 steps:986
Duration : 0:0:4.4683

$collatz2
You can also use a number as an argument. Defaulting to 1,000,000
Starting Collatz calculations from 1 up to 1,000,000 with 12 threads.
[[2025-12-05 08:40:48: TH-8] Started Thread for 499,999 to 583,331
[2025-12-05 08:40:48: TH-11] Started Thread for 749,998 to 833,330
[2025-12-05 08:40:48: TH-10] Started Thread for 666,665 to 749,997
[2025-12-05 08:40:48: TH-13] Started Thread for 916,664 to 1,000,000
[2025-12-05 08:40:48: TH-2] Started Thread for 1 to 83,333
2025-12-05 08:40:48: TH-12] Started Thread for 833,331 to 916,663
[2025-12-05 08:40:48: TH-7] Started Thread for 416,666 to 499,998
[2025-12-05 08:40:48: TH-3] Started Thread for 83,334 to 166,666
[2025-12-05 08:40:48: TH-2[[2025-12-05 08:40:48: TH-6] Started Thread for 333,333 to 416,665
[2025-12-05 08:40:48: TH-5] Started Thread for 250,000 to 333,332
[[2025-12-05 08:40:48: TH-9] Started Thread for 583,332 to 666,664
2025-12-05 08:40:48: TH-3] 2025-12-05 08:40:48: TH-4] Started Thread for 166,667 to 249,999
[2025-12-05 08:40:48: TH-7] Done. 416,666 to 499,998 was 461,263 steps:443
[[2025-12-05 08:40:48: TH-8] Done. 499,999 to 583,331 was 511,935 steps:469
[2025-12-05 08:40:48: TH-6] [[2025-12-05 08:40:48: TH-9] Done. 583,332 to 666,664 was 626,331 steps:508
2025-12-05 08:40:48: TH-10] Done. 666,665 to 749,997 was 704,623 steps:503
Done. 333,333 to 416,665 was 410,011 steps:448
2025-12-05 08:40:48: TH-4] Done. 166,667 to 249,999 was 230,631 steps:442
[2025-12-05 08:40:48: TH-5] Done. 250,000 to 333,332 was 270,271 steps:406
[2025-12-05 08:40:48: TH-12] Done. 833,331 to 916,663 was 837,799 steps:524
] Done. 1 to 83,333 was 77,031 steps:350
[2025-12-05 08:40:48: TH-11] Done. 749,998 to 833,330 was 796,095 steps:467
[2025-12-05 08:40:48: TH-13] Done. 916,664 to 1,000,000 was 939,497 steps:506
Done. 83,334 to 166,666 was 156,159 steps:382

All Done. Longest chain below 1,000,000 was 837,799 steps:524
Duration : 0:0:0.31
```
