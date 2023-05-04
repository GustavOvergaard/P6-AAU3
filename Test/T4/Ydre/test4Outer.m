clc
clear all
close all

load('dataYdre.mat');


realdata=[dataYdre(1248:12500)];





dataFilt(1) = realdata(1);
dataFilt(2) = (realdata(1) + realdata(2))/2;

nn = length(realdata);

for ii=1:(nn-2)
    dataFilt(ii+2)=(realdata(ii)+realdata(ii+1)+realdata(ii+2))/3;
end






n=0
n=length(dataFilt);

tid = 0
for i = 1:n
    tid(i) = 0.01 * i;
end



plot(tid,dataFilt);
title('Ydre regulator T.4')
ylabel("Vinkel [rad]");
xlabel("Tid [s]");

%%

for i = 1:10
    dataFilt(1,i) = data(1,i);
    dataFilt(2,i) = (data(1,i) + data(2,i))/2;
end

for i = 1:10
    for ii=1:1048
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
    end
end


temp = zeros(10);

for i=1:10
    for ii=550:1050
        temp(i) = temp(i) + dataFilt(ii,i);
    end
end


offset = temp/500;

for i=1:10
    figure(i);
    plot(tid,data1-offset);
    a=sprintf('Ydre regulator T.4 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

