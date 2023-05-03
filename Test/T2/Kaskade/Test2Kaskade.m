%Der er 10 ms mellem hver sampling
clc
clear all
close all

load('dataKaskade.mat');

realdata=[dataKaskade(1:20169)];

n = 2000;
for i = 1:n
    tid(i) = 0.01 * i;
end


data(:,1)=realdata(1:2000);
data(:,2)=realdata(2001:4000);
data(:,3)=realdata(4001:6000);
data(:,4)=realdata(6001:8000);
data(:,5)=realdata(8001:10000);
data(:,6)=realdata(10001:12000);
data(:,7)=realdata(12101:14100);
data(:,8)=realdata(14101:16100);
data(:,9)=realdata(16151:18150);
data(:,10)=realdata(18151:20150);

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
    a=sprintf('Kaskade regulator T.2 hop %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end




