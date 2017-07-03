%% packets per hour

format long
more off
set (gca, 'fontname', 'Helvetica', 'fontsize', 20);
measurements = csvread ('measurements.csv');
%measurementscnt = sum (measurements(:,2))

o0n = measurements(:,7);
o0s = measurements(:,8);
o1n = measurements(:,9);
o1s = measurements(:,10);
o2n = measurements(:,11);
o2s = measurements(:,12);

x = [0:13]';

%h = stem(timestamps, measurements(:,2)/10^6, 'marker', 'none') % plots stem graphic
%h = stem(measurements(:,2),x, 'marker', 'none') % plots stem graphic
hist(o0n,100)
%xlim([x(1) x(end)]);
%numberOfXTicks = 5;
%xData = get(h,'XData');
%set(gca,'Xtick',linspace(xData(1),xData(end),numberOfXTicks));
%set(gca,'DatetimeTickFormat', 'dd/mm');
%datetick('x', 'dd/mmm'); % sets x-axis display format
xlim([0 max(oOn)]);
xlabel('execution time [cycles]') % sets x-axis label
ylabel('frequency') % sets y-axis label
title('Distribution of execution times for our testsuite') %set plot title
grid on % enables grid lines
axis([0 max(o0s) 0 6]);
set(gca,'layer','top');

%%
hist(o0s,100)
xlim([0 max(oOs)]);
xlabel('execution time [cycles]') % sets x-axis label
ylabel('frequency') % sets y-axis label
title('Distribution of execution times for our testsuite') %set plot title
grid on % enables grid lines
axis([0 max(o0s) 0 6]);
set(gca,'layer','top');

%%
hist(o1n,100)
xlabel('execution time [cycles]') % sets x-axis label
ylabel('frequency') % sets y-axis label
title('Distribution of execution times for our testsuite') %set plot title
axis([0 max(o0s) 0 6]);
grid on % enables grid lines
set(gca,'layer','top');


%%
hist(o1s,100)
xlabel('execution time [cycles]') % sets x-axis label
ylabel('frequency') % sets y-axis label
title('Distribution of execution times for our testsuite') %set plot title
grid on % enables grid lines
axis([0 max(o0s) 0 6]);
set(gca,'layer','top');

%%
hist(o2n,100)
xlabel('execution time [cycles]') % sets x-axis label
ylabel('frequency') % sets y-axis label
title('Distribution of execution times for our testsuite') %set plot title
grid on % enables grid lines
axis([0 max(o0s) 0 6]);
set(gca,'layer','top');

%%
hist(o2s,100)
xlabel('execution time [cycles]') % sets x-axis label
ylabel('frequency') % sets y-axis label
title('Distribution of execution times for our testsuite') %set plot title
grid on % enables grid lines
axis([0 max(o0s) 0 6]);
set(gca,'layer','top');


%%

mean_val=mean(measurements(:,2)) % calculates the mean
std_val=std(measurements(:,2)) % calculates the standard deviation
[max_val max_ind]=max(measurements(:,2)) %calculates the maximum value
datestr((timestamps(max_ind))) % shows time/date of max in a human readable format
[min_val min_ind]=min(measurements(:,2)) %calculates the minimum value
datestr((timestamps(min_ind))) % shows time/date of min in a human readable format

%% unique srcIPs per hour

format long
more off
set (gca, 'fontname', 'Helvetica', 'fontsize', 20);
measurements = csvread ('p6_src_count_ts.csv');
%measurementscnt = sum (measurements(:,2))

timestamps = datenum (1970, 1, 1, 0, 0, measurements(:,1)); % transforms epoch into datenum format
x = datetime(timestamps,'ConvertFrom','datenum');
h = stem(x, measurements(:,2)/10^6, 'marker', 'none') % plots stem graphic
xlim([x(1) x(end)]);
numberOfXTicks = 5;
xData = get(h,'XData');
set(gca,'Xtick',linspace(xData(1),xData(end),numberOfXTicks));
xlabel('days in weeks of period') % sets x-axis label
ylabel('#srcIPs [millions]') % sets y-axis label
title('Number of unique IP sources per Hour [millions]') %set plot title
grid on % enables grid lines
set(gca,'layer','top');

mean_val=mean(measurements(:,2)) % calculates the mean
std_val=std(measurements(:,2)) % calculates the standard deviation
[max_val max_ind]=max(measurements(:,2)) %calculates the maximum value
datestr((timestamps(max_ind))) % shows time/date of max in a human readable format
[min_val min_ind]=min(measurements(:,2)) %calculates the minimum value
datestr((timestamps(min_ind))) % shows time/date of min in a human readable format

