#!/usr/bin/env python3
import serial, sys, time
port = '/dev/ttyACM1'
ser = serial.Serial(port, 9600, timeout=None)
def send_stop():
    if not ser.is_open:
    	ser.open()
    ser.write(b's')  
    print("Sent 's' to trigger MCU reset")

def parse_document(lines):
    # 将文档行解析为集合，只使用第一个空格前的内容
    data = set()
    for line in lines:
        parts = line.strip().split()
        if parts:  # 确保行不为空
            data.add(parts[0])  # 只取第一个空格前的完整20位字符串
    return data

def compare_documents(file1_path, file2_path):
    # 读取两个文件
    try:
        with open(file1_path, 'r') as f1:
            doc1_lines = f1.readlines()
        with open(file2_path, 'r') as f2:
            doc2_lines = f2.readlines()
    except FileNotFoundError as e:
        print(f"Error：cannot find file {e.filename}")
        return None, None
    except Exception as e:
        print(f"Error when reading file：{str(e)}")
        return None, None

    # 解析两个文档
    doc1_data = parse_document(doc1_lines)
    doc2_data = parse_document(doc2_lines)
    
    # 找出不匹配的条目
    mismatches = doc1_data - doc2_data  # 在doc1中有但在doc2中没有的条目
    mismatch_count = len(mismatches)
    
    return mismatch_count, mismatches

def main():
    # 获取文件路径
    file1_path = input("Please give the first file(include path): ")
    file2_path = input("Please give the second file(include path): ")
    
    # 比较文档
    mismatch_count, mismatched_entries = compare_documents(file1_path, file2_path)
    
    if mismatch_count is not None and mismatch_count != 0:
        # 输出结果
        print(f"\nnumbers of mismatch: {mismatch_count}")
        if mismatched_entries:
            print("\nmismatch details:")
            for entry in sorted(mismatched_entries):  # 按顺序显示
                print(entry)
        send_stop()
    elif mismatch_count == 0:
    	print(f"\ndata in file 1 is as same as data in file 2")
    elif mismatch_count is None:
    	print(f"\nError: mismatch_count is None!!!") 
if __name__ == "__main__":
    main()
