# Goal
Write API functions for LTC2943 sensor *(Multicell Battery Gas Gauge with Temperature, Voltage and Current Measurement)*



## Requirements 
- Cmake
- C/C++ compiler, gcc/clang etc. *(Visual Studio provides their own compiler if working in Windows)*


## API functionality
1) Change chip's ADC mode.
2) Read chip's ADC mode.
3) Check temperature alert.
4) Check voltage alert.


## Quickstart

(todo cmake can handle this!)
```console
mkdir build 
cd build
```


I'm using [ninja](https://ninja-build.org/) to build project but project can be build with Visual Studio(default cmake generator) if using Windows.
```console
cmake .. -G"Ninja"
ninja
.\LTC2943_api.exe
```

