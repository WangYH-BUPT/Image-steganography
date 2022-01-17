### (17,000-D) GFR 论文

> X.-F. Song, F.-L. Liu, C.-F. Yang, et al., “Steganalysis of Adaptive JPEG Steganography Using 2D Gabor Filters,” in Proceedings of the 3rd ACM Workshop on Information Hiding and Multimedia Security, pp. 15-23, 2015.

### (17,000-D) GFR 特征

通过 Untitled.m 调用 GFR.m 提取 GFR 特征

**需要改的地方是：**

1. 第二行 `JPEG_Toolbox` 的路径

		addpath(fullfile('..', 'JPEG_Toolbox'));

2. 第三行质量因子 `QF` = 75, 85, 95

		QF = 95;

2. 第四行 `images_stego` 的路径

		FILE1 = fullfile('..', 'images_stego');

3. 第九行迭代次数 `i` 根据数据集而变化

		for i = 1: 10000





