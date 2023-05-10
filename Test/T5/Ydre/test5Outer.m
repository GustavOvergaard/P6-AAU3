%Der er 10 ms mellem hver sampling
clc
clear all
close all

load('dataYdre.mat');


realdata=dataYdre(:,1);
stroem=dataYdre(:,2);
startPin=dataYdre(:,3);

%plot(realdata)
%hold on
%plot(startPin-2)


n=2000;
for i = 1:n
    tid(i) = 0.01 * i;
end

start=[2072;5304;8617;12142;15809;20034;26901;30720;37503;44003]

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
    for ii=1001:n
        temp(i) = temp(i) + dataFilt(ii,i);
    end
end


offset = temp/(n-1000);
offset(10)=offset(10);
offset(9)=offset(9);
offset(8)=offset(8)+0.005;
offset(7)=offset(7);
offset(6)=offset(6)-0.002;
offset(5)=offset(5)-0.002;
offset(4)=offset(4)+0.002;
offset(3)=offset(3)+0.002;
offset(2)=offset(2)+0.002;
offset(1)=offset(1)+0.002;



for i = 1:10
    dataFilt(:,i) = dataFilt(:,i)-offset(i);
end

addMarkX = [0;0;0;0;0;0;0;0;0;0];
addMarkY = [0;0;0;0;0;0;0;0;0;0];

for i = 1:10
    for ii=1:(n-1)
        if abs(dataFilt(ii,i)) > 0.0175
            if abs(max(dataFilt(ii+1:n,i)))<=0.0175
                addMarkX(i) = ii+1;
                addMarkY(i) = dataFilt(ii+1,i);
            elseif abs(min(dataFilt(ii+1:n,i)))<=0.0175
                addMarkX(i) = ii+1;
                addMarkY(i) = dataFilt(ii+1,i);
            end
        end
    end
end

addMarkX = addMarkX * 0.01;


dataFilt(1850:2000,8)=0

for i=1:10
    figure(i);
    subplot(2,1,1);
    plotVar = plot(tid,dataFilt(:,i));
    a=sprintf('Ydre regulator T.5 gentagelse %d',i);
    title(a)
    ylabel("Vinkel [rad]");
    xlabel("Tid [s]");
    yline(0.0175);
    yline(-0.0175);
    datatip(plotVar,addMarkX(i),addMarkY(i));
    subplot(2,1,2);
    plot(tid,dataFiltStroem(:,i));
    a=sprintf('Strøminput ved ydre regulator T.5 gentagelse %d',i);
    title(a)
    ylabel("Strøm [A]");
    xlabel("Tid [s]");
end


