%% Ledalab数据导入修复工具
%% 此脚本修复数据格式，使其可以被Ledalab 3.4.9导入
%% 
%% 使用方法:
%%   1. 修改下面的 input_file 为你的文件路径
%%   2. 运行此脚本
%%   3. 会生成多个格式的文件供Ledalab导入

clc;
clear;
close all;

fprintf('========================================================\n');
fprintf('    Ledalab 3.4.9 数据导入修复工具\n');
fprintf('========================================================\n\n');

%% 配置区域 - 修改这里
% ============================
input_file = '/Users/caifengyi/Documents/keboardin/test_keyboard.csv';  % 修改为你的文件路径
% ============================

%% 检查文件是否存在
if ~exist(input_file, 'file')
    error('文件不存在: %s', input_file);
end

fprintf('原始文件: %s\n', input_file);
fprintf('文件大小: %.2f KB\n\n', dir(input_file).bytes/1024);

%% 步骤1: 加载和分析原始文件
fprintf('步骤 1/5: 加载原始数据...\n');
fprintf('--------------------------------------------------------\n');

try
    original_data = load(input_file);
    fprintf('✓ 文件加载成功\n\n');
catch ME
    error('无法加载文件: %s', ME.message);
end

% 显示文件内容
fprintf('原始文件包含的变量:\n');
fields = fieldnames(original_data);
for i = 1:length(fields)
    if ~startsWith(fields{i}, '__')
        fprintf('  %d. %s\n', i, fields{i});
        var = original_data.(fields{i});
        fprintf('     类型: %s\n', class(var));
        if isnumeric(var)
            fprintf('     大小: %s\n', mat2str(size(var)));
            fprintf('     范围: %.6f 到 %.6f\n', min(var(:)), max(var(:)));
        end
    end
end

%% 步骤2: 智能提取数据
fprintf('\n步骤 2/5: 提取电导和时间数据...\n');
fprintf('--------------------------------------------------------\n');

conductance = [];
time = [];

% 策略1: 尝试标准字段名
standard_data_names = {'data', 'conductance', 'Data', 'Conductance', 'y', 'signal'};
standard_time_names = {'time', 'Time', 't', 'x', 'timestamp'};

for i = 1:length(standard_data_names)
    if isfield(original_data, standard_data_names{i})
        conductance = original_data.(standard_data_names{i});
        fprintf('✓ 找到电导数据: %s\n', standard_data_names{i});
        break;
    end
end

for i = 1:length(standard_time_names)
    if isfield(original_data, standard_time_names{i})
        time = original_data.(standard_time_names{i});
        fprintf('✓ 找到时间数据: %s\n', standard_time_names{i});
        break;
    end
end

% 策略2: 如果没找到，使用第一个数值数组作为电导
if isempty(conductance)
    fprintf('⚠ 未找到标准字段名，尝试使用第一个数值数组...\n');
    for i = 1:length(fields)
        if ~startsWith(fields{i}, '__')
            var = original_data.(fields{i});
            if isnumeric(var) && ~isempty(var)
                conductance = var;
                fprintf('✓ 使用字段: %s\n', fields{i});
                break;
            end
        end
    end
end

% 策略3: 如果仍未找到时间，生成时间序列
if isempty(time) && ~isempty(conductance)
    fprintf('⚠ 未找到时间数据，假设采样率=10Hz生成时间序列\n');
    time = (0:length(conductance(:))-1)' * 0.1;
end

% 验证数据
if isempty(conductance)
    error('错误: 无法提取电导数据！');
end

if isempty(time)
    error('错误: 无法生成时间数据！');
end

%% 步骤3: 数据清理和标准化
fprintf('\n步骤 3/5: 数据清理...\n');
fprintf('--------------------------------------------------------\n');

% 转换为列向量
conductance = conductance(:);
time = time(:);

fprintf('原始数据:\n');
fprintf('  电导: %d 个样本\n', length(conductance));
fprintf('  时间: %d 个样本\n', length(time));

% 确保长度匹配
min_len = min(length(conductance), length(time));
if length(conductance) ~= length(time)
    fprintf('⚠ 长度不匹配，截取到 %d 个样本\n', min_len);
    conductance = conductance(1:min_len);
    time = time(1:min_len);
end

% 处理NaN值
nan_count = sum(isnan(conductance));
if nan_count > 0
    fprintf('⚠ 发现 %d 个NaN值，正在替换...\n', nan_count);
    conductance(isnan(conductance)) = nanmean(conductance);
end

% 处理无穷大值
inf_count = sum(isinf(conductance));
if inf_count > 0
    fprintf('⚠ 发现 %d 个无穷大值，正在替换...\n', inf_count);
    conductance(isinf(conductance)) = mean(conductance(~isinf(conductance)));
end

% 时间归零
time = time - time(1);

% 计算采样率
if length(time) > 1
    intervals = diff(time);
    avg_interval = mean(intervals);
    sampling_rate = 1 / avg_interval;
else
    sampling_rate = 10;
end

fprintf('\n清理后的数据统计:\n');
fprintf('  样本数: %d\n', length(conductance));
fprintf('  时间范围: %.3f - %.3f 秒\n', time(1), time(end));
fprintf('  持续时间: %.2f 秒\n', time(end));
fprintf('  平均采样间隔: %.6f 秒\n', avg_interval);
fprintf('  估计采样率: %.2f Hz\n', sampling_rate);
fprintf('  电导范围: %.6f - %.6f μS\n', min(conductance), max(conductance));
fprintf('  电导均值: %.6f μS\n', mean(conductance));
fprintf('  电导标准差: %.6f μS\n', std(conductance));

%% 步骤4: 生成多种Ledalab兼容格式
fprintf('\n步骤 4/5: 生成Ledalab兼容文件...\n');
fprintf('--------------------------------------------------------\n');

[filepath, name, ~] = fileparts(input_file);

% 格式1: 标准Ledalab格式 (最推荐)
fprintf('\n生成格式1: 标准格式 (data + time)...\n');
clear ledalab_format1;
ledalab_format1.data = conductance;
ledalab_format1.time = time;

file1 = fullfile(filepath, [name '_ledalab_v1.mat']);
save(file1, '-struct', 'ledalab_format1', '-v7');
fprintf('  ✓ %s\n', file1);

% 格式2: 带事件标记
fprintf('\n生成格式2: 带事件标记 (data + time + event)...\n');
clear ledalab_format2;
ledalab_format2.data = conductance;
ledalab_format2.time = time;
ledalab_format2.event = [];  % 空事件数组

file2 = fullfile(filepath, [name '_ledalab_v2.mat']);
save(file2, '-struct', 'ledalab_format2', '-v7');
fprintf('  ✓ %s\n', file2);

% 格式3: 完整Ledalab格式
fprintf('\n生成格式3: 完整格式 (conductance + time + timeoff)...\n');
clear ledalab_format3;
ledalab_format3.conductance = conductance;
ledalab_format3.time = time;
ledalab_format3.timeoff = 0;

file3 = fullfile(filepath, [name '_ledalab_v3.mat']);
save(file3, '-struct', 'ledalab_format3', '-v7');
fprintf('  ✓ %s\n', file3);

% 格式4: 纯数据列（单列电导）
fprintf('\n生成格式4: 纯数据列 (仅电导)...\n');
clear ledalab_format4;
ledalab_format4.data = conductance;

file4 = fullfile(filepath, [name '_ledalab_v4.mat']);
save(file4, '-struct', 'ledalab_format4', '-v7');
fprintf('  ✓ %s\n', file4);

% 文本格式1: 两列 (时间 + 电导)
fprintf('\n生成文本格式1: 两列 (Tab分隔)...\n');
txt_file1 = fullfile(filepath, [name '_ledalab_2col.txt']);
fid = fopen(txt_file1, 'w');
for i = 1:length(time)
    fprintf(fid, '%.6f\t%.6f\n', time(i), conductance(i));
end
fclose(fid);
fprintf('  ✓ %s\n', txt_file1);

% 文本格式2: 两列带表头
fprintf('\n生成文本格式2: 带表头 (Tab分隔)...\n');
txt_file2 = fullfile(filepath, [name '_ledalab_header.txt']);
fid = fopen(txt_file2, 'w');
fprintf(fid, 'Time(s)\tConductance(uS)\n');
for i = 1:length(time)
    fprintf(fid, '%.6f\t%.6f\n', time(i), conductance(i));
end
fclose(fid);
fprintf('  ✓ %s\n', txt_file2);

% 文本格式3: 单列（仅电导）
fprintf('\n生成文本格式3: 单列数据...\n');
txt_file3 = fullfile(filepath, [name '_ledalab_1col.txt']);
fid = fopen(txt_file3, 'w');
for i = 1:length(conductance)
    fprintf(fid, '%.6f\n', conductance(i));
end
fclose(fid);
fprintf('  ✓ %s\n', txt_file3);

%% 步骤5: 数据可视化验证
fprintf('\n步骤 5/5: 数据可视化验证...\n');
fprintf('--------------------------------------------------------\n');

figure('Name', 'GSR Data Verification', 'Position', [100, 100, 1200, 800]);

% 子图1: 完整信号
subplot(3,1,1);
plot(time, conductance, 'b-', 'LineWidth', 1.5);
xlabel('Time (s)', 'FontSize', 12);
ylabel('Conductance (μS)', 'FontSize', 12);
title('Complete GSR Signal', 'FontSize', 14, 'FontWeight', 'bold');
grid on;

% 子图2: 前20秒详细视图
subplot(3,1,2);
if time(end) > 20
    idx = time <= 20;
    plot(time(idx), conductance(idx), 'r-', 'LineWidth', 1.5);
    title('First 20 Seconds (Detail View)', 'FontSize', 14, 'FontWeight', 'bold');
else
    plot(time, conductance, 'r-', 'LineWidth', 1.5);
    title('Complete Signal (< 20s)', 'FontSize', 14, 'FontWeight', 'bold');
end
xlabel('Time (s)', 'FontSize', 12);
ylabel('Conductance (μS)', 'FontSize', 12);
grid on;

% 子图3: 数据分布直方图
subplot(3,1,3);
histogram(conductance, 50, 'FaceColor', [0.3 0.7 0.9]);
xlabel('Conductance (μS)', 'FontSize', 12);
ylabel('Frequency', 'FontSize', 12);
title('Data Distribution', 'FontSize', 14, 'FontWeight', 'bold');
grid on;

fprintf('✓ 可视化完成\n');

%% 总结和导入说明
fprintf('\n========================================================\n');
fprintf('                  处理完成!\n');
fprintf('========================================================\n\n');

fprintf('已生成以下文件:\n');
fprintf('  MAT格式:\n');
fprintf('    1. %s (推荐首选)\n', file1);
fprintf('    2. %s (备选)\n', file2);
fprintf('    3. %s (备选)\n', file3);
fprintf('    4. %s (单列)\n', file4);
fprintf('  文本格式:\n');
fprintf('    5. %s (无表头)\n', txt_file1);
fprintf('    6. %s (有表头)\n', txt_file2);
fprintf('    7. %s (单列)\n', txt_file3);

fprintf('\n========================================================\n');
fprintf('             Ledalab导入步骤\n');
fprintf('========================================================\n\n');

fprintf('【方法1】导入MAT文件 (推荐):\n');
fprintf('  1. 在MATLAB命令窗口输入: Ledalab\n');
fprintf('  2. File → Import Data → Matlab\n');
fprintf('  3. 选择文件: %s\n', [name '_ledalab_v1.mat']);
fprintf('  4. 点击打开\n\n');

fprintf('【方法2】命令行导入:\n');
fprintf('  >> Ledalab(''%s'')\n\n', file1);

fprintf('【方法3】导入文本文件:\n');
fprintf('  1. 启动Ledalab\n');
fprintf('  2. File → Import Data → Text/Ascii\n');
fprintf('  3. 选择文件: %s\n', [name '_ledalab_2col.txt']);
fprintf('  4. 设置参数:\n');
fprintf('     - Number of columns: 2\n');
fprintf('     - Time column: 1\n');
fprintf('     - Data column: 2\n');
fprintf('     - Skip rows: 0\n');
fprintf('     - Column delimiter: Tab\n');
fprintf('     - Sampling rate: %.2f Hz\n\n', sampling_rate);

fprintf('【方法4】如果仍然失败 - 尝试单列格式:\n');
fprintf('  1. 启动Ledalab\n');
fprintf('  2. File → Import Data → Text/Ascii\n');
fprintf('  3. 选择文件: %s\n', [name '_ledalab_1col.txt']);
fprintf('  4. 设置参数:\n');
fprintf('     - Number of columns: 1\n');
fprintf('     - Skip rows: 0\n');
fprintf('     - Sampling rate: %.2f Hz (手动输入)\n\n', sampling_rate);

fprintf('========================================================\n');
fprintf('                 验证检查\n');
fprintf('========================================================\n\n');

fprintf('导入后，在Ledalab中应该看到:\n');
fprintf('  ✓ 信号波形图正常显示\n');
fprintf('  ✓ 样本数: %d\n', length(conductance));
fprintf('  ✓ 持续时间: %.2f 秒\n', time(end));
fprintf('  ✓ 电导范围合理 (%.2f - %.2f μS)\n\n', min(conductance), max(conductance));

fprintf('如果导入成功，你可以:\n');
fprintf('  1. 进行预处理 (Preprocessing)\n');
fprintf('  2. 运行分解分析 (Analysis → CDA)\n');
fprintf('  3. 导出结果 (File → Export)\n\n');

fprintf('========================================================\n');
fprintf('             故障排除\n');
fprintf('========================================================\n\n');

fprintf('如果所有方法都失败:\n\n');

fprintf('1. 在MATLAB中手动验证文件:\n');
fprintf('   >> test_data = load(''%s'');\n', file1);
fprintf('   >> whos(''-file'', ''%s'')\n', file1);
fprintf('   >> plot(test_data.time, test_data.data)\n\n');

fprintf('2. 检查Ledalab版本:\n');
fprintf('   应该是 3.4.9 或更新版本\n\n');

fprintf('3. 尝试Ledalab示例数据:\n');
fprintf('   导入Ledalab安装目录下的example文件\n');
fprintf('   如果示例数据可以导入，说明是你的数据格式问题\n\n');

fprintf('4. 确认Ledalab路径:\n');
fprintf('   >> which Ledalab\n');
fprintf('   应该显示Ledalab.m的路径\n\n');

fprintf('========================================================\n');
fprintf('完成! 请尝试上述方法导入数据。\n');
fprintf('========================================================\n\n');

fprintf('数据图形已显示，请检查信号是否正常。\n');
fprintf('关闭图形窗口后可以继续操作。\n\n');