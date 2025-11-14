input_file = "/Users/caifengyi/Documents/keboardin/ledalab_events.txt"    # 原文件
output_file = "/Users/caifengyi/Desktop/ledalab_events.txt"  # 修正后文件

with open(input_file, 'r') as f_in, open(output_file, 'w') as f_out:
    # 写入列名
    f_out.write("Onset(s)\tDuration(s)\tEventName\n")
    
    for line in f_in:
        line = line.strip()
        if not line:
            continue
        # 将空格分隔改为制表符
        parts = line.split()  # 默认按空格分隔
        if len(parts) == 3:
            f_out.write(f"{parts[0]}\t{parts[1]}\t{parts[2]}\n")

print(f"已生成 Ledalab 可用文件：{output_file}")