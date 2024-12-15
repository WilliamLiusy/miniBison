import os
from prod import Prod
import sys

# 检查命令行参数
if len(sys.argv) != 3:
    print("Usage: python script.py <instance_index> <grammar_name>")
    exit(1)

# 从命令行参数获取 instance_index
try:
    instance_index = int(sys.argv[1])
    grammar_name = sys.argv[2]
except ValueError:
    print("Error: instance_index must be an integer.")
    exit(1)

# 获取当前脚本的目录
script_dir = os.path.dirname(os.path.abspath(__file__))

## read the prod table
grammar_file_path = os.path.join(script_dir, f"test_{grammar_name}","test_data", f"grammar_{grammar_name}.txt")
grammar = []
with open(grammar_file_path, "r") as f:
    try: 
        num_symbols, num_productions = map(int, f.readline().split())
        num_productions = num_productions - 1
        
        lines = f.readlines()
        
        for line in lines :
            num_right, left, *right = map(int, line.split())
            
            assert num_right == len(right)
            # assert 0 <= left < num_symbols
            assert all(0 <= x < num_symbols for x in right)
            
            prod = Prod(left, right)
            if left >= 0:
                grammar.append(prod)
            
        assert len(grammar) == num_productions
        
    except:
        print(f"Error reading file: {grammar_file_path}")
        exit(2)
    
        
## read the input array
input_file_path = os.path.join(script_dir, f"test_{grammar_name}","test_data", f"{grammar_name}_instance{instance_index}_input.txt")

expr_array = []
with open(input_file_path, "r") as f:
    try:
        expr_array_len = int(f.readline())
        expr_array = list(map(int, f.readline().split()))
        
        # print(expr_array)
        assert all(0 <= x < num_symbols for x in expr_array)
        assert len(expr_array) == expr_array_len
        
    except:
        print(f"Error reading file: {input_file_path}")
        exit(2)
    
## read the output array
output_file_name = os.path.join(script_dir, f"test_{grammar_name}","test_data", f"{grammar_name}_instance{instance_index}_output.txt")
oper_array = []
with open(output_file_name, "r") as f:
    try: 
        oper_array = list(map(int, f.readlines()))
    
        assert all(-1 <= x < num_productions for x in oper_array)

    except:
        print(f"Error reading file: {output_file_name}")
        exit(2)
        
        
## test the shift reduce algorithm
left_scan_line = []
expr_array_index = 0
for oper in oper_array:
    if oper == -1:
        try:
            left_scan_line.append(expr_array[expr_array_index])
            expr_array_index += 1
        except:
            print(f"Error: The scan line is out of the expression")
            exit(1)
    else:
        try: 
            prod = grammar[oper]
            ## check the left side close to the scan line is the same as the right side of the production
            assert left_scan_line[-len(prod.right):] == prod.right
            ## execute the reduction
            left_scan_line = left_scan_line[:-len(prod.right)]
            left_scan_line.append(prod.left)
        except:
            print(f"Error: fail to execute the reduction of production {oper}")
            exit(1)
            
    ## show the current shift reduce status
    str_left_scan_line = " ".join(map(str, left_scan_line))
    str_right_scan_line = " ".join(map(str, expr_array[expr_array_index:]))
    print(str_left_scan_line + " | " + str_right_scan_line) 
        
## examine if finish the shift reduce algorithm
try:
    assert len(left_scan_line) == 1 and left_scan_line[0] == 0
    print("Succeed")
except:
    print("Error: fail to reduce the expression to the start symbol")
    exit(1)