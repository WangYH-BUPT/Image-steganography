function Extract_Cover_func()
    clc;
    clear all;
%     for afa = 0.001:0.001:0.01
%         dircover = strcat('E:/Inter_block_coefficient/cover_sel_75/',num2str(afa),'/');
        dircover = strcat('D:\Matlab\bin\SI-UNIWARD_matlab\images_cover\');
        files=dir([dircover,'*.JPEG']); %遍历路径下所有JPG图像    
        num=length(files); 
        F = zeros(num,548);
        parfor i=1:num
            if files(i).isdir==0
                filename=[dircover '\' files(i).name];
                fname={fullfile(filename)}; 
                F(i,:)=ccpev548(filename, 75);
                fprintf(['cover ' 'processing: %s\n'],files(i).name);
            end
        end
        names=cell(num,1);
        for j=1:num
            names(j,1)={files(j).name};
        end
        save(['D:\SIUNIWARD-CCPEV-75\' strcat('cover75','.mat')],'names','F');
%     end
end


