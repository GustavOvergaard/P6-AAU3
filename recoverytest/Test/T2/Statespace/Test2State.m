%Der er 10 ms mellem hver sampling
clc
clear all
close all

load('dataState.mat');
load('dataeStateVenstre.mat');

realdata=[dataState(1:20616);dataStateVenstre(1:10502)];

n = 2000;
for i = 1:n
    tid(i) = 0.01 * i;
end



data(:,1)=realdata(1:2000);
data(:,2)=realdata(2001:4000);
data(:,3)=realdata(4001:6000);
data(:,4)=realdata(6201:8200);
data(:,5)=realdata(8201:10200);
data(:,6)=realdata(20401:22400);
data(:,7)=realdata(22601:24600);
data(:,8)=realdata(24701:26700);
data(:,9)=realdata(26701:28700);
data(:,10)=realdata(28701:30700);

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
    a=sprintf('State space regulator T.2 hop %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

