% data sets
Ed1_bandwidth = [1102,2355,1348,521,388,692,449,692,1298,938,2508,1434,1528,4516,2343,4723,4011,4733,4765,1287,4773,4095,385,587,312,1223,1304,575,3127,2983,1877,2080,3802,4725,4709,3036,4709,4085,1678,4593];
%Ed2_bandwidth =
sample1_bandwidth = [494,967,1378,830,592,529,1018,1464,583,450,1092,891,743,1137,738,7900,1188,1280,1097,4076,4389,5012,4675,2145,645,3478,2699,818,882,4028,4016,463,1501,3683,4310,4580,3213,722,1903,719,3567,3764,1706,4444];
%sample2_bandwidth =

Ed1_time = [5.71,13.36,9.34,8.06,10.82,6.06,9.37,6.06,3.23,13.42,12.55,4.39,4.12,1.39,13.43,1.33,1.57,1.33,1.32,9.79,1.32,1.54,10.90,7.14,13.43,3.43,3.22,7.31,2.01,2.11,6.71,3.03,1.66,1.33,1.34,10.36,1.34,1.54,3.75,1.37];
%Ed2_time =
sample1_time = [12.74,10.85,7.61,12.64,10.63,11.89,12.37,8.6,10.79,13.99,9.6,11.77,14.12,5.53,8.54,0.8,8.83,8.2,9.56,1.54,1.43,1.26,1.35,2.94,9.76,1.81,2.33,15.4,14.27,1.56,1.57,13.6,4.19,1.71,1.46,1.38,1.96,14.53,5.51,14.59,1.76,1.67,3.69,1.42];
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
