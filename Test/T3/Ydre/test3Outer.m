clc
clear all
close all

load('dataYdre.mat');
load('dataYdre2.mat');

realdata=[dataYdre(1:29318);dataYdre2(1:13274)];

n=1500;
for i = 1:n
    tid(i) = 0.01 * i;
end

data(:,1)=realdata(1:1500);
data(:,2)=realdata(2651:4150);
data(:,3)=realdata(6201:7700);
data(:,4)=realdata(7851:9350);
data(:,5)=realdata(11351:12850);
data(:,6)=realdata(13751:15250);
data(:,7)=realdata(16551:18050);
data(:,8)=realdata(20051:21550);
data(:,9)=realdata(22251:23750);
data(:,10)=realdata(24601:26100);

for i = 1:10
    dataFilt(1,i) = data(1,i);
    dataFilt(2,i) = (data(1,i) + data(2,i))/2;
end

for i = 1:10
    for ii=1:1498
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
    end
end


temp = zeros(10);

for i=1:10
    for ii=1001:1500
        temp(i) = temp(i) + dataFilt(ii,i);
    end
end


offset = temp/500;

for i=1:10
    figure(i);
    plot(tid,dataFilt(:,i)-offset(i));
    a=sprintf('Ydre regulator T.3 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

