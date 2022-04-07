clc
clear

pat='/home/isv/nasirZadeh/gpu2/cmake-build-debug/result';  % Your folder
fil=fullfile(pat,'*.csv');
d=dir(fil);
result = [];
for k=1:numel(d)
  filename=fullfile(pat,d(k).name);
  r = csvread(filename);
  result=[result;r(:,3:end)];
  % do
end

plot(result(:,1),result(:,2),'.')