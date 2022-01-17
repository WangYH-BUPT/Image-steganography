clc;clear;
a = load("D:\Matlab\bin\SI-UNIWARD_matlab\matlab\stego_01075.mat");
F1 = cell2mat(struct2cell(a.F));
F = F1';
names = a.names;
save stego_010.mat F names;