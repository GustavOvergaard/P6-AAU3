clc
clear all
close all

load('dataState.mat');

realdata=[dataState(1:12685)];

n=1050;
for i = 1:n
    tid(i) = 0.01 * i;
end

data(:,1)=realdata(151:1200);
data(:,2)=realdata(1171:2220);
data(:,3)=realdata(2211:3260);
data(:,4)=realdata(3241:4290);
data(:,5)=realdata(4291:5340);
data(:,6)=realdata(5371:6420);
data(:,7)=realdata(6326:7375);
data(:,8)=realdata(7361:8410);
data(:,9)=realdata(8421:9470);
data(:,10)=realdata(11601:12650);

for i = 1:10
    dataFilt(1,i) = data(1,i);
    dataFilt(2,i) = (data(1,i) + data(2,i))/2;
end

for i = 1:10
    for ii=1:1048
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
    end
end


temp = zeros(10);

for i=1:10
    for ii=550:1050
        temp(i) = temp(i) + dataFilt(ii,i);
    end
end


offset = temp/500;

for i=1:10
    figure(i);
    plot(tid,dataFilt(:,i)-offset(i));
    a=sprintf('State regulator T.4 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

