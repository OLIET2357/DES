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