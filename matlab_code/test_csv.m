clc
clear
close all
pat='/home/supercomputer/Documents/parfor_issue/build-gpu2-Desktop_Qt_5_15_2_GCC_64bit-Release/result';  % Your folder
fil=fullfile(pat,'*.csv');
d=dir(fil);
result = [];
mkdir('png')
for k=1:numel(d)
    fprintf('%d from %d\n',k,numel(d));
    filename=fullfile(pat,d(k).name);
      disp(filename)
    r = csvread(filename);
    %   r = imresize(r,[200,200]);
%     H= fspecial('average',3);
%     r = imfilter(r,H);
    %   r = imgaussfilt(r,2);
    %   h = figure('visible','off');
    %   imagesc(r)
    %   saveas(h,sprintf('png/FIG%d.png',k))
    if(isempty(result))
        result = r;
    else
        result = result + r;
    end
end
% result(1:5,1)=0;
% result(result==0)=1;
% result = log2(result);
% result = result/max(result(:));
result(:,941)=[];
% result = real(log10(result));
% result = imgaussfilt(result,1);
result(isinf(result))=0;
[x,y]=meshgrid(linspace(-47,47,940));
contourf(x,y,result,40)
result(471,471)=result(473,473);
% imagesc(result)
vpa(max(result(:)))
% thresh = graythresh(result);

% for i=0:0.05:1
%     bw = imbinarize(result,i);
%       h = figure('visible','off');
%   imagesc(bw)
%   saveas(h,sprintf('png/FIG%f.png',i))
% end