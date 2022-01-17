### SI-UNIWARD 论文

> V. Holub, J. Fridrich, and T. Denemark, “Universal Distortion Function for Steganography in an Arbitrary Domain,” EURASIP Journal on Information Security, vol. 1, no. 1, pp. 1-13, 2014.


通过 Embedding.m 调用 SI-UNIWARD.m 嵌入。

**需要改的地方是：**

1. 第四行 `JPEG_Toolbox` 的路径

		addpath(fullfile('D:\Matlab\bin\JPEG_Toolbox'));

2. 第七行 `pathcover` 的路径

		pathcover = '..\images_precover\';

3. 第十五十六行 `pathcover1` 和 `pathcover2` 的路径

		pathcover1 = '..\images_cover\';
		pathcover2 = '..\images_stego\';

4. 第二十六行嵌入率 `payload` = 0.1, 0.2, 0.25, 0.3, 0.4, 0.5, 0.75, 1.0, ...

		payload = 0.1;

5. 第二十八行质量因子 `quality_factor` = 75, 85, 95

		quality_factor = 95;


