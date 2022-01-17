function Extract_Stego_func()
    clc;
    clear all;
    for afa = 0.1
        %afa = 0.01:0.01:0.1
        dirstego = strcat('D:\Matlab\bin\SI-UNIWARD_matlab\images_stego\');
        files=dir([dirstego,'*.JPEG']); %遍历路径下所有JPG图像    
        num=length(files); 
        F=zeros(num,548);
        parfor i=1:num 
            if files(i).isdir==0
                filename = [dirstego '\' files(i).name];
                fprintf(['stego ' 'processing: %s\n'],files(i).name);
                F(i,:)=ccpev548(filename, 75);       
            end
        end
        names=cell(num,1);
        for j=1:num
            names(j,1)={files(j).name};
        end
        save(['D:\Personal\科研\已经提取的特征、图像库\SIUNIWARD-CCPEV-75\' strcat('stego050','.mat')],'names','F');    
    end
end