%Der er 10 ms mellem hver sampling
clc
clear all
close all

load('dataYdre.mat');


n=6100;
for i = 1:n
    tid(i) = 0.01 * i;
end





data(:,1)=dataYdre(2001:8100);
data(:,2)=dataYdre(6101:12200);
data(:,3)=dataYdre(12201:18300);
data(:,4)=dataYdre(31125:31125+6099);
data(:,5)=dataYdre(38027:38027+6099);
data(:,6)=dataYdre(50000:50000+6099);
data(:,7)=dataYdre(43000:43000+6099);
data(:,8)=dataYdre(62000:62000+6099);
data(:,9)=dataYdre(79550:79550+6099);
data(:,10)=dataYdre(83000-6099:83000);

for i = 1:10
    for ii=1:6098
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
    end
end


temp = zeros(10);

for i=1:10
    for ii=1:6100
        temp(i) = temp(i) + dataFilt(ii,i);
    end
end


offset = temp/6100;


for i=1:10
    figure(i);
    plot(tid,dataFilt(:,i)-offset(i));
    a=sprintf('Ydre regulator T.1 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

