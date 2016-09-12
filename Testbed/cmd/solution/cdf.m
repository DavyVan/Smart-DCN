% data sets
Ed1_bandwidth = [1128,2349,1763,923,1898,1680,3130,1969,1818,4198,2444,4547,3884,4721,3235,2298,1242,3059,2694,3289,2624,3064,1468,3385,4097,4801,3128,2834,3956,3987,2112,4344];
%Ed2_bandwidth =
sample1_bandwidth = [1079,2888,1383,934,896,1338,2529,2094,866,3194,3077,4439,3805,3595,3058,956,5287,2731,2084,949,4224,2887,1130,3291,4415,4111,4861,2404,3963,3374,827,4505];
%sample2_bandwidth =

Ed1_time = [5.58,13.40,3.57,6.82,13.26,7.49,10.05,3.20,3.46,1.50,12.87,1.39,1.62,1.33,1.95,2.74,5.07,2.06,2.34,7.65,2.40,2.05,8.58,1.86,1.54,1.31,2.01,11.10,1.59,1.58,2.98,1.45];
%Ed2_time =
sample1_time = [5.83,10.89,9.10,13.47,14.04,9.41,12.44,3.01,7.27,1.97,10.22,1.42,1.65,1.75,2.06,13.16,1.19,2.30,6.04,13.26,1.49,2.18,11.14,1.91,1.43,1.53,1.30,13.09,1.59,1.87,7.61,1.40];
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
