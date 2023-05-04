%Der er 10 ms mellem hver sampling
clc
clear all
close all

load('dataYdre.mat');


n = 9896;
for i = 1:n
    tid(i) = 0.01 * i;
end

for i=1:9894
    dataFilt(i+2)=(dataYdre(i,1)+dataYdre(i+1,1)+dataYdre(i+2,1))/3;
end

plot(tid,dataFilt);
 a=sprintf('Ydre regulator T.2');
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");

    dataSucces=dataYdre(23:1843);

 n2 = length(dataSucces);   
for i = 1:n2
    tid2(i) = 0.01 * i;
end




plot(tid2,dataSucces-0.005);
a=sprintf('Ydre regulator T.2 succesfuld');
title(a)
ylabel("Vinkel [rad]");
xlabel("Tid [s]");
yline(0.0175);
yline(-0.0175);

%%


for i = 1:10
    for ii=1:1998
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
    end
end


temp = zeros(10);


for i=1:10
    for ii=1001:2000
        temp(i) = temp(i) + dataFilt(ii,i);
    end
end

offset = temp/1000;

for i=1:10
    figure(i);
    plot(tid,dataFilt(:,i)-offset(i));
    a=sprintf('Kaskade regulator T.2 hop %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end




