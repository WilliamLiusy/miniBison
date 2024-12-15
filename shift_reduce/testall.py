import subprocess
import os

# 定义两个程序的路径
validation_path = "./validation.py"  # 第一个程序的路径

# 检查文件是否存在
if not os.path.exists(validation_path):
    print(f"Error: File not found: {validation_path}")
    exit(1)

# 在这里定义要验证的语法名称以及样例数量
grammar_name = ["add_minus", "add_multiply", "simple_language"]
# range(1,15) 生成 1 到 14 的整数，因此表示有 14 组样例
instance_indices = [range(1, 15), range(1, 15), range(1, 15)]

for grammar_index in range(len(grammar_name)):
    for instance_index in instance_indices[grammar_index]:
        print("\n")
        try:
            # 调用第一个程序，传递 instance_index 作为参数
            print(f"Running {validation_path} with grammar {grammar_name[grammar_index]} instance index {instance_index}")
            subprocess.run(["python3", validation_path, str(instance_index), grammar_name[grammar_index]], check=True)
            print(f"validation.py executed successfully with instance index {instance_index}")
        except subprocess.CalledProcessError as e:
            print(f"Error running program1.py with instance index {instance_index}: {e}")