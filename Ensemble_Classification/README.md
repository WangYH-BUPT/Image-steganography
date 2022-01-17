### Ensemble_Classification 论文

Ensemble Classification是基于随机森林、蒙特卡洛思想的分类器，在隐写学表现得效果较好。

> J. Kodovský, J. Fridrich, and V. Holub, “Ensemble Classifiers for Steganalysis of Digital Media,” IEEE Transactions on Information Forensics and Security, vol. 7, no. 2, pp. 432-444, 2012.

### Ensemble_Classification 程序

通过 Ensemble\_Classification\_20171101.m 调用 ensemble\_testing.m 和 ensemble\_training.m

**需要改的地方是：**

1. 第三行 `cover` 的路径

		cover = load('D:\Personal\SIUNIWARD-DCTR-95\cover.mat');

2. 第四行 `stego` 的路径

		stego = load('D:\Personal\SIUNIWARD-DCTR-95\stego050.mat');

3. 第五行 `columns` 行数

		load('D:\Personal\columns.mat');

输出是十个分类错误率和一个平均的分类错误率。



