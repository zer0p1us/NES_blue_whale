### Building from source

**Windows**:

```cmake
cmake .. -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE=Release"
```

**important**: make sure the build type is ```Release```, there seems to a bug with linking SDL otherwise