# SW_Computer_Architectures

## 1. 설계목적
Data cache simulator를 구현해 봄으로써 N-way associative cache의 동작원리를 이해하도록 함.

----
## 2. 설계방법
본 설계과제에서 cache 사이즈를 변화(2의 승수에서 64KB까지) 시킬 수 있는 N-way associative cache  
를 설계한다. Cache set에서 replace되는 way/block을 선택하기 위해 LRU (Least Recently Used) 방식  
을 선택하라. 시뮬레이션을 위해 BYTE addressable 메모리 계층구조를 가정하라. 여러분이 구현할 시뮬  
레이터는 다음과 같은 4가지 변수가 실행되도록 하여야 한다.  

```
-t input_trace_file
-s size
-a associativity
-b block_size
```

Cache size (-s)와 block size (-b)는 byte로 수행하고, associativity는 2의 승수로 수행되어야 한다.  
Input trace file은 첨부한 go_ld_trace (ZIP file)를 사용하라. 이 파일은 어떤 프로그램의 실행된  
load 명령어들의 trace를 포함하고 있다. 각 라인은 실행된 load 명령어에 의해 접근된 바이트 주소로  
표현되어 있다. Trace를 읽는 예는 read_ld_trace.c에서 기술되어 있다. 여러분은 이 파일을 사용하고,  
아래 보이는 trace의 시작부분부터 사용하여 캐쉬 시뮬레이터를 작성하라.  

Here is the start of the trace file "go_ld_trace trace" you will be working with; once restored  
(make sure you see the same load addresses) is:  
536869800  
536869816  
1073839984  
1073867968  
1074199644  
1074170256  
536869696  
1073839832  
1074229644  
1074196644  

The output of running your cache simulator should look like:  
>cache-sim -t go_ld_trace -s 1024 -b 32 -a 2  
program_name: go_ld_trace  
cache_size:1024  
block_size:32  
associativity:2  
total_lds:1500000  
cache_hits:1005939  
cache_misses:494061  
cache_miss_rate:0.33  
The maximum values your simulator needs to support for each of the parameters (s,b,a) is 64K  
bytes for the cache size, 256 bytes for the block size, and the full associativity.  
