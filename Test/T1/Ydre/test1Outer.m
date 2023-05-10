%Der er 50 ms mellem hver sampling
clc
clear all
close all

load('dataYdre.mat');


realdata=dataYdre(:,1);
stroem=dataYdre(:,2);

n=1250;
for i = 1:n
    tid(i) = 0.05 * i;
end

start=[0;0;0;0;0;0;0;0;0;0];
start(1)=1;
for i=2:8
    start(i)=start(i-1)+n;
end
start(9)=10400;
start(10)=12080;
for i = 1:10
    data(:,i)=realdata(start(i):start(i)+(n-1));
    dataStroem(:,i)=stroem(start(i):start(i)+(n-1));
end

n=length(data)

for i = 1:10
    dataFilt(1,i) = data(1,i);
    dataFilt(2,i) = (data(1,i) + data(2,i))/2;
    dataFiltStroem(1,i) = dataStroem(1,i);
    dataFiltStroem(2,i) = (dataStroem(1,i) + dataStroem(2,i))/2;
end

for i = 1:10
    for ii=1:n-2
        dataFilt(ii+2,i)=(data(ii,i)+data(ii+1,i)+data(ii+2,i))/3;
        dataFiltStroem(ii+2,i)=(dataStroem(ii,i)+dataStroem(ii+1,i)+dataStroem(ii+2,i))/3;        
    end
end


temp = [0;0;0;0;0;0;0;0;0;0];

for i=1:10
    for ii=1:n
        temp(i) = temp(i) + dataFilt(ii,i);
    end
end


offset = temp/n;

tempStroem = [0;0;0;0;0;0;0;0;0;0];

for i=1:10
    for ii=1:n
        tempStroem(i) = tempStroem(i) + dataFiltStroem(ii,i);
    end
end


offsetStroem = tempStroem/n;


for i=1:10
    figure(i);
    subplot(2,1,1);
    plot(tid,dataFilt(:,i)-offset(i));
    a=sprintf('Ydre regulator T.1 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
    subplot(2,1,2);
    plot(tid,dataFiltStroem(:,i)-offsetStroem(i));
    a=sprintf('Strøminput ved ydre regulator T.1 gentagelse %d',i);
    title(a)
    ylabel("Strøm [A]");
    xlabel("Tid [s]");
end



temp=[0;0;0;0;0;0;0;0;0;0];
for i = 1:10
    for ii=1:n
        temp(i) = temp(i) + (dataFilt(ii,i)-offset(i))^2;
    end
end
var=[0;0;0;0;0;0;0;0;0;0];
for i = 1:10
    var(i) = 1/(n-1) * temp(i);
end

