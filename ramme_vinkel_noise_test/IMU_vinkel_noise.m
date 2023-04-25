clc 
clear all
load('vinkelData.mat');
n=11696;

for i = 1:n
    tid(i) = 0.01 * i;
end
plot(tid,data)

temp=0;
for i = 1:n
    temp = temp + (data(i)-mean(data))^2;
end
var=1/(n-1)*temp