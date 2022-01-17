clc; clear all; close all;
addpath(fullfile('..','JPEG_Toolbox'));
QF=95;
FILE1=fullfile('..','images_stego');
fileExtCover='*.JPEG';
Houzhui=dir(fullfile(FILE1,fileExtCover));
F=[];
names = cell(10000,1);
for i=1:10000
    FILE=fullfile(FILE1,Houzhui(i).name);
    C(i).data=CCJRM(FILE,QF);
    F=[F;C(i).data];
    name=strcat(num2str(i),'.jpeg');
    names(i,1)=cellstr(name); %%ÀàÐÍ×ª»»char----> cell
    i
end
stego_050.F=F;
stego_050.names=names;
save stego_050;
fprintf(' - DONE');