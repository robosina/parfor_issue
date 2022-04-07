clc
clear
close all
%% section 1 opening the text file
first_tic = tic;
fid1= fopen('/home/supercomputer/Documents/parfor_issue/cor52.txt','r');
sizeA=[2,10000];
formatSpec ='%f%f';
A=fscanf(fid1,formatSpec,sizeA);
fclose(fid1);
fprintf('time elapsed for reading text file is:%f\n',toc(first_tic))
%%
mat=zeros(940,940);
A=A';
for i1=1:size(A,1)/2-1
        fprintf('i1=%d\n',i1);
    for i2=i1+1:size(A,1)/2
        p1 = find_mb(A,i1);
        p2 = find_mb(A,i2);
        xsel=A(2*i1-1:2*i1,1);
        Start=min(xsel);
        End=max(A(2*i1-1:2*i1,1));
        x0 = (p2(2)-p1(2))/(p1(1)-p2(1));
        %     fprintf('start=%f x0=%f end=%f\n',Start,x0,End);
        if(Start <=x0 && x0<=End)
            if(abs(p1(1)*x0)>0.01)
                y0=p1(1)*x0+p1(2);
                x=round(10*(x0+47));
                y=round(10*(y0+47));
%                 if(i1==91 && i2>93 && i2<200)
%                     fprintf("%d %d %f %f x:%d y:%d\n",i1,i2,x0,y0,x,y);
                    mat(x,y)=mat(x,y)+1;
%                 end
                
                
                %             fprintf('%d,%d,%f,%f\n',i1,i2,x0,p1(1)*x0+p1(2));
            end
            j=j+1;
        end
    end
end

imagesc(mat)

function p=find_mb(A,i)
As = A(2*i-1:2*i,:);
%     p(i,:)=polyfit(As(:,1), As(:,2),1);
m = (As(1,2)-As(2,2))/(As(1,1)-As(2,1));
b = -m*As(1,1) + As(1,2);
p= [m,b];
end