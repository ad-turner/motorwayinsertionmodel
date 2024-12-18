"""
This program pipes values of I from 0 to 39 and a fixed value of F = 20 to the C code
"""

my_list = range(40)
for x in my_list:
    print(f"{x},20")
