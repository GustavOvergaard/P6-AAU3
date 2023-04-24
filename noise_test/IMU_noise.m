
clc 
clear all
load('noiseData.mat');
n=10755;

for i = 1:10755
    tid(i) = 0.01 * i;
end
plot(tid,data-mean(data))

temp=0;
for i = 1:10755
    temp = temp + (data(i)-mean(data))^2;
end
var=1/(n-1)*temp