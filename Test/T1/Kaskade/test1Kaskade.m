%Der er 10 ms mellem hver sampling
clc
clear all
close all


load('dataKaskade.mat');


n=6100;
for i = 1:n
    tid(i) = 0.01 * i;
end





data(:,1)=dataKaskade(1:6100);
data(:,2)=dataKaskade(6101:12200);
data(:,3)=dataKaskade(12201:18300);
data(:,4)=dataKaskade(18301:24400);
data(:,5)=dataKaskade(24401:30500);
data(:,6)=dataKaskade(30501:36600);
data(:,7)=dataKaskade(36601:42700);
data(:,8)=dataKaskade(42701:48800);
data(:,9)=dataKaskade(48801:54900);
data(:,10)=dataKaskade(54901:61000);

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
    a=sprintf('Kaskade regulator T.1 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

