clc
clear all
close all

load('dataYdre.mat');
load('dataYdre2.mat');
load('dataYdre3.mat');

realdata=[dataYdre(1:10418);dataYdre2(1:10786);dataYdre3(1:4115)];

dataFilt(1) = realdata(1);
dataFilt(2) = (realdata(1) + realdata(2))/2;

nn = length(realdata);

for ii=1:(nn-2)
    dataFilt(ii+2)=(realdata(ii)+realdata(ii+1)+realdata(ii+2))/3;
end


data1 = dataFilt(220:1900);
data2 = dataFilt(1921:2920);
data3 = dataFilt(3451:4450);
data4 = dataFilt(6150:7429);
data5 = dataFilt(7430:8529);
data6 = dataFilt(8531:9350);
data7 = dataFilt(10800:11799);
data8 = dataFilt(11910:12909);
data9 = dataFilt(13320:14319);
data10 = dataFilt(14850:15849);


%%
n=0
n=length(data10);

tid = 0
for i = 1:n
    tid(i) = 0.01 * i;
end

temp=0

for ii=(n-200):n
    temp = temp + data10(ii);
end

offset=0
offset = temp/200;


plot(tid,data10-offset+0.04);
title('Ydre regulator T.4 gentagelse 10')
ylabel("Vinkel [rad]");
xlabel("Tid [s]");
yline(0.0175);
yline(-0.0175);

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

