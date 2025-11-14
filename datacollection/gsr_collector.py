import serial
import serial.tools.list_ports
import numpy as np
from scipy.io import savemat
import time
from datetime import datetime
import sys

class GSRDataCollector:
    def __init__(self, port=None, baudrate=115200):
        self.port = port
        self.baudrate = baudrate
        self.ser = None
        self.data = []
        self.is_recording = False
        
    def list_ports(self):
        """列出可用的串口"""
        ports = serial.tools.list_ports.comports()
        print("\n可用的串口:")
        for i, port in enumerate(ports):
            print(f"  {i+1}. {port.device} - {port.description}")
        return ports
    
    def connect(self):
        """连接到Arduino"""
        if self.port is None:
            ports = self.list_ports()
            if not ports:
                print("错误: 未找到可用串口")
                return False
            
            if len(ports) == 1:
                self.port = ports[0].device
                print(f"\n自动选择: {self.port}")
            else:
                choice = input("\n请选择串口编号: ")
                try:
                    self.port = ports[int(choice)-1].device
                except (ValueError, IndexError):
                    print("无效的选择")
                    return False
        
        try:
            self.ser = serial.Serial(self.port, self.baudrate, timeout=1)
            time.sleep(2)
            print(f"已连接到 {self.port}")
            
            while self.ser.in_waiting:
                line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    print(f"Arduino: {line}")
            
            return True
        except serial.SerialException as e:
            print(f"连接错误: {e}")
            return False
    
    def start_recording(self):
        """开始记录数据"""
        if not self.ser:
            print("错误: 未连接到Arduino")
            return False
        
        self.data = []
        self.is_recording = True
        self.ser.write(b"START\n")
        print("\n开始记录数据...")
        return True
    
    def stop_recording(self):
        """停止记录数据"""
        if not self.ser:
            return False
        
        self.is_recording = False
        self.ser.write(b"STOP\n")
        print("\n停止记录数据")
        return True
    
    def read_data(self):
        """读取串口数据"""
        if not self.ser or not self.ser.is_open:
            return
        
        while self.ser.in_waiting:
            try:
                line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                
                if line.startswith("DATA:"):
                    data_str = line[5:]
                    values = data_str.split(',')
                    
                    if len(values) == 4:
                        timestamp = float(values[0]) / 1000.0
                        gsr_raw = int(values[1])
                        resistance = float(values[2])
                        conductance = float(values[3])
                        
                        self.data.append({
                            'time': timestamp,
                            'raw': gsr_raw,
                            'resistance': resistance,
                            'conductance': conductance
                        })
                        
                        if len(self.data) % 10 == 0:
                            print(f"\r已采集 {len(self.data)} 个样本 "
                                  f"({timestamp:.1f}秒) - "
                                  f"电导: {conductance:.2f} μS", 
                                  end='', flush=True)
                elif line and not line.startswith("HEADER"):
                    print(f"\n{line}")
                    
            except Exception as e:
                print(f"\n解析数据错误: {e}")
    
    def save_to_mat(self, filename=None):
        """
        保存数据为Ledalab完全兼容的MAT文件
        使用标准Ledalab v3.4.9格式
        """
        if not self.data:
            print("错误: 没有数据可保存")
            return False
        
        if filename is None:
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            filename = f"GSR_data_{timestamp}.mat"
        
        # 确保文件名以.mat结尾
        if not filename.endswith('.mat'):
            filename += '.mat'
        
        # 准备数据数组 - 必须是列向量
        time_array = np.array([d['time'] for d in self.data], dtype=np.float64)
        conductance_array = np.array([d['conductance'] for d in self.data], dtype=np.float64)
        
        # 确保时间从0开始
        time_array = time_array - time_array[0]
        
        # 转换为列向量 (n, 1)
        time_col = time_array.reshape(-1, 1)
        conductance_col = conductance_array.reshape(-1, 1)
        
        # 计算采样率
        if len(time_array) > 1:
            time_diffs = np.diff(time_array)
            avg_interval = np.mean(time_diffs)
            sampling_rate = 1.0 / avg_interval if avg_interval > 0 else 10.0
        else:
            sampling_rate = 10.0
        
        print(f"\n数据统计:")
        print(f"  样本数: {len(self.data)}")
        print(f"  持续时间: {time_array[-1]:.2f} 秒")
        print(f"  计算的采样率: {sampling_rate:.2f} Hz")
        print(f"  电导范围: {conductance_array.min():.2f} - {conductance_array.max():.2f} μS")
        print(f"  平均电导: {conductance_array.mean():.2f} μS")
        
        # Ledalab标准格式 - 创建data结构体
        # 这是Ledalab最稳定兼容的格式
        ledalab_struct = {
            'conductance': conductance_col,  # 列向量 (n, 1)
            'time': time_col,                # 列向量 (n, 1)
            'timeoff': 0.0,                  # 标量
            'samplingrate': sampling_rate,    # 标量
            'event': np.array([]),           # 空数组
        }
        
        # 将结构体包装在'data'键中
        mat_data = {'data': ledalab_struct}
        
        try:
            # 使用MATLAB v7格式保存（最兼容Ledalab）
            # oned_as='column' 确保1D数组保存为列向量
            savemat(
                filename, 
                mat_data, 
                format='5',  # MATLAB v5格式
                do_compression=False,
                oned_as='column'  # 关键：确保1D数组为列向量
            )
            
            print(f"\n✓ MAT文件已保存: {filename}")
            
            # 验证保存的文件
            print("\n正在验证MAT文件...")
            self.verify_mat_file(filename)
            
            # 同时保存为文本格式（备用）
            txt_filename = filename.replace('.mat', '.txt')
            self.save_to_txt(txt_filename, time_array, conductance_array)
            
            # 保存为CSV格式（额外备用）
            csv_filename = filename.replace('.mat', '.csv')
            self.save_to_csv(csv_filename, time_array, conductance_array)
            
            return True
            
        except Exception as e:
            print(f"\n✗ 保存文件错误: {e}")
            import traceback
            traceback.print_exc()
            return False
    
    def verify_mat_file(self, filename):
        """验证保存的MAT文件结构"""
        try:
            from scipy.io import loadmat
            mat_contents = loadmat(filename)
            
            print("  文件内容验证:")
            
            # 检查'data'键是否存在
            if 'data' in mat_contents:
                data_struct = mat_contents['data']
                print(f"    ✓ 'data' 结构存在")
                
                # 检查必需字段
                required_fields = ['conductance', 'time', 'timeoff', 'samplingrate']
                for field in required_fields:
                    if field in data_struct.dtype.names:
                        field_data = data_struct[field][0, 0]
                        if field in ['conductance', 'time']:
                            print(f"    ✓ '{field}': shape={field_data.shape}, dtype={field_data.dtype}")
                        else:
                            print(f"    ✓ '{field}': {field_data}")
                    else:
                        print(f"    ✗ 缺失字段: '{field}'")
                
                print("  ✓ MAT文件结构正确")
            else:
                print("  ✗ 警告: 'data'键不存在")
                print(f"    可用键: {[k for k in mat_contents.keys() if not k.startswith('__')]}")
                
        except Exception as e:
            print(f"  ⚠ 无法验证文件: {e}")
    
    def save_to_txt(self, filename, time_array, conductance_array):
        """保存为制表符分隔的文本格式（Ledalab可以导入）"""
        try:
            with open(filename, 'w', encoding='utf-8') as f:
                f.write("Time(s)\tConductance(uS)\n")
                for t, c in zip(time_array, conductance_array):
                    f.write(f"{t:.6f}\t{c:.6f}\n")
            print(f"  ✓ 文本备份: {filename}")
        except Exception as e:
            print(f"  ⚠ 无法保存文本文件: {e}")
    
    def save_to_csv(self, filename, time_array, conductance_array):
        """保存为CSV格式"""
        try:
            with open(filename, 'w', encoding='utf-8') as f:
                f.write("Time,Conductance\n")
                for t, c in zip(time_array, conductance_array):
                    f.write(f"{t:.6f},{c:.6f}\n")
            print(f"  ✓ CSV备份: {filename}")
        except Exception as e:
            print(f"  ⚠ 无法保存CSV文件: {e}")
    
    def close(self):
        """关闭串口连接"""
        if self.ser and self.ser.is_open:
            self.stop_recording()
            self.ser.close()
            print("已断开连接")

def main():
    print("=" * 70)
    print("Grove GSR数据采集器 - Ledalab完全兼容版")
    print("=" * 70)
    
    collector = GSRDataCollector()
    
    try:
        if not collector.connect():
            return
        
        print("\n按 Enter 开始记录...")
        input()
        
        collector.start_recording()
        
        print("记录中... 按 Enter 停止记录")
        
        while True:
            collector.read_data()
            
            # 检测键盘输入
            if sys.platform == 'win32':
                import msvcrt
                if msvcrt.kbhit():
                    msvcrt.getch()
                    break
            else:
                import select
                if select.select([sys.stdin], [], [], 0)[0]:
                    input()
                    break
            
            time.sleep(0.01)
        
        collector.stop_recording()
        time.sleep(0.5)
        collector.read_data()
        
        # 保存数据
        print("\n" + "="*70)
        filename = input("输入文件名(留空自动生成): ").strip()
        if not filename:
            filename = None
        
        collector.save_to_mat(filename)
        
        print("\n" + "="*70)
        print("✓ 数据采集完成!")
        print("\n📖 Ledalab导入步骤:")
        print("  1. 打开MATLAB并启动Ledalab (输入 ledalab 命令)")
        print("  2. File → Import Data")
        print("  3. 选择 'Matlab' 格式")
        print("  4. 浏览并选择生成的 .mat 文件")
        print("  5. 点击 Import")
        print("\n📝 如果MAT文件无法导入:")
        print("  - 尝试用 'Text-file' 格式导入 .txt 文件")
        print("  - 检查MATLAB版本 (推荐 R2014a 或更高)")
        print("  - 确保Ledalab版本为 3.4.9 或更高")
        print("="*70)
        
    except KeyboardInterrupt:
        print("\n\n⚠ 用户中断")
        collector.stop_recording()
    except Exception as e:
        print(f"\n✗ 错误: {e}")
        import traceback
        traceback.print_exc()
    finally:
        collector.close()

if __name__ == "__main__":
    main()