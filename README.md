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

### test.cpp
    #include <iostream>
    #include "des.hpp"
    int main()
    {
        const block_t p = 0xcafebabedeadbeaf;
        const block_t k = 0x0123456789abcdef;
        const block_t c = DES::encrypt_block(p, k);
        std::cout << std::hex << c << std::endl;
        std::cout << std::hex << DES::decrypt_block(c, k) << std::endl;
        return 0;
    }

g++ test.cpp des.cpp

