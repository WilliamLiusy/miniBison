import subprocess
import os

grammar_name = ["add_minus", "add_multiply", "simple_language"]
instance_indices = [range(1, 15), range(1, 15), range(1, 15)]

def main():

    # 循环运行C程序，instance从1到14
    for grammar_index in range(len(grammar_name)):
        print("----------------------------------------------------------------------------------\n")
        print(f"running instances of grammar: {grammar_name[grammar_index]}\n")
        # 硬编码传递给C程序的参数
        grammar_file = f"./test_{grammar_name[grammar_index]}/test_data/grammar_{grammar_name[grammar_index]}.txt"  # 语法文件
        input_file_prefix = f"./test_{grammar_name[grammar_index]}/test_data/{grammar_name[grammar_index]}_instance"  # 输入文件前缀
        output_file_prefix = f"./test_{grammar_name[grammar_index]}/test_data/{grammar_name[grammar_index]}_instance"  # 输出文件前缀
        for instance in instance_indices[grammar_index]:  # instance从1到14
            input_file = f"{input_file_prefix}{instance}_input.txt"  # 输入文件名
            output_file = f"{output_file_prefix}{instance}_output.txt"  # 输出文件名

            try:
                # 调用C程序
                subprocess.run(
                    ["./generator", grammar_file, input_file, output_file],
                    check=True  # 如果C程序返回非零值，抛出异常
                )
                print(f"C program executed successfully. Output written to {output_file}\n")
            except subprocess.CalledProcessError:
                # 如果C程序失败，创建一个空白的文件
                with open(output_file, "w") as f:
                    pass  # 创建空文件
                print(f"C program failed. Created an empty file at {output_file}.\n")
            except FileNotFoundError:
                # 如果C程序不存在，提示用户
                print(f"Error: C program 'generator' not found. Please ensure it is compiled and available.")
                break  # 退出循环
        print("----------------------------------------------------------------------------------\n")

        print("\n")


if __name__ == "__main__":
    main()