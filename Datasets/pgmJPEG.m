clc;clear;
for i=1:10000
picname1=[num2str(i),'.JPEG'];
pathfile1=['D:\Matlab\bin\SI-UNIWARD_matlab\images_cover\',picname1];
img=imread(pathfile1);
picname2=[num2str(i),'.jpeg'];
pathfile2=['D:\Matlab\bin\images_cover\',picname2];
imwrite(img,pathfile2,'jpeg');
i
end
