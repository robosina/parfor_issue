%parpool(5*2_ (-0.41,-0.39) )
clc
clear
close all
%% section 1 opening the text file
first_tic = tic;
fid1= fopen('/home/supercomputer/Documents/parfor_issue/cor52.txt','r');
sizeA=[2,Inf];
formatSpec ='%f%f';
A=fscanf(fid1,formatSpec,sizeA);
fclose(fid1);
fprintf('time elapsed for reading text file is:%f\n',toc(first_tic))
%%
A=A';
i1 = 153;
for i2=90:100
    p1 = find_mb(A,i1);
    
    p2 = find_mb(A,i2);
%     fprintf('index:%d\n',2*i1);
    xsel=A(2*i1-1:2*i1,1);
    Start=min(xsel);
    End=max(A(2*i1-1:2*i1,1));
    x0 = (p2(2)-p1(2))/(p1(1)-p2(1));
%     fprintf('start=%f x0=%f end=%f\n',Start,x0,End);
    if(Start <=x0 && x0<=End)
        if(abs(p1(1)*x0)>0.01)
            fprintf('%d,%d,%f,%f\n',i1,i2,x0,p1(1)*x0+p1(2));
        end
        j=j+1;
    end
end

p1 = 10501;
p2 = 10576;
disp(A(2*p1-1:2*p1,:))
disp(A(2*p2-1:2*p2,:))


function p=find_mb(A,i)
As = A(2*i-1:2*i,:);
%     p(i,:)=polyfit(As(:,1), As(:,2),1);
m = (As(1,2)-As(2,2))/(As(1,1)-As(2,1));
b = -m*As(1,1) + As(1,2);
p= [m,b];
end