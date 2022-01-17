
clc;clear all; close all; fclose all;
addpath(fullfile('D:\Personal\¿ÆÑÐ\´úÂë\J-UNIWARD\J-UNIWARD_matlab\','JPEG_Toolbox'));
all_payload=[0.1,0.2,0.3,0.4,0.5];
% for i_payload = 1:5
%     payload = single(all_payload(i_payload));
%     payload 
%% Settings
parfor n=1:10
payload = 0.1;   % measured in bits per non zero AC coefficients
%coverPath = fullfile('..','  images_cover', '1.jpg');
coverPath = strcat('D:\Matlab\bin\J-UNIWARD_matlab\J-UNIWARD_matlab\images_cover\',num2str(n),'.jpg');
stegoPath = strcat('D:\Matlab\bin\J-UNIWARD_matlab\J-UNIWARD_matlab\images_stego\',num2str(n),'.jpg');

tStart = tic;

      S_STRUCT = J_UNIWARD(coverPath, payload);
   %    stegoy = jpeg_read(stego_Y);
%          YCbCr_data(:,:,1)=uint8(segoy);
%          stego=ycbcr2rgb(YCbCr_data);


%% Embedding

%stego = J_UNIWARD(coverPath, payload);

%% Plots
tEnd = toc(tStart);
jpeg_write(S_STRUCT, stegoPath);
 
C_STRUCT = jpeg_read(coverPath);
C_SPATIAL = double(imread(coverPath));
S_SPATIAL = double(imread(stegoPath));
 
 nzAC = nnz(C_STRUCT.coef_arrays{1})-nnz(C_STRUCT.coef_arrays{1}(1:8:end,1:8:end));
% 
% figure;
% imshow(uint8(C_SPATIAL));
% title('Cover image');
% 
% figure;
% imshow(uint8(S_SPATIAL));
% title('Stego image');
% 
% figure; 
% diff = S_STRUCT.coef_arrays{1}~=C_STRUCT.coef_arrays{1};
% imshow(diff);
% title('Changes in DCT domain (in standard JPEG grid)');
% 
% figure;
% [row, col] = find(diff);
% row = mod(row-1, 8)+1;
% col = mod(col-1, 8)+1;
% vals = zeros(8, 8);
% for i=1:numel(row)
%     vals(row(i), col(i)) = vals(row(i), col(i))+1;
% end
% bar3(vals, 'detached');
% xlabel('cols');
% ylabel('rows');
% 
% figure;
% diff = S_SPATIAL-C_SPATIAL;
% cmap = colormap('Bone');
% imshow(diff, 'Colormap', cmap);
% c=colorbar('Location', 'SouthOutside');
% caxis([-20, 20]);
% title('Changes in spatial domain caused by DCT embedding');
% 
% figure;
% h = hist(diff(:), -20:1:20);
% bar(-20:1:20, h);
% set(gca, 'YScale', 'log');
% xlabel('pixel difference in spatial domain');
% ylabel('occurrences');
% title('Number of pixel differencees in spatial domain');
% 
% figure;
% hRange = -50:50;
% changePos = (C_STRUCT.coef_a1rrays{1}~=S_STRUCT.coef_arrays{1});
% coverChDCT = C_STRUCT.coef_arrays{1}(changePos);
% histAll = hist(C_STRUCT.coef_arrays{1}(:), hRange);
% histChanges = hist(coverChDCT(:), hRange);
% [AX, h1, h2] = plotyy(hRange(2:end-1), histChanges(2:end-1), hRange(2:end-1), histChanges(2:end-1) ./ histAll(2:end-1));
% set(get(AX(1),'Ylabel'),'String','Number of changed DCT coefficients');
% set(get(AX(2),'Ylabel'),'String','Probability of change');
% xlabel('DCT value');
% hold off;
% 
n
fprintf('\nElapsed time: %.4f s, change rate per nzAC: %.4f, nzAC: %d', tEnd, sum(S_STRUCT.coef_arrays{1}(:)~=C_STRUCT.coef_arrays{1}(:))/nzAC, nzAC);
end
