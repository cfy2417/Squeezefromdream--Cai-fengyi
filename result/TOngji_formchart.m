clear; clc; close all;

fprintf('\n=== Ledalab SCR 对照分析（TXT 文件版） ===\n');

%% ==== 1. 选择文件夹 ====
group1_path = uigetdir(pwd, '/Users/caifengyi/Documents/joystickin');
group2_path = uigetdir(pwd, '/Users/caifengyi/Documents/keboardin');

fprintf('\nGroup1 文件夹：%s\n', group1_path);
fprintf('Group2 文件夹：%s\n', group2_path);

%% ==== 2. 读取 Group1 ====
grp1_files = dir(fullfile(group1_path, '*.txt'));
group1_amp_all = [];
group1_onset_all = [];

for i = 1:length(grp1_files)
    filepath = fullfile(group1_path, grp1_files(i).name);
    data = readmatrix(filepath, 'FileType','text');

    % 自动判断列数
    if size(data,2) < 2
        warning('⚠ 文件 %s 数据列不足，跳过', grp1_files(i).name);
        continue;
    end

    group1_onset = data(:,1);
    group1_amp = data(:,2);

    group1_onset_all = [group1_onset_all; group1_onset];
    group1_amp_all = [group1_amp_all; group1_amp];
end

fprintf('\n✅ Group1 导入完成：%d 个文件，总计 %d 条记录\n', ...
    length(grp1_files), length(group1_amp_all));

%% ==== 3. 读取 Group2 ====
grp2_files = dir(fullfile(group2_path, '*.txt'));
group2_amp_all = [];
group2_onset_all = [];

for i = 1:length(grp2_files)
    filepath = fullfile(group2_path, grp2_files(i).name);
    data = readmatrix(filepath, 'FileType','text');

    if size(data,2) < 2
        warning('⚠ 文件 %s 数据列不足，跳过', grp2_files(i).name);
        continue;
    end

    group2_onset = data(:,1);
    group2_amp = data(:,2);

    group2_onset_all = [group2_onset_all; group2_onset];
    group2_amp_all = [group2_amp_all; group2_amp];
end

fprintf('✅ Group2 导入完成：%d 个文件，总计 %d 条记录\n', ...
    length(grp2_files), length(group2_amp_all));

%% ==== 4. 描述性统计 ====
fprintf('\n===== 描述性统计 =====\n');
fprintf('Group1 平均幅值: %.4f ± %.4f\n', mean(group1_amp_all), std(group1_amp_all));
fprintf('Group2 平均幅值: %.4f ± %.4f\n', mean(group2_amp_all), std(group2_amp_all));
fprintf('Group1 平均Onset: %.4f ± %.4f\n', mean(group1_onset_all), std(group1_onset_all));
fprintf('Group2 平均Onset: %.4f ± %.4f\n', mean(group2_onset_all), std(group2_onset_all));

%% ==== 5. 可视化 ====

% 箱型图
figure('Name','SCR Amplitude Comparison');
boxplot([group1_amp_all; group2_amp_all], ...
        [ones(length(group1_amp_all),1); 2*ones(length(group2_amp_all),1)], ...
        'Labels', {'Group1','Group2'});
ylabel('SCR Amplitude (µS)');
title('SCR 幅值分布对比');
grid on;

% 直方图
figure('Name','Amplitude Histogram');
histogram(group1_amp_all, 'Normalization','probability'); hold on;
histogram(group2_amp_all, 'Normalization','probability');
legend('Group1','Group2');
xlabel('SCR Amplitude (µS)'); ylabel('Probability');
title('SCR 幅值分布');
grid on;

% 时间序列
figure('Name','Time Series of SCR');
plot(group1_onset_all, group1_amp_all, 'b.', 'DisplayName','Group1'); hold on;
plot(group2_onset_all, group2_amp_all, 'r.', 'DisplayName','Group2');
xlabel('Time (s)'); ylabel('SCR Amplitude (µS)');
legend show; title('SCR 时间序列');
grid on;

%% ==== 6. 统计检验 ====
fprintf('\n===== 统计检验 =====\n');

% 6.1 t检验
[~, p_ttest] = ttest2(group1_amp_all, group2_amp_all);
fprintf('t检验 (Amplitude) p值: %.5f\n', p_ttest);

% 6.2 非参数检验 (Mann–Whitney U)
p_ranksum = ranksum(group1_amp_all, group2_amp_all);
fprintf('Mann-Whitney 检验 (Amplitude) p值: %.5f\n', p_ranksum);

% 6.3 Onset 差异
p_ranksum_onset = ranksum(group1_onset_all, group2_onset_all);
fprintf('Mann-Whitney 检验 (Onset) p值: %.5f\n', p_ranksum_onset);

%% ==== 7. 汇总结果 ====
summary_table = table( ...
    [mean(group1_amp_all); mean(group2_amp_all)], ...
    [std(group1_amp_all); std(group2_amp_all)], ...
    [mean(group1_onset_all); mean(group2_onset_all)], ...
    [std(group1_onset_all); std(group2_onset_all)], ...
    'VariableNames', {'MeanAmp','StdAmp','MeanOnset','StdOnset'}, ...
    'RowNames', {'Group1','Group2'} );

disp('===== 指标汇总 =====');
disp(summary_table);

%% ==== 8. 保存结果 ====
save('SCR_Txt_Comparison_Result.mat','group1_amp_all','group2_amp_all','group1_onset_all','group2_onset_all','summary_table');
fprintf('\n✅ 分析完成，结果已保存为 SCR_Txt_Comparison_Result.mat\n');