% data sets
Ed1_bandwidth = [1405,2370,1306,691,501,655,876,771,540,1625,2348,883,1546,5014,2486,2650,2521,4387,4914,936,3806,2677,661,834,383,957,506,319,3238,3225,963,2798,3415,3106,4409,2722,3568,3662,1748,4382];
%Ed2_bandwidth =
sample1_bandwidth = [1152,990,1277,1207,1250,1267,939,1224,574,500,1142,830,782,629,1838,2974,781,827,925,3476,3537,3397,3208,851,1063,4894,2866,1843,1356,3762,3301,557,573,5234,3665,3318,4913,1139,812,1021,4014,3828,472,5790];
%sample2_bandwidth =

Ed1_time = [4.49,13.27,9.64,6.08,8.38,6.41,4.80,5.45,7.78,7.74,13.40,7.13,4.07,1.25,12.66,2.37,2.50,1.43,1.28,13.45,1.65,2.35,6.35,5.04,10.98,4.39,8.30,13.16,1.95,1.95,13.08,2.25,1.84,2.03,1.43,11.56,1.76,1.72,3.60,1.44];
%Ed2_time =
sample1_time = [5.46,10.59,8.22,8.69,5.04,4.97,13.41,10.28,10.96,12.59,9.18,12.64,13.42,10.02,3.43,2.12,13.42,12.69,11.35,1.81,1.78,1.85,1.96,7.40,5.92,1.29,2.20,6.83,9.28,1.67,1.91,11.31,10.99,1.20,1.72,1.9,1.28,9.21,12.91,10.28,1.57,1.64,13.34,1.09];
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
