%Der er 10 ms mellem hver sampling
clc
clear all
close all

load('dataYdre.mat');


n=6100;
for i = 1:n
    tid(i) = 0.01 * i;
end



plot(dataYdre-mean(dataYdre));



data(:,1)=dataYdre(1:6100);
data(:,2)=dataYdre(6101:12200);
data(:,3)=dataYdre(12201:18300);
data(:,4)=dataYdre(18301:24400);
data(:,5)=dataYdre(24401:30500);
data(:,6)=dataYdre(30501:36600);
data(:,7)=dataYdre(36601:42700);
data(:,8)=dataYdre(42701:48800);
data(:,9)=dataYdre(48801:54900);
data(:,10)=dataYdre(58001:64100);

for i = 1:10
    for ii=1:6098
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
    end
end


temp = zeros(10);




for i=1:10
    figure(i);
    plot(tid,dataFilt(:,i)-mean(dataFilt(:,i)));
    a=sprintf('Ydre regulator T.1 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

