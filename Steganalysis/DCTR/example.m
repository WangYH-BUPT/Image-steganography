clc; clear all;
addpath(fullfile('D:\Matlab\bin\JPEG_Toolbox'));
% Specify all images for extraction
filepath_stego = strcat('D:\Matlab\bin\images_stego\');
imagenum=10000; Dim=8000;
Feature_Stego=single(zeros(imagenum,Dim));
names = cell(imagenum,1);

quality_factor = 95;
for i=1:imagenum
    imfileStego = strcat(filepath_stego,[num2str(i),'.JPEG']);  
    I_STRUCT = jpeg_read(imfileStego);
    F = DCTR(I_STRUCT, quality_factor);
    Feature_Stego(i,:) = F; 
    name = strcat(num2str(i),'.JPEG');
    names(i,1) = cellstr(name); %%类型转换char----> cell
    disp(strcat('current percentage:',num2str(i),'/',num2str(imagenum),',',imfileStego)); 
    i
end
F = Feature_Stego;
save(['D:\Personal\科研\已经提取的特征、图像库\nsF5-DCTR-95\' strcat('stego030','.mat')],'names','F');    