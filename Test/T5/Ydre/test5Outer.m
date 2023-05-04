clc
clear all
close all

load('dataYdre.mat');
load('StartpinYdre.mat');

realdata=[dataYdre];

n=2000;
for i = 1:n
    tid(i) = 0.01 * i;
end





data(:,1)=realdata(282:(282+1999));
data(:,2)=realdata(5190:(5190+1999));
data(:,3)=realdata(9436:(9436+1999));
data(:,4)=realdata(13054:(13054+1999));
data(:,5)=realdata(16045:(16045+1999));
data(:,6)=realdata(20929:(20929+1999));
data(:,7)=realdata(26543:(26543+1999));
data(:,8)=realdata(30981:(30981+1999));
data(:,9)=realdata(41391:(41391+1999));
data(:,10)=realdata(46259:(46259+1999));

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
