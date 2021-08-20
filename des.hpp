#pragma once

#include <iostream>
#include <cstdint>

#define PREFIX

using block_t = uint64_t;

namespace DES
{
    void print_bit(uint64_t x, int bit);
    PREFIX uint32_t popcnt(uint32_t x);
    PREFIX int parity(unsigned int x);
    PREFIX uint64_t translocate_PC1(const block_t k);
    PREFIX block_t translocate_PC1I(const uint64_t kp);
    PREFIX uint64_t translocate_PC2(const uint64_t cd);
    PREFIX uint64_t translocate_IP(const block_t a);
    PREFIX uint64_t translocate_E(const uint64_t r);
    PREFIX uint32_t transform_SBOX(const uint64_t i);
    PREFIX uint32_t translocate_P(const uint32_t j);
    PREFIX uint32_t f(const uint64_t r, const uint64_t sk);
    PREFIX block_t translocate_IPI(const uint64_t rl);
    PREFIX void generate_subkeys(uint64_t sk[16], const uint64_t kp);
    PREFIX block_t encrypt_block_with_subkeys_prime(const uint64_t pp, const uint64_t sk[16]);
    PREFIX block_t decrypt_block_with_subkeys_prime(const uint64_t cp, const uint64_t sk[16]);
    PREFIX block_t encrypt_block_with_subkeys(const block_t p, const uint64_t sk[16]);
    PREFIX block_t decrypt_block_with_subkeys(const block_t c, const uint64_t sk[16]);
    PREFIX block_t encrypt_block(const block_t p, const block_t k);
    PREFIX block_t decrypt_block(const block_t c, const block_t k);
    void main();
}

#undef PREFIX
