clc
clear all
close all

load('dataState.mat');

realdata=[dataState(1:37454)];

n=2000;
for i = 1:n
    tid(i) = 0.01 * i;
end





data(:,1)=realdata(1:2000);
data(:,2)=realdata(3401:5400);
data(:,3)=realdata(6651:8650);
data(:,4)=realdata(9701:11700);
data(:,5)=realdata(14101:16100);
data(:,6)=realdata(17401:19400);
data(:,7)=realdata(20801:22800);
data(:,8)=realdata(23901:25900);
data(:,9)=realdata(27351:29350);
data(:,10)=realdata(30651:32650);

for i = 1:10
    dataFilt(1,i) = data(1,i);
    dataFilt(2,i) = (data(1,i) + data(2,i))/2;
end

for i = 1:10
    for ii=1:1998
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
    end
end


temp = zeros(10);

for i=1:10
    for ii=1001:2000
        temp(i) = temp(i) + dataFilt(ii,i);
    end
end


offset = temp/1000;

for i=1:10
    figure(i);
    plot(tid,dataFilt(:,i)-offset(i));
    a=sprintf('State space regulator T.5 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

