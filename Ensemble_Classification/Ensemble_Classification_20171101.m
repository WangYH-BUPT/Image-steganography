%Ö÷³ÌÐò
clc;clear all;
cover = load('D:\Personal\SIUNIWARD-DCTR-95\cover.mat');
stego = load('D:\Personal\SIUNIWARD-DCTR-95\stego050.mat');
load('D:\Personal\columns.mat');
num = length(columns);
% num = 1;
results_errors = zeros(num,2);
for x = 1:10
for i = 1:num
    indexcolumn = columns{i};
    if isempty(indexcolumn)
        break;
    end
    c = cover.F;
    s = stego.F;
   C = c(:,indexcolumn);
   S = s(:,indexcolumn);
   %  C = cover.F;
   %  S = stego.F;
    names = cover.names;
    N = 3;
    testing_errors = zeros(1,N);
    for seed = 1:N
        
        RandStream.setGlobalStream(RandStream('mt19937ar','Seed',seed));
        
        random_permutation = randperm(size(C,1));
        training_set = random_permutation(1:round(size(C,1)/2));
        testing_set = random_permutation(round(size(C,1)/2)+1:end);
        training_names = names(training_set);
        testing_names = names(testing_set);
        
        TRN_cover = C(training_set,:);
        TRN_stego = S(training_set,:);
        
        TST_cover = C(testing_set,:);
        TST_stego = S(testing_set,:);
        
        [trained_ensemble,results] = ensemble_training(TRN_cover,TRN_stego);
        
        test_results_cover = ensemble_testing(TST_cover,trained_ensemble);
        test_results_stego = ensemble_testing(TST_stego,trained_ensemble);
        
        false_alarms = sum(test_results_cover.predictions~=-1);
        missed_detections = sum(test_results_stego.predictions~=+1);
        num_testing_samples = size(TST_cover,1)+size(TST_stego,1);
        testing_errors(seed) = (false_alarms + missed_detections)/num_testing_samples;
        fprintf('%i/%i Testing error %i: %.4f\n',i, num, seed,testing_errors(seed));
    end
    fprintf('---\nAverage testing error over 2 splits: %.4f (+/- %.4f)\n',mean(testing_errors),std(testing_errors));
    results_errors(i,1) = mean(testing_errors); results_errors(i,2) = std(testing_errors);
end
results_errors(x) = results_errors(1,1);
disp(x)
end
results_errors = max(results_errors);
save('results_errors.mat','results_errors');