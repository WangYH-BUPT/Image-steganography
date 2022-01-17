### (22,510-D) CC-JRM特征

通过Extracting.m调用CCJRM.m提取CC-JRM特征

**需要改的地方是：**

1. 第二行JPEG_Toolbox的路径

	addpath(fullfile('..','JPEG_Toolbox'));

2. 第三行质量因子QF = 75, 85, 95

	QF=95;

3. 第四行images_stego的路径

	FILE1=fullfile('..','images_stego');

4. 第九行迭代次数根据数据集而变化

	for i=1:10000