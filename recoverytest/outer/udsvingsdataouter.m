clc
clear all
close all

load('data1.mat');
load('data2.mat');
load('data3.mat');
load('data4.mat');
load('data6.mat');
load('data7.mat');
realdata=[data1(50:2900,1);data2(570:4550,1);data6(800:2050,1);data7(200:1000,1)]

n = length(realdata);
for i = 1:n
    tid(i) = 0.01 * i;
end

plot(tid,realdata);
ylabel("Vinkel [rad]");
xlabel("Tid [s]");

