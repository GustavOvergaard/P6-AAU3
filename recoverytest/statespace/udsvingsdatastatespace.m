clc
clear all
close all

load('data1.mat');
load('data2.mat');
load('data3.mat');
load('data4.mat');

%plot(data4);

realdata=[data1(1:3850,1);data2(50:850,1);data3(50:2150,1);data4(50:1900,1)]

plot(realdata);
n = length(realdata);
for i = 1:n
    tid(i) = 0.01 * i;
end

plot(tid,realdata);
ylabel("Vinkel [rad]");
xlabel("Tid [s]");