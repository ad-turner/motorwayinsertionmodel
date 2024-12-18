## SIE ENG270 TITLE

## Project Description

DESCRIPTION
This program will simulate a road given certain density and insertion parameters.
---- C can be stand alone asjdhfksf ---

### Inputs and outputs

Inputs:
- Insertion and density parameters provided by the Python programs
- ias_data.csv

Outputs:
- 3 graphs generated by Python

ias_data.csv is provided but can be generated again by user if wished

### Implementation details

Overview:
- The simulation is handled by C. The C program is compiled then communicates with Python via pipe.
- Python handles I/O. It pipes input values to the C program and then analyses the output piped out by C. Certain parameters are defined in the C program directly (they do not change throughout this study).
- Ultimately, Python produces the graphs.

Structure:
- gennum.py:
    - feeds input values to C program
- analyze_main.py:
    - creates dictionnary from ias_data.csv
    - produced 3 graphs using the output from the C program and the data in ias_data.csv
- model_main.c:
    - recieves values from Python
    - runs accordingly
    - sends values back to Python
- gennum_ias.py:
    - feeds the input values to C program with the aim of producing the ias_data.csv file
- analyze_ias.py:
    - recieves the output from C and produces ias_data.csv

We provide a ias_data.csv file. If you wish to reproduce the IAS data follow instructions bellow. If not, the gennum_ias.py and analyse_ias.py files are of no use to you.

For more details on each program read the comments in each file.

## Instructions

To reproduce results in the report, two steps should be followed:

1. Compile model_main.c.
2. Run and pipe the programs.

To compile the C code, run the following line in the terminal from the project root directory (location of this README.md file):
```
gcc -Wall -o model_main model_main.c -lm    
```

To produce the graphs pipe information from gennum.py to model_main.c then again to analyze_main.py like this:
```
python3 gennum.py | ./model_main | python3 analyse_main.py
```
This command will run everything and generate the three graphs found in the report (Fig. 1, Fig.3 and Fig.4). Figure 2 was not generated with python, the lines were simply added by hand for illustrative purposes.

If you want to reproduce the IAS data, after you have compiled the model run the following line in your terminal:
```
python3 gennum_ias.py | ./model_main | python3 analyse_ias.py
```
This will similarly do a three-way pipe and generate a new ias_data.csv.
Note that this new file will override the current one. Also note that the values may differ slightly as we are rerunning the simulation.

## Requirements

Versions of Python and C used are as follows. 
```
$ python --version
Python 3.12.2

$ gcc --version
Apple clang version 14.0.3 (clang-1403.0.22.14.1)
Target: arm64-apple-darwin22.6.0
Thread model: posix
```

The Python libraries utilized were the following:
``` 
sys
matplotlib.pyplot
csv
math
gaussian_filter1d from scipy.ndimage
```

## Credits

The updateroad() function in model_main.c is a (somewhat modified) implementation of the road evolution rules described in the paper [Nagel, K., Schreckenberg, M. (1992): A cellular automaton model for freeway traffic](https://www.researchgate.net/publication/263504490_A_cellular_automaton_model_for_freeway_traffic)(page 2).

The websites [Geeks for geeks](https://www.geeksforgeeks.org/) and [W3Schools](https://www.w3schools.com/) as well as the artificial intelligence [ChatGPT](https://chatgpt.com/) were used for basic syntaxical queries.

## Author:

Adrien Turner
