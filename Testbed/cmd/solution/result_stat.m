ed_times = [];
ed_bandwidth = [];
sample_times = [];
sample_bandwidth = [];

% read files from ed
file_dir = '.\result\ed\';
files = dir(file_dir);
files_count = length(files);
for i = 3:files_count
    path_ = strcat(file_dir, files(i).name);
    f = fopen(path_);
    text = fread(f, '*char')';
    data_array = parse_json(text);
    ed_times(i-2) = data_array{1,1}.end.sum_sent.seconds;
    ed_bandwidth(i-2) = data_array{1,1}.end.sum_sent.bits_per_second;
end

% read files from sample
file_dir = '.\result\sample\';
files = dir(file_dir);
files_count = length(files);
for i = 3:files_count
    path_ = strcat(file_dir, files(i).name);
    f = fopen(path_);
    text = fread(f, '*char')';
    data_array = parse_json(text);
    sample_times(i-2) = data_array{1,1}.end.sum_sent.seconds;
    sample_bandwidth(i-2) = data_array{1,1}.end.sum_sent.bits_per_second;
end

%draw
figure(1)
y1 = cdfplot(ed_times);
hold on;
y2 = cdfplot(sample_times);
set(y1,'LineWidth',1.5,'color','g');
set(y2,'LineWidth',1.5,'color','b');
legend('Weight_matching', 'sample');

figure(2)
y3 = cdfplot(ed_bandwidth);
hold on;
y4 = cdfplot(sample_bandwidth);
set(y3,'LineWidth',1.5,'color','g');
set(y4,'LineWidth',1.5,'color','b');
legend('Weight_matching', 'sample');