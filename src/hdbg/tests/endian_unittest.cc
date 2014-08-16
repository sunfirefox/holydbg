#include <gtest/gtest.h>

#include <hdbg/arch/endian.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>

TEST(EndianTest, BeLoadIntegral) {
  static const std::uint8_t be_bytes_from[] = { 0xde, 0xad, 0xbe, 0xef };
  ASSERT_EQ(0xdeadbeef, hdbg::be_load<std::uint32_t>(be_bytes_from));
}

TEST(EndianTest, LeLoadIntegral) {
  static const std::uint8_t le_bytes_from[] = { 0xef, 0xbe, 0xad, 0xde };
  ASSERT_EQ(0xdeadbeef, hdbg::le_load<std::uint32_t>(le_bytes_from));
}

TEST(EndianTest, BeStoreIntegral) {
  static const std::uint8_t be_bytes[] = { 0xde, 0xad, 0xbe, 0xef };
  std::uint8_t be_bytes_out[ sizeof(std::uint32_t) ];
  hdbg::be_store(be_bytes_out, 0xdeadbeef);
  ASSERT_TRUE( std::equal(std::begin(be_bytes), std::end(be_bytes), be_bytes_out) );
}

TEST(EndianTest, LeStoreIntegral) {
  static const std::uint8_t le_bytes[] = { 0xef, 0xbe, 0xad, 0xde };
  std::uint8_t le_bytes_out[ sizeof(std::uint32_t) ];
  hdbg::le_store(le_bytes_out, 0xdeadbeef);
  ASSERT_TRUE( std::equal(std::begin(le_bytes), std::end(le_bytes), le_bytes_out) );
}

TEST(EndianTest, BeLoadIntoArray) {
  static const std::uint32_t be_expected[] = {
    0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff
  };
  static const std::uint8_t be_bytes[] = {
    0x00, 0x11, 0x22, 0x33,
    0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb,
    0xcc, 0xdd, 0xee, 0xff
  };
  std::uint32_t be_array_out[4];
  hdbg::be_load_into(be_bytes, be_array_out);
  ASSERT_TRUE( std::equal(std::begin(be_expected), std::end(be_expected), be_array_out) );
}

TEST(EndianTest, LeLoadIntoArray) {
  static const std::uint32_t le_expected[] = {
    0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff
  };
  static const std::uint8_t le_bytes[] = {
    0x33, 0x22, 0x11, 0x00,
    0x77, 0x66, 0x55, 0x44,
    0xbb, 0xaa, 0x99, 0x88,
    0xff, 0xee, 0xdd, 0xcc
  };
  std::uint32_t le_array_out[4];
  hdbg::le_load_into(le_bytes, le_array_out);
  ASSERT_TRUE( std::equal(std::begin(le_expected), std::end(le_expected), le_array_out) );
}

TEST(EndianTest, BeStoreArray) {
  static const std::uint8_t be_expected[] = {
    0x00, 0x11, 0x22, 0x33,
    0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb,
    0xcc, 0xdd, 0xee, 0xff
  };
  static const std::uint32_t be_array[] = {
    0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff
  };
  std::uint8_t be_bytes_out[ sizeof(be_expected) ];
  hdbg::be_store(be_bytes_out, be_array);
  ASSERT_TRUE( std::equal(std::begin(be_expected), std::end(be_expected), be_bytes_out) );
}

TEST(EndianTest, LeStoreArray) {
  static const std::uint8_t le_expected[] = {
    0x33, 0x22, 0x11, 0x00,
    0x77, 0x66, 0x55, 0x44,
    0xbb, 0xaa, 0x99, 0x88,
    0xff, 0xee, 0xdd, 0xcc
  };
  static const std::uint32_t le_array[] = {
    0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff
  };
  std::uint8_t le_bytes_out[ sizeof(le_expected) ];
  hdbg::le_store(le_bytes_out, le_array);
  ASSERT_TRUE( std::equal(std::begin(le_expected), std::end(le_expected), le_bytes_out) );
}

struct Foo
{
  std::uint32_t a;
  std::uint16_t b;
  std::uint8_t  c;
};

bool operator==(const Foo & lhs, const Foo & rhs)
{
  return lhs.a == rhs.a &&
         lhs.b == rhs.b &&
         lhs.c == rhs.c ;
}

namespace hdbg {

template <>
Foo be_load<Foo>(const void * from)
{
  Foo foo;
  const auto bytes = reinterpret_cast<const std::uint8_t *>(from);
  be_load_into(bytes + offsetof(Foo, a), foo.a);
  be_load_into(bytes + offsetof(Foo, b), foo.b);
  be_load_into(bytes + offsetof(Foo, c), foo.c);
  return foo;
}

template <>
Foo le_load<Foo>(const void * from)
{
  const auto bytes = reinterpret_cast<const std::uint8_t *>(from);
  Foo foo;
  le_load_into(bytes + offsetof(Foo, a), foo.a);
  le_load_into(bytes + offsetof(Foo, b), foo.b);
  le_load_into(bytes + offsetof(Foo, c), foo.c);
  return foo;
}

template <>
void be_store(void * to, const Foo & foo)
{
  const auto bytes = reinterpret_cast<std::uint8_t *>(to);
  be_store(bytes + offsetof(Foo, a), foo.a);
  be_store(bytes + offsetof(Foo, b), foo.b);
  be_store(bytes + offsetof(Foo, c), foo.c);
}

template <>
void le_store(void * to, const Foo & foo)
{
  const auto bytes = reinterpret_cast<std::uint8_t *>(to);
  le_store(bytes + offsetof(Foo, a), foo.a);
  le_store(bytes + offsetof(Foo, b), foo.b);
  le_store(bytes + offsetof(Foo, c), foo.c);
}

} // namespace hdbg

TEST(EndianTest, BeLoadStruct) {
  static const Foo be_expected { 0xaabbccdd, 0x1234, 0xff };
  static const std::uint8_t be_bytes[] = {
    0xaa, 0xbb, 0xcc, 0xdd,
    0x12, 0x34,
    0xff
  };
  ASSERT_EQ(be_expected, hdbg::be_load<Foo>(be_bytes));
}

TEST(EndianTest, LeLoadStruct) {
  static const Foo le_expected { 0xaabbccdd, 0x1234, 0xff };
  static const std::uint8_t le_bytes[] = {
    0xdd, 0xcc, 0xbb, 0xaa,
    0x34, 0x12,
    0xff
  };
  ASSERT_EQ(le_expected, hdbg::le_load<Foo>(le_bytes));
}

TEST(EndianTest, BeStoreStruct) {
  static const Foo foo { 0xaabbccdd, 0x1234, 0xff };
  static const std::uint8_t be_bytes[] = {
    0xaa, 0xbb, 0xcc, 0xdd,
    0x12, 0x34,
    0xff
  };
  std::uint8_t be_buf[ sizeof(Foo) ];
  hdbg::be_store(be_buf, foo);
  ASSERT_TRUE( std::equal(std::begin(be_bytes), std::end(be_bytes), be_buf) );
}

TEST(EndianTest, LeStoreStruct) {
  static const Foo foo { 0xaabbccdd, 0x1234, 0xff };
  static const std::uint8_t le_bytes[] = {
    0xdd, 0xcc, 0xbb, 0xaa,
    0x34, 0x12,
    0xff
  };
  std::uint8_t le_buf[ sizeof(Foo) ];
  hdbg::le_store(le_buf, foo);
  ASSERT_TRUE( std::equal(std::begin(le_bytes), std::end(le_bytes), le_buf) );
}

