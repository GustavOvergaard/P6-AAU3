clc
clear all
close all

load('dataKaskade.mat');

realdata=[dataKaskade(1:54654)];

n=6100;
for i = 1:n
    tid(i) = 0.01 * i;
end


%%


data(:,1)=realdata(1:2000);
data(:,2)=realdata(6901:8900);
data(:,3)=realdata(10901:12900);
data(:,4)=realdata(14801:16800);
data(:,5)=realdata(18801:20800);
data(:,6)=realdata(22901:24900);
data(:,7)=realdata(36601:42700);
data(:,8)=realdata(42701:48800);
data(:,9)=realdata(48801:54900);
data(:,10)=realdata(54901:61000);

for i = 1:10
    dataFilt(1,i) = data(1,i);
    dataFilt(2,i) = (data(1,i) + data(2,i))/2;
end

for i = 1:10
    for ii=1:6098
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
    end
end


temp = zeros(10);




for i=1:10
    figure(i);
    plot(tid,dataFilt(:,i)-mean(dataFilt(:,i)));
    a=sprintf('State space regulator T.1 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

