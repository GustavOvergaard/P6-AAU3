clc
clear all
close all
load('noiseData.mat');
n=10755;

for i = 1:10755
    tid(i) = 0.01 * i;
end
plot(tid,data-mean(data))
xlabel("Tid [s]");
ylabel("Vinkelhastighed [rad/s]");

temp=0;
for i = 1:10755
    temp = temp + (data(i)-0)^2;
end
var=1/(n-1)*temp