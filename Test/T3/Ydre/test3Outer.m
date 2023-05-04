clc
clear all
close all

load('dataYdre.mat');
load('StartpinYdre.mat');


realdata=[dataYdre];



n=1500;
for i = 1:n
    tid(i) = 0.01 * i;
end

data(:,1)=realdata(359:(359+1499));
data(:,2)=realdata(3271:(3271+1499));
data(:,3)=realdata(6543:(6543+1499));
data(:,4)=realdata(9819:(9819+1499));
data(:,5)=realdata(13180:(13180+1499));
data(:,6)=realdata(16451:(16451+1499));
data(:,7)=realdata(18042:(18042+1499));
data(:,8)=realdata(21280:(21280+1499));
data(:,9)=realdata(24674:(24674+1499));
data(:,10)=realdata(29877:(29877+1499));


for i = 1:10
    dataFilt(1,i) = data(1,i);
    dataFilt(2,i) = (data(1,i) + data(2,i))/2;
end

for i = 1:10
    for ii=1:1498
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
    end
end


temp = zeros(10);

for i=1:10
    for ii=1001:1500
        temp(i) = temp(i) + dataFilt(ii,i);
    end
end


offset = temp/500;

for i=1:10
    figure(i);
    plot(tid,dataFilt(:,i)-offset(i));
    a=sprintf('Ydre regulator T.3 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

