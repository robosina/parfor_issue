Main codes is located in gpu2 folder.

# how to build
git clone https://github.com/robosina/parfor_issue.git --recursive


# gpu result
number of intersections:16000000

Which Operation\which gpu | 960mx | 1080ti | cpu |
--- | --- | --- | --- |
time for read dataset | 11652 microseconds | 4971 microseconds | Not defined |
Time to compute intersections | 57.314 ms  | **0.006 ms** | 30 days |
totoal computation time | 415 milliseconds | 175 milliseconds | not defined |

# matlab result

30 days

# compare results

![Selection_317](https://user-images.githubusercontent.com/14973524/120114617-3ea1a180-c195-11eb-920c-0a0b0f15d092.png)
