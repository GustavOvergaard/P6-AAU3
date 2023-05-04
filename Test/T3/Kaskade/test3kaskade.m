clc
clear all
close all

load('dataKaskade.mat');

realdata=[dataKaskade(1:22551)];

n=1500;
for i = 1:n
    tid(i) = 0.01 * i;
end


data(:,1)=realdata(151:1650);
data(:,2)=realdata(2101:3600);
data(:,3)=realdata(4151:5650);
data(:,4)=realdata(6151:7650);
data(:,5)=realdata(8201:9700);
data(:,6)=realdata(10451:11950);
data(:,7)=realdata(12601:14100);
data(:,8)=realdata(14681:16180);
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
    a=sprintf('Kaskade regulator T.3 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

