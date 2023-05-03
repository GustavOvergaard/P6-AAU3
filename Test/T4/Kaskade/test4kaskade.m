clc
clear all
close all

load('dataKaskade.mat');

realdata=[dataKaskade(1:12299)];

n=1050;
for i = 1:n
    tid(i) = 0.01 * i;
end


data(:,1)=realdata(351:1400);
data(:,2)=realdata(1451:2500);
data(:,3)=realdata(2801:3850);
data(:,4)=realdata(3901:4950);
data(:,5)=realdata(5001:6050);
data(:,6)=realdata(6101:7150);
data(:,7)=realdata(7151:8200);
data(:,8)=realdata(8201:9250);
data(:,9)=realdata(9281:10330);
data(:,10)=realdata(11250:12299);

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
    a=sprintf('Kaskade regulator T.4 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
end



%plot(data);

