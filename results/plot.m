%% plots the distributions of execution times for 
%% various optimization levels & code variants

%files = {'1-hw.csv', '1-pasim-full.csv',  '2-hw.csv',  '2-pasim-full.csv',  '3-hw.csv',  '3-pasim-full.csv'};

format long
more off
set (gca, 'fontname', 'Helvetica', 'fontsize', 20);

mkdir plots     
files = dir('csv_files/*.csv');   
for i = 1:length(files)
  filename = files(i).name; 
  measurements = csvread (['csv_files/' filename]);

  o0n = measurements(:,1);
  o0s = measurements(:,2);
  o1n = measurements(:,3);
  o1s = measurements(:,4);
  o2n = measurements(:,5);
  o2s = measurements(:,6);

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
  xlim([0 9333186]);
  xlabel('execution time [cycles]') % sets x-axis label
  ylabel('frequency') % sets y-axis label
  title('Distribution of execution times for our testsuite') %set plot title
  grid on % enables grid lines
  axis([0 9333186 0 6]);
  set(gca,'layer','top');
  print(['plots/' filename '-o0n.jpg'], '-djpg')
  
  %
  hist(o0s,100)
  xlim([0 9333186]);
  xlabel('execution time [cycles]') % sets x-axis label
  ylabel('frequency') % sets y-axis label
  title('Distribution of execution times for our testsuite') %set plot title
  grid on % enables grid lines
  axis([0 9333186 0 6]);
  set(gca,'layer','top');
  print(['plots/' filename '-o0s.jpg'], '-djpg')

  %
  hist(o1n,100)
  xlim([0 9333186]);
  xlabel('execution time [cycles]') % sets x-axis label
  ylabel('frequency') % sets y-axis label
  title('Distribution of execution times for our testsuite') %set plot title
  grid on % enables grid lines
  axis([0 9333186 0 6]);
  set(gca,'layer','top');
  print(['plots/' filename '-o1n.jpg'], '-djpg')


  %
  hist(o1s,100)
  xlim([0 9333186]);
  xlabel('execution time [cycles]') % sets x-axis label
  ylabel('frequency') % sets y-axis label
  title('Distribution of execution times for our testsuite') %set plot title
  grid on % enables grid lines
  axis([0 9333186 0 6]);
  set(gca,'layer','top');
  print(['plots/' filename '-o1s.jpg'], '-djpg')

  %
  hist(o2n,100)
  xlim([0 9333186]);
  xlabel('execution time [cycles]') % sets x-axis label
  ylabel('frequency') % sets y-axis label
  title('Distribution of execution times for our testsuite') %set plot title
  grid on % enables grid lines
  axis([0 9333186 0 6]);
  set(gca,'layer','top');
  print(['plots/' filename '-o2n.jpg'], '-djpg')

  %
  hist(o2s,100)
  xlim([0 9333186]);
  xlabel('execution time [cycles]') % sets x-axis label
  ylabel('frequency') % sets y-axis label
  title('Distribution of execution times for our testsuite') %set plot title
  grid on % enables grid lines
  axis([0 9333186 0 6]);
  set(gca,'layer','top');
  print(['plots/' filename '-o2s.jpg'], '-djpg')

end


%%

%mean_val=mean(measurements(:,2)) % calculates the mean
%std_val=std(measurements(:,2)) % calculates the standard deviation
%[max_val max_ind]=max(measurements(:,2)) %calculates the maximum value
%datestr((timestamps(max_ind))) % shows time/date of max in a human readable format
%[min_val min_ind]=min(measurements(:,2)) %calculates the minimum value
%datestr((timestamps(min_ind))) % shows time/date of min in a human readable format

