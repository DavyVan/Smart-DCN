times = [113.17 86.26;111.44 94.33];
error_std = rand(2,2) * 10;
y = barweb(times, error_std, 1, ['normal','double demand'], [], [], [], [0 1 0; 1 0 0], [], ['weight-matching' 'sample'], 2, 'plot');
% y=bar(times);
% xSTR = {'weight-matching','sample','weight-matching-x2','sample-x2'};
% set(gca,'XTickLabel',xSTR,'fontsize',14);
% hold on;

% errorbar(times,error_std,'rx');
hold on;

