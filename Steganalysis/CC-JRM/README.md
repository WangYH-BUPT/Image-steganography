### (22,510-D) CC-JRM 论文

> J. Kodovský, and J. Fridrich, “Steganalysis of JPEG Images Using Rich Models,” in Proceedings of SPIE, Electronic Imaging, Media Watermarking, Security, and Forensics XIV, vol. 8303, no. 83030, 2012.

### (22,510-D) CC-JRM特征

通过`Extracting.m`调用`CCJRM.m`提取CC-JRM特征

**需要改的地方是：**

1. 第二行`JPEG_Toolbox`的路径

		addpath(fullfile('..','JPEG_Toolbox'));

2. 第三行质量因子`QF` = 75, 85, 95

		QF=95;

3. 第四行`images_stego`的路径

		FILE1=fullfile('..','images_stego');

4. 第九行迭代次数`i`根据数据集而变化

		for i=1:10000
