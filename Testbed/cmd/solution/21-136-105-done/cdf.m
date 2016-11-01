% data sets
Ed1_bandwidth = [1098,865,1077,736,538,1870,2877,952,2844,2357,972,804,890,622,934,1456,2506,2323,1286,1004,1387,1369,1077,2420,1954,1206,1212,1158,1211,1236,698,1622,1092,1402,794,1130,1845,1078,2537,2853,1684,1059,1076,922,3592,2338,2545,935,946,880,2386,1061,1857,2951,1112,893,1138,2314,1221,861,836,1651,1813,1327,1905,2232];
%Ed2_bandwidth =
sample1_bandwidth = [2137,1139,623,576,505,2511,1604,1833,1889,929,1309,1767,1488,1034,1647,552,597,1138,1553,1504,1372,595,792,974,1079,1064,872,1753,1293,507,599,823,1351,917,1736,966,1127,1197,1385,1737,1659,1655,2026,2385,2567,897,538,2182,1138,1564,1963,1890,1825,1710,1971,1410,962,1166,621,808,1714,1651,852,1722,1408,1683,1857,2311,1411,1378,3348,1415,1420,2611,1389,825,718,1087,1542,1379,1417,918,1442,1379];
%sample2_bandwidth =

Ed1_time = [107.07,104.64,66.11,55.13,75.50,35.00,28.21,113.17,23.36,30.53,107.11,112.09,103.60,104.55,92.66,69.56,48.65,27.85,72.24,66.08,90.47,73.25,102.42,48.15,40.07,84.80,75.66,89.46,98.02,48.30,85.59,72.87,85.89,70.36,83.52,76.25,40.04,74.11,37.64,43.93,59.04,86.38,88.89,73.69,17.65,43.66,48.35,107.58,90.30,83.16,27.11,44.88,25.65,39.35,88.34,98.91,85.67,39.27,95.98,95.41,77.05,47.72,44.56,29.46,20.51,41.84];
%Ed2_time =
sample1_time = [55.00,79.46,38.09,41.21,47.05,32.33,40.79,44.27,57.02,71.50,54.99,58.88,30.28,43.56,56.01,58.93,54.47,76.06,65.23,81.06,47.14,39.05,29.32,23.83,21.51,31.17,38.04,71.55,77.56,72.53,61.38,44.71,86.26,85.36,58.94,31.65,27.13,25.56,74.78,68.33,71.98,71.41,46.28,41.37,25.83,24.04,40.04,9.88,18.94,47.23,40.70,50.54,68.66,58.13,46.41,33.91,49.69,19.42,36.52,28.03,37.00,30.91,59.88,71.46,35.72,29.87,46.00,31.85,45.83,69.12,34.69,69.46,62.20,37.35,65.41,71.02,81.58,75.58,41.77,57.11,28.51,44.01,54.21,67.68];
%sample2_time =

figure(1)
y1=cdfplot(Ed1_bandwidth);
hold on
%y2=cdfplot(Ed2_bandwidth);
%hold on
y3 = cdfplot(sample1_bandwidth);
hold on
%y4 = cdfplot(sample2_bandwidth);
set(y1,'LineWidth',1.5,'color','g');
%set(y2,'LineWidth',1.5,'color','b');
set(y3,'LineWidth',1.5,'color','r');
%set(y4,'LineWidth',1.5,'color','m');
%legend('Ed1','Ed2','sample1','sample2');
legend('Ed','sample');

figure(2)
y5 = cdfplot(Ed1_time);
hold on
%y6 = cdfplot(Ed2_time);
%hold on
y7 = cdfplot(sample1_time);
hold on
%y8 = cdfplot(sample2_time);
%hold on
set(y5,'LineWidth',1.5,'color','g');
%set(y6,'LineWidth',1.5,'color','b');
set(y7,'LineWidth',1.5,'color','r');
%set(y8,'LineWidth',1.5,'color','m');
%legend('Ed1','Ed2','sample1','sample2');
legend('Ed','sample');