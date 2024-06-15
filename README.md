# TypMap
Mapper For Typical CGRA
## Table of contents

1. [Directory Structure](#directory-structure)
2. [Getting Started](#getting-started)
    1. [Hardware pre-requisities](#hardware-pre-requisities)
    2. [Software pre-requisites](#software-pre-requisites)
    3. [Installation](#installation)
    4. [Running example](#running-example)
    5. [Data formats](#data-formats)
# Directory Structure
```
TypMap
│───README.md
│───build.sh
│───run.sh
│───data
│     └───test.txt
│───log
│     └───test.log
│───CGRA.cpp
│───CGRA.h
│───config.h
│───DFG.cpp
│───DFG.h
│───TEC.cpp
│───TEC.h
│───main.cpp
│───mcqd.cpp
│───mcqd.h
│───tool.cpp
└───tool.h
```

# Getting start
## Hardware pre-requisities
* Ubuntu 20.04.5
* Intel(R) Xeon(R) CPU E5-2650 v4 @ 2.20GHz
## Software pre-requisities
* c++11
* gflags
* glog

## Installation
First, install the gflags
```
git clone https://github.com/gflags/gflags.git
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=ON -DINSTALL_HEADERS=ON -DINSTALL_SHARED_LIBS=ON -DINSTALL_STATIC_LIBS=ON ..
make
sudo make install
```
Then, install the glog
```
git clone https://github.com/google/glog.git
cd glog
mkdir build
cmake ..
make
sudo make install
```
Finally, compile this project and you can get ./mcq  file
```
./build
```
## Running Example
You can run the example and get the final results in log/test.log
```
./run.sh start
```
Please note that in ./run.sh, you need to modify the following parameters for the DFG that requires map:
* --II (The II of Dfg after scheduling.)
* --childNum (The maximum number of child nodes in DFG.)
* --pea_column (The number of columns of processing element and LSU array in Typ-CGRA).)
* --pea_row (The number of rows of processing element array in Typ-CGRA.)

## Data Formats

Format of DFG scheduling that try to map
```
|----------|------------|-------------|------------|------------|
|node index|  time step |node's type  |child node 1|child node 2|
|----------|------------|-------------|------------|------------|
```
An example
```
0,0,1,1,-1,-1,-1
1,1,0,2,-1,-1,-1
2,2,0,3,-1,-1,-1
3,3,1,-1,-1,-1,-1
```
Format of DFG mapping
```
--------------------------[0]----------------------------------
|   LSU    |   PE    |   PE    |   PE    |   PE    |  
|   LSU    |   PE    |   PE    |   PE    |   PE    |  
|   LSU    |   PE    |   PE    |   PE    |   PE    |  
|   LSU    |   PE    |   PE    |   PE    |   PE    |  
---------------------------------------------------------------
```
An example
```
----------------------------------------[0]----------------------------------------
|         0       |               |               |               |       2       |
|                 |               |               |               |               |
|                 |               |               |               |               |
|                 |               |               |               |               |
----------------------------------------[1]----------------------------------------
|         3       |               |               |               |       1       |
|                 |               |               |               |               |
|                 |               |               |               |               |
|                 |               |               |               |               |
-----------------------------------------------------------------------------------
```
