clc
clear all
close all

load('dataKaskade.mat');

realdata=[dataKaskade(1:54654)];

n=2000;
for i = 1:n
    tid(i) = 0.01 * i;
end




data(:,1)=realdata(1:2000);
data(:,2)=realdata(6851:8850);
data(:,3)=realdata(10701:12700);
data(:,4)=realdata(14801:16800);
data(:,5)=realdata(18801:20800);
data(:,6)=realdata(22701:24700);
data(:,7)=realdata(27301:29300);
data(:,8)=realdata(33001:35000);
data(:,9)=realdata(38101:40100);
data(:,10)=realdata(43201:45200);

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
    a=sprintf('Kaskade regulator T.5 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

