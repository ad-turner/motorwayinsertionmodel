"""
This program is used as part of the process to generate a csv file containing IAS values
This process pipes a fixed of I = 0 and varying values of F from 4 to 39 to the C code
"""

my_list = range(4,30)

for x in my_list:
    print(f"0,{x}")
