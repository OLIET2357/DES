# ABSTRACT

Fast DES implementation by OLIET.

Full compatibility with CUDA.

Referred https://qiita.com/arsley/items/d2679f1a7e68d93c8dbe (Japanese)

# USAGE
Use via C++, compile "des.cpp", and include "des.hpp".

Use via CUDA, DO NOT COMPILE "des.cpp" neither "des.hpp", just include "des.cpp".

## Example Code
### main.cpp
    #include "des.hpp"
    int main()
    {
        DES::main();
        return 0;
    }

g++ main.cpp des.cpp

### Main.cu
    #include "des.cpp"
    int main()
    {
        DES::main();
        return 0;
    }

nvcc Main.cu

