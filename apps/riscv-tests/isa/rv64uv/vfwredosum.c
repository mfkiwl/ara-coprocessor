// Copyright 2022 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Xiaorui Yin <yinx@student.ethz.ch>
// Date: 2022/05/03

#include "float_macros.h"
#include "vector_macros.h"

// Naive test
void TEST_CASE1(void) {
  VSET(1, e32, m1);
  VLOAD_32(v2, 0x3F800000);
  VSET(16, e16, m1);
  // 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8
  VLOAD_16(v6, 0x3c00, 0x4000, 0x4200, 0x4400, 0x4500, 0x4600, 0x4700, 0x4800,
           0x3c00, 0x4000, 0x4200, 0x4400, 0x4500, 0x4600, 0x4700, 0x4800);
  asm volatile("vfwredosum.vs v4, v6, v2");
  VCMP_U32(1, v4, 0x42920000);
}

// Masked naive test
void TEST_CASE2(void) {
  VSET(1, e64, m1);
  VLOAD_64(v2, 0x3FF0000000000000);
  VSET(16, e32, m1);
  VLOAD_32(v6, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  asm volatile("vfwredosum.vs v4, v6, v2");
  VCMP_U64(2, v4, 0x4052400000000000);

  VSET(1, e32, m1);
  VLOAD_32(v2, 0x3F800000);
  VSET(2, e8, m1);
  VLOAD_8(v0, 0xaa, 0x55);
  VSET(16, e16, m1);
  // 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8
  VLOAD_16(v6, 0x3c00, 0x4000, 0x4200, 0x4400, 0x4500, 0x4600, 0x4700, 0x4800,
           0x3c00, 0x4000, 0x4200, 0x4400, 0x4500, 0x4600, 0x4700, 0x4800);
  asm volatile("vfwredosum.vs v4, v6, v2, v0.t");
  VCMP_U32(3, v4, 0x42140000);

  VSET(1, e64, m1);
  VLOAD_64(v2, 0x3FF0000000000000);
  VSET(2, e8, m1);
  VLOAD_8(v0, 0xaa, 0x55);
  VSET(16, e32, m1);
  VLOAD_32(v6, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  asm volatile("vfwredosum.vs v4, v6, v2, v0.t");
  VCMP_U64(4, v4, 0x4042800000000000);
}

// Are we respecting the undisturbed tail policy?
void TEST_CASE3(void) {
  VSET(16, e16, m1);
  // 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8
  VLOAD_16(v6, 0x3c00, 0x4000, 0x4200, 0x4400, 0x4500, 0x4600, 0x4700, 0x4800,
           0x3c00, 0x4000, 0x4200, 0x4400, 0x4500, 0x4600, 0x4700, 0x4800);
  VLOAD_32(v2, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  VLOAD_32(v4, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  asm volatile("vfwredosum.vs v4, v6, v2");
  VCMP_U32(5, v4, 0x42920000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);

  VSET(16, e32, m1);
  VLOAD_32(v6, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  VLOAD_64(v2, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  VLOAD_64(v4, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  asm volatile("vfwredosum.vs v4, v6, v2");
  VCMP_U64(6, v4, 0x4052400000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
}

// Odd number of elements, undisturbed policy
void TEST_CASE4(void) {
  VSET(1, e32, m1);
  VLOAD_32(v6, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  VLOAD_64(v2, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  VLOAD_64(v4, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  asm volatile("vfwredosum.vs v4, v6, v2");
  VCMP_U64(7, v4, 0x4000000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);

  VSET(3, e32, m1);
  VLOAD_32(v6, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  VLOAD_64(v2, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  VLOAD_64(v4, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  asm volatile("vfwredosum.vs v4, v6, v2");
  VCMP_U64(8, v4, 0x401C000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);

  VSET(7, e32, m1);
  VLOAD_32(v6, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  VLOAD_64(v2, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  VLOAD_64(v4, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  asm volatile("vfwredosum.vs v4, v6, v2");
  VCMP_U64(9, v4, 0x403d000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);

  VSET(15, e32, m1);
  VLOAD_32(v6, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  VLOAD_64(v2, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  VLOAD_64(v4, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  asm volatile("vfwredosum.vs v4, v6, v2");
  VCMP_U64(10, v4, 0x4050400000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
}

// Odd number of elements, undisturbed policy, and mask
void TEST_CASE5(void) {
  VSET(7, e16, m1);
  VLOAD_8(v0, 0x00, 0xff);
  // 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8
  VLOAD_16(v6, 0x3c00, 0x4000, 0x4200, 0x4400, 0x4500, 0x4600, 0x4700, 0x4800,
           0x3c00, 0x4000, 0x4200, 0x4400, 0x4500, 0x4600, 0x4700, 0x4800);
  VLOAD_32(v2, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  VLOAD_32(v4, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  asm volatile("vfwredosum.vs v4, v6, v2, v0.t");
  VCMP_U32(11, v4, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);

  VSET(1, e32, m1);
  VLOAD_8(v0, 0xff, 0x00);
  VLOAD_32(v6, 0x3F800000, 0x40000000, 0x40400000, 0x40800000, 0x40A00000,
           0x40C00000, 0x40E00000, 0x41000000, 0x3F800000, 0x40000000,
           0x40400000, 0x40800000, 0x40A00000, 0x40C00000, 0x40E00000,
           0x41000000);
  VLOAD_64(v2, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  VLOAD_64(v4, 0x3FF0000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
  asm volatile("vfwredosum.vs v4, v6, v2, v0.t");
  VCMP_U64(12, v4, 0x4000000000000000, 0x4000000000000000, 0x4008000000000000,
           0x4010000000000000, 0x4014000000000000, 0x4018000000000000,
           0x401C000000000000, 0x4020000000000000, 0x3FF0000000000000,
           0x4000000000000000, 0x4008000000000000, 0x4010000000000000,
           0x4014000000000000, 0x4018000000000000, 0x401C000000000000,
           0x4020000000000000);
}

int main(void) {
  INIT_CHECK();
  enable_vec();
  enable_fp();

  TEST_CASE1();
  TEST_CASE2();
  TEST_CASE3();
  TEST_CASE4();
  TEST_CASE5();

  EXIT_CHECK();
}
