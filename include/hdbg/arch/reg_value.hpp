#ifndef __HDBG_ARCH_REG_VALUE_HPP__
#define __HDBG_ARCH_REG_VALUE_HPP__

#include <hdbg/arch/endian.hpp>

#include <boost/multiprecision/gmp.hpp>

namespace hdbg {

typedef boost::multiprecision::mpz_int RegValue;

} // namespace hdbg

#endif // __HDBG_ARCH_REG_VALUE_HPP__
