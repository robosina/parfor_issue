
tic
fid1= fopen('cor.txt','r');
sizeA=[2,4000];
formatSpec ='%f%f';
A=fscanf(fid1,formatSpec,sizeA);
fclose(fid1);
A=A';


k=size(A,1)/2;
p=zeros(k,2);

Points = [];
delete(gcp);
%parpool(k);

parfor i=1:k
    Start=min(A(2*i-1:2*i,1));
    End=max(A(2*i-1:2*i,1));
    
    xval=Start:0.1:End;
    p(i,:)=polyfit(A(2*i-1:2*i,1),A(2*i-1:2*i,2),1);
    yval=polyval(p(i,:),xval);
    plotVals{i}=  {xval , yval} %add this line to save needed values to plot
    
    %     hold on
    %     plot(xval,yval)
end


for i=2:k
    Start=min(A(2*i-1:2*i,1));
    End=max(A(2*i-1:2*i,1));
    for t=1:i-1
        %           x0=fzero(@(x) polyval(p(i,:)-p(1:t,:),x),3);
        x0=fzero(@(x) polyval(p(i,:)-p(t,:),x),3);
        %           x0=fzero(@(x) polyval(p(1:i-1,:),x),3);
        if(Start<=x0 && x0<=End)
            Points = [Points; [x0 polyval(p(i,:),x0)]];
        end
    end
end


fid= fopen('Points.txt','wt');
% save Points in txt file:
for j=1:size(Points,1)
    fprintf(fid,'%g\t%g \n' ,Points(j,1),Points(j,2));
end
fclose(fid);

% parfor i=1:k
%     figure(i)
%     plot(plotVals{i}{1},plotVals{i}{2})   %change this line to plot values correctly
%     saveas(gcf,['temp' num2str(i) '.jpg']);
% end

toc

