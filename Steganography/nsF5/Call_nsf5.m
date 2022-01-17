
clc;clear all;
addpath(fullfile('D:\Matlab\bin\SI-UNIWARD_matlab\JPEG_Toolbox'));
pathcover1='D:\Matlab\bin\images_cover\';
pathcover2='D:\Matlab\bin\images_stego\';

fileExtCover1='*.JPEG';
Houzhui1=dir(fullfile(pathcover1,fileExtCover1));
Houzhui2=dir(fullfile(pathcover2,fileExtCover1));
for i=1:10000
    cover_path(i).path = fullfile(strcat(pathcover1,Houzhui1(i).name));
    stego_path(i).path = fullfile(strcat(pathcover2,Houzhui2(i).name));%这里相当于借用之前的名字
end
fprintf('Embedding using Matlab file');
ALPHA = 0.5; % relative payload in terms of bits per nonzero AC DCT coefficient
SEED = 99; % PRNG seed for the random walk over the coefficients
for i=1:10000
    nsf5_simulation(cover_path(i).path,stego_path(i).path,ALPHA,SEED);
    i %为了了解进行到哪一步了
end
fprintf('-----\n');
fprintf('nsF5 simulation finished\n');