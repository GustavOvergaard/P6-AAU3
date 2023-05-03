clc
clear all
close all

load('dataYdre.mat');
load('dataYdre3.mat');

realdata=[dataYdre(1:39869);dataYdre3(1:10264)];

n=2000;
for i = 1:n
    tid(i) = 0.01 * i;
end




data(:,1)=realdata(851:2850);
data(:,2)=realdata(4401:6400);
data(:,3)=realdata(7901:9900);
data(:,4)=realdata(12101:14100);
data(:,5)=realdata(14401:16400);
data(:,6)=realdata(19551:21550);
data(:,7)=realdata(23551:25550);
data(:,8)=realdata(27401:29400);
data(:,9)=realdata(31101:33100);
data(:,10)=realdata(36751:38750);

for i = 1:10
    dataFilt(1,i) = data(1,i);
    dataFilt(2,i) = (data(1,i) + data(2,i))/2;
end

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
    a=sprintf('Ydre regulator T.5 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

