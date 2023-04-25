clc 
clear all
load('SvinghjulHastighedData.mat');


n=17803;

for i = 1:n
    tid(i) = 0.01 * i;
end
plot(tid,data-mean(data))

temp=0;
for i = 1:n
    temp = temp + (data(i)-mean(data))^2;
end
var=1/(n-1)*temp