# CSE221_F22_Project

DIRECTORY STRUCTURE:
```
|-- Makefile
|
|
|---utils
|     
|     
|     
|---measurements
-    |
     |
     |--- CPU Scheduling and OS services
     |                |
     |                |---context switch time
     |                |--- measurement overhead
     |                |--- procedure call 
     |                |--- system call
     |                |--- task creation time
     |                -
     |
     |---- Memory Operations
     |                |--- page fault time 
     |                |--- RAM access time
     |                |--- RAM Bandwidth
     |                |--- Memory Info
     |                -
     |
     |
     |--- Network Operations
     |                |--- page fault time 
     |                |--- RAM access time
     |                |--- RAM Bandwidth
     |                |--- Memory Info
     |                -
     |
     |
     |---File System Operations
     -                |--- Contention
                      |--- FS cache size
                      |--- FS read time
                      -
```

We will use `Makefile` to specify the buiding directions for the project. 

Once we build using command  `make`, we write all the binaries to the `executables` folder.

The `raw_data` folder holds the measurement results. The notebooks in `utils_graph_notebooks` 
can be used to generate the graphs for our report.

# Executing Experiments:
```
make # This will build the project by creating and storing the executables in the ./executables folder.
chmod +x ./utils/repeat_cmd.sh
```



## Measurement Overhead


### Loop Overhead:

```
./utils/repeat_cmd.sh ./executables/loop_overhead <num_iterations>
```
```
(base) rg7@rg7-Inspiron-15-5510:~/Documents/Github/CSE221_F22_Project$ sudo bash  utils/repeat_cmd.sh ./executables/loop_overhead 100
Running command - utils/repeat_cmd.sh ./executables/loop_overhead 100
Currently in /home/rg7/Documents/Github/CSE221_F22_Project
Running ./executables/loop_overhead 100 times to get the median & average
===================================================================
Total counter is 100
Median:4
Sum is 414
Average:4.14
Std dev: 0.4256847956692942
```

### Reading Time Overhead
```
./utils/repeat_cmd.sh ./executables/reading_time_overhead <num_iterations>
```


```
(base) rg7@rg7-Inspiron-15-5510:~/Documents/Github/CSE221_F22_Project$ sudo bash  utils/repeat_cmd.sh ./executables/reading_time_overhead 100
Running command - utils/repeat_cmd.sh ./executables/reading_time_overhead 100
Currently in /home/rg7/Documents/Github/CSE221_F22_Project
Running ./executables/reading_time_overhead 100 times to get the median & average.
Total counter is 100
Median:23
Sum is 2343
Average:23.43
Std dev: 2.9135763328419566
```

### Process creation overhead

```
sudo bash  utils/repeat_cmd.sh ./executables/creation_processes <num_iterations>
```

```
sudo bash  utils/repeat_cmd.sh ./executables/creation_processes 100
 
Total counter is 100
Median:171634
Sum is 17248363
Average:172483.63
Std dev: 7329.631021318093
```

### Thread Creation overhead

```
sudo bash  utils/repeat_cmd.sh ./executables/creation_kthreads <num_iterations>
```

```
sudo bash  utils/repeat_cmd.sh ./executables/creation_kthreads 100

Total counter is 100
Median:25561
Sum is 2574845
Average:25748.45
Std dev: 1522.7859675781276
```

### System Call Overhead

```
sudo bash  utils/repeat_cmd.sh ./executables/system_call <num_iterations>
```

```
sudo bash  utils/repeat_cmd.sh ./executables/system_call 100
Total counter is 100
Median:1
Sum is 128
Average:1.28
Std dev: 1.4224922690292887
```

### Context Switch time

```
sudo bash  utils/repeat_cmd.sh ./executables/context_switch_kthreads <num_iterations>
sudo bash  utils/repeat_cmd.sh ./executables/context_switch_processes <num_iterations>
```

```
sudo bash  utils/repeat_cmd.sh ./executables/context_switch_kthreads 100

===================================================================
Total counter is 100
Median:4448
Sum is 446462
Average is 4464.620000
Std dev: 250.54723894197207
===================================================================

sudo bash  utils/repeat_cmd.sh ./executables/context_switch_processes 100


===================================================================
Total counter is 100
Median:4329
Sum is 434647
Average is 4346.470000
Std dev: 176.45370169164022
===================================================================
```

### Process Call

```
main: 


main: ***************** PROCEDURE_CALL_OVERHEAD *****************
set_nice: pid = 33694, old_priority = 0, new_priority = -20
main: 


main: ***************** RESULT 0*****************
main: iterations = 100000
main: (per iteration) args:0 cycles_taken = 6
main: 


main: ***************** RESULT 1*****************
main: iterations = 100000
main: (per iteration) args:1 cycles_taken = 6
main: 


main: ***************** RESULT 2 *****************
main: iterations = 100000
main: (per iteration) args:2 cycles_taken = 6
main: 


main: ***************** RESULT 3 *****************
main: iterations = 100000
main: (per iteration) args:3 cycles_taken = 7
main: 


main: ***************** RESULT 4 *****************
main: iterations = 100000
main: (per iteration) args:4 cycles_taken = 7
main: 


main: ***************** RESULT 5 *****************
main: iterations = 100000
main: (per iteration) args:5 cycles_taken = 8
main: 


main: ***************** RESULT 6 *****************
main: iterations = 100000
main: (per iteration) args:6 cycles_taken = 9
main: 


main: ***************** RESULT 7 *****************
main: iterations = 100000
main: (per iteration) args:7 cycles_taken = 12
```

### Analysis of results.

As we can see the time latency seen in the for loops and the time latency seen in the reading accounts for the total 
time delay, and this matches up with the spec provided.


### Conclusion: CPU Operations

As we can see that our results concur with our prediction from system specifications with a mininimum deviation. 
These deviations are transitive in nature and often converge to our predictions with increased iterations. 




## File Systems

### File Cache Size

![alt text](https://github.com/gmax9/CSE221_F22_Project/blob/master/measurements/file_system/file_cache_size/logs/file_cache_size_blocksize.png)


![alt text](https://github.com/gmax9/CSE221_F22_Project/blob/master/measurements/file_system/file_cache_size/logs/file_cache_size_readhead.png)



```
    set_nice: pid = 75123, old_priority = 0, new_priority = -20
    Block size is 4096B
    size(MB), time(ms), bandwidth(GB/s)
    1.00, 0.30, 3.36
    2.00, 0.64, 3.10
    3.00, 0.99, 3.04
    4.00, 1.30, 3.08
    5.00, 1.59, 3.15
    6.00, 1.89, 3.17
    7.00, 2.17, 3.23
    8.00, 2.61, 3.06
    9.00, 3.65, 2.46
    10.00, 3.22, 3.11
    100.00, 31.06, 3.22
    200.00, 61.69, 3.24     
    300.00, 93.16, 3.22
    400.00, 124.98, 3.20
    500.00, 164.09, 3.05
    600.00, 192.95, 3.11
    700.00, 227.28, 3.08
    800.00, 251.80, 3.18
    900.00, 283.75, 3.17
    1024.00, 323.05, 3.17
    2048.00, 667.02, 3.07
    3072.00, 1080.93, 2.84
    4096.00, 1339.51, 3.06
    4120.00, 1320.34, 3.12
    4140.00, 1278.62, 3.24
    4160.00, 1280.50, 3.25
    4180.00, 1343.91, 3.11
    4184.00, 1301.59, 3.21
    4188.00, 1345.00, 3.11
    4192.00, 1315.66, 3.19
    4200.00, 1308.43, 3.21
    4220.00, 1307.28, 3.23
    4240.00, 1311.61, 3.23
    4280.00, 1322.44, 3.24
    4340.00, 1346.19, 3.22
    4380.00, 1354.92, 3.23
    4420.00, 1410.41, 3.13
    4500.00, 1395.86, 3.22
    4580.00, 1403.75, 3.26
    4660.00, 1430.83, 3.26
    4740.00, 1455.39, 3.26
    4820.00, 1482.18, 3.25
    4980.00, 1542.06, 3.23
    5000.00, 1552.32, 3.22
    5040.00, 1735.85, 2.90
    5080.00, 3158.14, 1.61
    5140.00, 52733.96, 0.10
    5120.00, 52466.52, 0.10
    5160.00, 53341.77, 0.10
    6144.00, 63558.47, 0.10
    7168.00, 74441.74, 0.10
    8192.00, 85996.98, 0.10

```
![alt text](https://github.com/gmax9/CSE221_F22_Project/blob/master/measurements/file_system/file_cache_size/logs/file_cache_size.png)



### File Read time


#### Local file read time:

![alt text](https://github.com/gmax9/CSE221_F22_Project/blob/master/measurements/file_system/file_read_time/logs/file_read_time_only_small_files.png)



```
sudo ./executables/file_read_time_sizes.sh ./executables/file_read_time_seq
===================================================================
main: total access:128KB, Average access time/block(32):992349 clock cycles
main: total access:256KB, Average access time/block(64):490581 clock cycles
main: total access:512KB, Average access time/block(128):507070 clock cycles
main: total access:1MB, Average access time/block(256):341850 clock cycles
main: total access:2MB, Average access time/block(512):316896 clock cycles
main: total access:4MB, Average access time/block(1024):206814 clock cycles
main: total access:8MB, Average access time/block(1024):213863 clock cycles
main: total access:16MB, Average access time/block(1024):199088 clock cycles
main: total access:32MB, Average access time/block(1024):49101 clock cycles
main: total access:64MB, Average access time/block(1024):52232 clock cycles
main: total access:128MB, Average access time/block(1024):50587 clock cycles
main: total access:256MB, Average access time/block(1024):45057 clock cycles
main: total access:512MB, Average access time/block(1024):38698 clock cycles
main: total access:1GB, Average access time/block(1024):41848 clock cycles
main: total access:2GB, Average access time/block(1024):37695 clock cycles
main: total access:3GB, Average access time/block(1024):43746 clock cycles
main: total access:4GB, Average access time/block(1024):38568 clock cycles
main: total access:5GB, Average access time/block(1024):44174 clock cycles
main: total access:6GB, Average access time/block(1024):51064 clock cycles
main: total access:7GB, Average access time/block(1024):34688 clock cycles
```


![alt text](https://github.com/gmax9/CSE221_F22_Project/blob/master/measurements/file_system/file_read_time/logs/file_read_time.png)


#### Remote file read

```
===================================================================
main: total access:128KB, Average access time/block(32):69543857 clock cycles
main: total access:256KB, Average access time/block(64):58735454 clock cycles
main: total access:512KB, Average access time/block(128):55503708 clock cycles
main: total access:1MB, Average access time/block(256):55496092 clock cycles
main: total access:2MB, Average access time/block(512):77469376 clock cycles
main: total access:4MB, Average access time/block(1024):55007107 clock cycles
main: total access:8MB, Average access time/block(1024):57113248 clock cycles
main: total access:16MB, Average access time/block(1024):55303971 clock cycles
main: total access:32MB, Average access time/block(1024):56502063 clock cycles
main: total access:64MB, Average access time/block(1024):55739287 clock cycles
main: total access:128MB, Average access time/block(1024):93390548 clock cycles
main: total access:256MB, Average access time/block(1024):56295501 clock cycles
main: total access:512MB, Average access time/block(1024):55959462 clock cycles
main: total access:1GB, Average access time/block(1024):55505095 clock cycles
main: total access:2GB, Average access time/block(1024):56100125 clock cycles
main: total access:3GB, Average access time/block(1024):58413921 clock cycles
main: total access:4GB, Average access time/block(1024):58675522 clock cycles
main: total access:5GB, Average access time/block(1024):69095978 clock cycles
main: total access:6GB, Average access time/block(1024):57515622 clock cycles
main: total access:7GB, Average access time/block(1024):58220136 clock cycles
```

![alt text](https://github.com/gmax9/CSE221_F22_Project/blob/master/measurements/file_system/file_read_time/remote_log/remote_file_read_time.png)





### File Contention, with different readers

```
===================================================================

1 readers, average time / 4KB block = 348203 clock cycles

------------------------------------------------------------

2 readers, average time / 4KB block = 411365 clock cycles

------------------------------------------------------------

3 readers, average time / 4KB block = 429930 clock cycles

------------------------------------------------------------

4 readers, average time / 4KB block = 467640 clock cycles

------------------------------------------------------------

5 readers, average time / 4KB block = 475739 clock cycles

------------------------------------------------------------

6 readers, average time / 4KB block = 475704 clock cycles

------------------------------------------------------------

7 readers, average time / 4KB block = 485825 clock cycles

------------------------------------------------------------

8 readers, average time / 4KB block = 477760 clock cycles

------------------------------------------------------------

9 readers, average time / 4KB block = 511404 clock cycles

------------------------------------------------------------

```




### Conclusion: File Systems


As we can see our cache size experiments concur with our predictions, and the file read time and the contention to perform
writes as well work as per our expectations. 




## Memory Operations

### Page Fault time

```
Currently in /home/rg7/Documents/Github/CSE221_F22_Project
Running ./executables/page_fault_time 100 times to get the median & average.

===================================================================
Average:0.173481
Stddev:0.0188483
===================================================================
```

### RAM Bandwidth

```
Running ./executables/ram_access_time 100 times
        read (GB/s), write(GB/s)
===================================================================
Median: 10.84240, 9.75544  
Avg:    11.21756, 9.52479
Stddev: 1.91151, 0.96299
===================================================================
```



Build:

```
make mem_access_latency
make mem_access_bw
```

Execute:
```
sudo ./run_mem_benchmarks.sh
```
