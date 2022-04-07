%parpool(5*2_ (-0.41,-0.39) )
clc
clear
close all
%% section 1 opening the text file
first_tic = tic;
fid1= fopen('cor.txt','r');
sizeA=[2,4000];
formatSpec ='%f%f';
A=fscanf(fid1,formatSpec,sizeA);
fclose(fid1);
fprintf('time elapsed for reading text file is:%f\n',toc(first_tic))
%%
tic
A=A';

k=size(A,1)/2;
p=zeros(k,2);
Points=zeros(k,2);
j=1;
for i=1:150
    As = A(2*i-1:2*i,:);
%     p(i,:)=polyfit(As(:,1), As(:,2),1);
    m = (As(1,2)-As(2,2))/(As(1,1)-As(2,1));
    b = -m*As(1,1) + As(1,2);
    p(i,:) = [m,b];
    yval=As(:,2);
    %     hold on
    %     plot(Asub(:,1),Asub(:,2))
    if(i>1)
        for t=1:i-1
            %             x0=fzero(@(x) polyval(p(i,:)-p(t,:),x),3);
            p1 = p(i,:);
            p2 = p(t,:);
            x0 = (p2(2)-p1(2))/(p1(1)-p2(1));
            if(min(As(:,1)) <=x0 && x0<=max(As(:,1)  ))
                Points(j,1) = x0;
                Points(j,2) = m*x0+b;  %Points(j,2)=
                j=j+1;
            end
        end
    end
end
fprintf('time elapsed for calculating points:%f total_time %f\n',toc,toc(first_tic))
%% save the results
tic
fid= fopen('Points.txt','wt');
% save Points in txt file:
for j=1:size(Points,1)
    fprintf(fid,'%g\t %g \n' ,Points(j,1),Points(j,2));
end

fclose(fid);
fprintf('time elapsed for saving the results:%f total_time %f\n',toc,toc(first_tic))
%parpool('close');
