% Code by WangYH-BUPT
clc; clear;
a = load("D:\Matlab\bin\stego_010.mat");
F1 = cell2mat(struct2cell(a.F));
F = F1';
names = a.names;
save stego_010.mat F names;
