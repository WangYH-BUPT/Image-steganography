### (548-D) CC-PEV 论文

> 1. T. Pevný, and J. Fridrich, “Merging Markov and DCT Features for Multiclass JPEG Steganalysis,” in Proceedings of SPIE, Electronic Imaging, Security, Steganography, and Watermarking of Multimedia Contents IX, pp. 1-14, 2007.

> 2. J. Kodovský, and J. Fridrich, “Calibration Revisited,” in Proceedings of
the 11th ACM Multimedia and Security Workshop, pp. 63-74, 2009.

### (548-D) CC-PEV 特征

通过 Extract\_Cover\_func.m 和 Extract\_Stego\_func.m 调用 ccpev548.m 提取 CC-PEV 特征 (cover: payload = 0; stego: payload != 0)

**需要改的地方是：**

***Extract\_Cover_func.m***

1. 第六行 `images_cover` 的路径

		dircover = strcat('D:\Matlab\bin\SI-UNIWARD_matlab\images_cover\');

2. 第七行 `files` 的图片格式

		files = dir([dircover, '*.JPEG']);

3. 第二十二行提取特征保存的路径

		save(['D:\SIUNIWARD-CCPEV-75\' strcat('cover75','.mat')],'names','F');

##

***Extract\_Stego_func.m***

1. 第六行 `images_stego` 的路径

		dirstego = strcat('D:\Matlab\bin\SI-UNIWARD_matlab\images_stego\');

2. 第七行 `files` 的图片格式

		files = dir([dirstego, '*.JPEG']);

3. 第十四行质量因子75, 85, 95

		F(i,:) = ccpev548(filename, 75);

3. 第二十二行提取特征保存的路径

		save(['D:\SIUNIWARD-CCPEV-75\' strcat('cover75', '.mat')], 'names', 'F');


