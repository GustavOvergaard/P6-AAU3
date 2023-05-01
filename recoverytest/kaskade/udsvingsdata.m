clc
clear all
close all

load('data1.mat');
load('data2.mat');
load('data3.mat');
load('data4.mat');
load('data5.mat');
load('data6.mat');
realdata=[data1(2200:3400,1);data2(100:1400,1);data3(190:900,1);data4(1:1650,1);data5(100:1250,1);data6(100:1550,1)]

n = length(realdata);
for i = 1:n
    tid(i) = 0.01 * i;
end

plot(tid,realdata-0.075);
ylabel("Vinkel [rad]");
xlabel("Tid [s]");