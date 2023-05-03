clc
clear all
close all

load('dataState.mat');

realdata=[dataState(1:22551)];

n=1500;
for i = 1:n
    tid(i) = 0.01 * i;
end


data(:,1)=realdata(51:1550);
data(:,2)=realdata(2551:4050);
data(:,3)=realdata(4401:5900);
data(:,4)=realdata(6501:8000);
data(:,5)=realdata(8651:10150);
data(:,6)=realdata(10601:12100);
data(:,7)=realdata(12601:14100);
data(:,8)=realdata(14701:16200);
data(:,9)=realdata(16701:18200);
data(:,10)=realdata(18701:20200);

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
    a=sprintf('State space regulator T.3 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

