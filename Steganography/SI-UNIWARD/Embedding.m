
% -------------------------------------------------------------------------
clc; clear all; close all;
addpath(fullfile('..','JPEG_Toolbox'));

% load precover
pathcover='..\images_precover\';
fileExtCover='*.pgm';
Houzhui=dir(fullfile(pathcover,fileExtCover));
for i=1:10000
%precover_path = fullfile('..', 'images_precover', '1.pgm');
    precover(i).path = imread(strcat(pathcover,Houzhui(i).name));
end
% load cover image
pathcover1='..\images_cover\';
pathcover2='..\images_stego\';

fileExtCover1='*.jpeg';
Houzhui1=dir(fullfile(pathcover1,fileExtCover1));
Houzhui2=dir(fullfile(pathcover2,fileExtCover1));
for i=1:10000
    cover_path(i).path = fullfile(strcat(pathcover1,Houzhui1(i).name));
    stego_path(i).path = fullfile(strcat(pathcover2,Houzhui2(i).name));
end
% set payload
payload = 0.1;
% set quality factor
quality_factor = 95;

fprintf('Embedding using Matlab file');

for i=1:10000
    SI_UNIWARD(precover(i).path, cover_path(i).path, stego_path(i).path, payload, quality_factor);
    C_STRUCT = jpeg_read(cover_path(i).path);
    C_SPATIAL = double(imread(cover_path(i).path));
    S_STRUCT = jpeg_read(stego_path(i).path);
    S_SPATIAL = double(imread(stego_path(i).path));

    nzAC = nnz(C_STRUCT.coef_arrays{1})-nnz(C_STRUCT.coef_arrays{1}(1:8:end,1:8:end));
end
fprintf(' - DONE');
