data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
19 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 121433, L2_DCM 121345. temp: 0, data: 1118048. [0:0]
20 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 129276, L2_DCM 129940. temp: 0, data: 1348081. [0:0]
buf[0]:  1348081 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
344 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 1718670, L2_DCM 1896581. temp: 0, data: 1515855. [0:0]
361 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 1297735, L2_DCM 1916955. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
19 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 287, L2_DCM 244. temp: 0, data: 1048576. [0:0]
19 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 407, L2_DCM 354. temp: 0, data: 1048577. [1:1]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
18 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 69007, L2_DCM 68935. temp: 0, data: 1048576. [0:0]
28 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 77056, L2_DCM 79373. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
70 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 284880, L2_DCM 283346. temp: 1048576, data: 1. [0:1]
75 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 272536, L2_DCM 272225. temp: 0, data: 1048576. [0:0]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
16 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 248, L2_DCM 211. temp: 1048576, data: 1. [1:1]
18 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 256, L2_DCM 215. temp: 0, data: 1048576. [0:0]


----------- repeat

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
28 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 362, L2_DCM 285. temp: 0, data: 2094508. [0:0]
33 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 351, L2_DCM 275. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
80 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 241271, L2_DCM 283251. temp: 0, data: 1158406. [0:0]
119 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 235782, L2_DCM 278664. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
16 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 594, L2_DCM 564. temp: 0, data: 1048576. [0:0]
18 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 351, L2_DCM 329. temp: 0, data: 1048577. [1:1]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
16 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 72931, L2_DCM 72852. temp: 0, data: 1048576. [0:0]
21 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 76997, L2_DCM 78200. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
117 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 716818, L2_DCM 716328. temp: 0, data: 1048576. [0:0]
117 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 731716, L2_DCM 732134. temp: 1048576, data: 1. [0:1]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
10 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 110, L2_DCM 84. temp: 1048576, data: 1. [1:1]
13 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 164, L2_DCM 154. temp: 0, data: 1048576. [0:0]

--------------- repeat

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
17 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 104505, L2_DCM 104368. temp: 0, data: 1158964. [0:0]
21 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 105165, L2_DCM 105192. temp: 0, data: 1267964. [0:0]
buf[0]:  1267964 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
170 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 308074, L2_DCM 316983. temp: 0, data: 1885483. [0:0]
172 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 230760, L2_DCM 319669. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
35 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 440, L2_DCM 362. temp: 0, data: 1048576. [0:0]
41 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 227, L2_DCM 189. temp: 0, data: 1048577. [1:1]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
17 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 78507, L2_DCM 78457. temp: 1048576, data: 1. [0:1]
23 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 78624, L2_DCM 78583. temp: 0, data: 1048576. [0:0]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
79 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 391423, L2_DCM 391065. temp: 0, data: 1048576. [0:0]
83 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 401438, L2_DCM 401628. temp: 1048576, data: 1. [0:1]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
10 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 151, L2_DCM 117. temp: 0, data: 1048576. [0:0]
11 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 140, L2_DCM 99. temp: 1048576, data: 1. [1:1]

------------------------ repeat

ata sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
19 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 61864, L2_DCM 61693. temp: 0, data: 1055400. [0:0]
40 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 60583, L2_DCM 60306. temp: 0, data: 1778472. [0:0]
buf[0]:  1778472 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
280 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 1037088, L2_DCM 1082175. temp: 0, data: 2093849. [0:0]
281 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 693861, L2_DCM 995849. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
18 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 316, L2_DCM 285. temp: 0, data: 1048576. [0:0]
28 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 550, L2_DCM 506. temp: 0, data: 1048577. [1:1]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
23 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 51138, L2_DCM 51074. temp: 0, data: 1048576. [0:0]
43 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 61777, L2_DCM 61444. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
62 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 112103, L2_DCM 112466. temp: 0, data: 1048576. [0:0]
72 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 139430, L2_DCM 138692. temp: 1048576, data: 1. [0:1]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
17 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 415, L2_DCM 356. temp: 1048576, data: 1. [1:1]
27 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 335, L2_DCM 222. temp: 0, data: 1048576. [0:0]

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
16 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 100101, L2_DCM 100021. temp: 0, data: 1026916. [0:0]
19 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 100682, L2_DCM 100650. temp: 0, data: 1403045. [0:0]
buf[0]:  1403045 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
161 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 716659, L2_DCM 735688. temp: 0, data: 1215799. [0:0]
191 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 531952, L2_DCM 724591. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
11 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 131, L2_DCM 98. temp: 0, data: 1048576. [0:0]
12 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 212, L2_DCM 183. temp: 0, data: 1048577. [1:1]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
13 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 68788, L2_DCM 68741. temp: 0, data: 1048576. [0:0]
23 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 69222, L2_DCM 69175. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
37 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 161251, L2_DCM 162003. temp: 1048576, data: 1. [0:1]
62 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 153092, L2_DCM 152768. temp: 0, data: 1048576. [0:0]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
10 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 137, L2_DCM 114. temp: 1048576, data: 1. [1:1]
12 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 174, L2_DCM 161. temp: 0, data: 1048576. [0:0]

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
17 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 108835, L2_DCM 109081. temp: 0, data: 1105705. [0:0]
20 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 107462, L2_DCM 107425. temp: 0, data: 1343897. [0:0]
buf[0]:  1343897 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
279 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 1364801, L2_DCM 1617373. temp: 0, data: 1855734. [0:0]
288 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 1406311, L2_DCM 1650462. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
25 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 377, L2_DCM 355. temp: 0, data: 1048577. [1:1]
30 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 380, L2_DCM 342. temp: 0, data: 1048576. [0:0]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
13 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 92243, L2_DCM 92202. temp: 0, data: 1048576. [0:0]
20 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 92166, L2_DCM 92129. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
56 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 318691, L2_DCM 315521. temp: 1048576, data: 1. [0:1]
76 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 302345, L2_DCM 302207. temp: 0, data: 1048576. [0:0]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
12 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 211, L2_DCM 181. temp: 1048576, data: 1. [1:1]
18 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 357, L2_DCM 323. temp: 0, data: 1048576. [0:0]

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
18 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 95644, L2_DCM 95746. temp: 0, data: 1354048. [0:0]
21 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 102921, L2_DCM 103409. temp: 0, data: 1425695. [0:0]
buf[0]:  1425695 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
194 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 959922, L2_DCM 982958. temp: 0, data: 1302010. [0:0]
219 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 745387, L2_DCM 990445. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
13 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 172, L2_DCM 148. temp: 0, data: 1048577. [1:1]
17 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 367, L2_DCM 343. temp: 0, data: 1048576. [0:0]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
20 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 96858, L2_DCM 96243. temp: 1048576, data: 1. [0:1]
23 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 97318, L2_DCM 97252. temp: 0, data: 1048576. [0:0]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
61 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 366144, L2_DCM 365285. temp: 1048576, data: 1. [0:1]
82 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 345882, L2_DCM 345342. temp: 0, data: 1048576. [0:0]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
11 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 125, L2_DCM 85. temp: 1048576, data: 1. [1:1]
10 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 129, L2_DCM 118. temp: 0, data: 1048576. [0:0]

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
17 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 84791, L2_DCM 84834. temp: 0, data: 1146830. [0:0]
19 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 84668, L2_DCM 84797. temp: 0, data: 1324386. [0:0]
buf[0]:  1324386 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
426 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 1500916, L2_DCM 2189278. temp: 0, data: 1976902. [0:0]
431 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 2023146, L2_DCM 2192123. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
14 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 198, L2_DCM 187. temp: 0, data: 1048576. [0:0]
20 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 262, L2_DCM 241. temp: 0, data: 1048577. [1:1]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
17 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 102058, L2_DCM 102012. temp: 0, data: 1048576. [0:0]
34 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 101715, L2_DCM 101597. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
82 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 440697, L2_DCM 440682. temp: 0, data: 1048576. [0:0]
83 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 455747, L2_DCM 450444. temp: 1048576, data: 1. [0:1]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
11 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 166, L2_DCM 143. temp: 1048576, data: 1. [1:1]
25 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 982, L2_DCM 924. temp: 0, data: 1048576. [0:0]

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
18 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 75031, L2_DCM 74576. temp: 0, data: 1173700. [0:0]
21 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 75208, L2_DCM 75138. temp: 0, data: 1478354. [0:0]
buf[0]:  1478354 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
426 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 2255448, L2_DCM 2440288. temp: 0, data: 1570928. [0:0]
442 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 1618187, L2_DCM 2454384. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
14 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 338, L2_DCM 312. temp: 0, data: 1048577. [1:1]
16 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 235, L2_DCM 217. temp: 0, data: 1048576. [0:0]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
12 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 54515, L2_DCM 54484. temp: 0, data: 1048576. [0:0]
20 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 55850, L2_DCM 56054. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
52 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 312771, L2_DCM 304822. temp: 1048576, data: 1. [0:1]
75 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 292486, L2_DCM 291895. temp: 0, data: 1048576. [0:0]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
15 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 355, L2_DCM 326. temp: 0, data: 1048576. [0:0]
17 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 337, L2_DCM 319. temp: 1048576, data: 1. [1:1]

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
18 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 97120, L2_DCM 96946. temp: 0, data: 1022889. [0:0]
21 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 95649, L2_DCM 95638. temp: 0, data: 1255459. [0:0]
buf[0]:  1255459 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
446 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 2335044, L2_DCM 2599571. temp: 0, data: 1698608. [0:0]
459 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 1979979, L2_DCM 2642966. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
13 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 233, L2_DCM 198. temp: 0, data: 1048576. [0:0]
14 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 192, L2_DCM 189. temp: 0, data: 1048577. [1:1]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
13 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 84047, L2_DCM 84020. temp: 0, data: 1048576. [0:0]
18 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 85206, L2_DCM 85207. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
64 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 391683, L2_DCM 381442. temp: 1048576, data: 1. [0:1]
79 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 367998, L2_DCM 367347. temp: 0, data: 1048576. [0:0]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
9 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 88, L2_DCM 66. temp: 1048576, data: 1. [1:1]
12 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 196, L2_DCM 162. temp: 0, data: 1048576. [0:0]

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
20 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 104712, L2_DCM 105077. temp: 0, data: 1025598. [0:0]
23 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 103342, L2_DCM 103073. temp: 0, data: 1279324. [0:0]
buf[0]:  1279324 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
447 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 2321698, L2_DCM 2577163. temp: 0, data: 1683904. [0:0]
459 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 2018419, L2_DCM 2672265. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
11 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 224, L2_DCM 218. temp: 0, data: 1048576. [0:0]
16 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 320, L2_DCM 305. temp: 0, data: 1048577. [1:1]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
13 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 45202, L2_DCM 45159. temp: 0, data: 1048576. [0:0]
21 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 55569, L2_DCM 55417. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
110 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 679209, L2_DCM 678432. temp: 0, data: 1048576. [0:0]
112 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 699090, L2_DCM 696672. temp: 1048576, data: 1. [0:1]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
13 mticks, Reader (thread 1), on node 0 (cpu 1). L1_DCM 221, L2_DCM 193. temp: 1048576, data: 1. [1:1]
14 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 417, L2_DCM 347. temp: 0, data: 1048576. [0:0]

data sharing without sync, write
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
18 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 99812, L2_DCM 99739. temp: 0, data: 1189567. [0:0]
17 mticks, Writer (thread 1), on node 0 (cpu 1). L1_DCM 104878, L2_DCM 105041. temp: 0, data: 1462555. [0:0]
buf[0]:  1462555 1 2 3 4 5 6 7
data sharing with sync
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
356 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 1944539, L2_DCM 2010204. temp: 0, data: 1397320. [0:0]
375 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 1436176, L2_DCM 1936331. temp: 0, data: 2097152. [0:0]
buf[0]:  2097152 1 2 3 4 5 6 7
No data sharing
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
10 mticks, Writer (thread 1), on node 0 (cpu 2). L1_DCM 89, L2_DCM 78. temp: 0, data: 1048577. [1:1]
13 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 164, L2_DCM 145. temp: 0, data: 1048576. [0:0]
buf[0]:  1048576 1 2 3 4 5 6 7

data sharing without sync, PAPI, halfhalf
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 0[n]
21 mticks, Writer (thread 0), on node 0 (cpu 2). L1_DCM 109122, L2_DCM 109049. temp: 0, data: 1048576. [0:0]
27 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 110451, L2_DCM 108667. temp: 1048576, data: 1. [0:1]
data sharing whith sync, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 0, synchronization: 1[y]
81 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 524223, L2_DCM 527024. temp: 1048576, data: 1. [0:1]
89 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 473434, L2_DCM 472953. temp: 0, data: 1048576. [0:0]
No data sharing, PAPI
data_race running: 2 threads, 1 nodes, 8 cores, data_sharing: 1, synchronization: 0[n]
10 mticks, Writer (thread 0), on node 0 (cpu 1). L1_DCM 180, L2_DCM 146. temp: 0, data: 1048576. [0:0]
13 mticks, Reader (thread 1), on node 0 (cpu 2). L1_DCM 204, L2_DCM 187. temp: 1048576, data: 1. [1:1]
