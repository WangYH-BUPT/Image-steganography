### (8,000-D) DCTR论文

> V. Holub, and J. Fridrich, “Low Complexity Features for JPEG Steganalysis Using Undecimated DCT,” IEEE Transactions on Information Forensics and Security, vol. 10, no. 2, pp. 219-228, 2015.

### (8,000-D) DCTR特征

通过 example.m 调用 DCTR.m 提取 DCTR 特征

**需要改的地方是：**

1. 第二行 `JPEG_Toolbox` 的路径

		addpath(fullfile('D:\Matlab\bin\JPEG_Toolbox'));

2. 第四行 `images_stego` 的路径

		filepath_stego = strcat('D:\Matlab\bin\images_stego\');

3. 第五行迭代次数 `imagenum` 根据数据集而变化

		imagenum = 10000; Dim = 8000;

4. 第九行质量因子 `quality_factor` = 75, 85, 95

		quality_factor = 95;



