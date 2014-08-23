#ifndef __HDBG_SRC_BINFMT_ELF_FROM_IMAGE_ELF_IMAGE_HPP__
#define __HDBG_SRC_BINFMT_ELF_FROM_IMAGE_ELF_IMAGE_HPP__

#include <hdbg/binary_format.hpp>
#include <hdbg/arch/endian.hpp>
#include <hdbg/dbg/debug_process.hpp>

#include "../elf.hpp"

#include <boost/range/iterator_range.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace hdbg {

class DebugProcess;

template <class ElfXX>
class Elf_Image final : public BinaryFormat
{
public:
  Elf_Image(const DebugProcess &, std::uintptr_t);
  Elf_Image(Elf_Image const&) = delete;
  Elf_Image(Elf_Image &&);
  virtual ~Elf_Image();
  
  virtual const char * arch() const override;
  virtual std::uintptr_t preferred_base() const override;
  virtual std::ptrdiff_t entry() const override;
  
private:
  std::uintptr_t get_base_address() const;
  
  const DebugProcess * proc_ = nullptr;
  std::uintptr_t base_;
  Elf_Ehdr<ElfXX> ehdr_;
  std::uintptr_t prf_base_;
};

typedef Elf_Image<Elf32> Elf32_Image;
typedef Elf_Image<Elf64> Elf64_Image;

bool elf_check_image(const DebugProcess & proc, std::uintptr_t base);
std::unique_ptr<BinaryFormat> make_elf_image(const DebugProcess & proc, std::uintptr_t base);

template <> Elf32_Ehdr le_load(const void * from);
template <> Elf64_Ehdr le_load(const void * from);

template <> Elf32_Phdr le_load(const void * from);
template <> Elf64_Phdr le_load(const void * from);

namespace detail {

template <typename T> inline
T rem_le_load(const DebugProcess & proc, std::uintptr_t addr)
{
  std::uint8_t buf [ sizeof(T) ];
  proc.read_mem(addr, sizeof(buf), buf);
  return le_load<T>(buf);
}

std::string rem_load_str(const DebugProcess & dbg_proc, std::uintptr_t addr);

} // namespace detail

template <class ElfXX>
Elf_Image<ElfXX>::Elf_Image(const DebugProcess & proc, std::uintptr_t base)
  : proc_( &proc ), base_( base )
  , ehdr_( detail::rem_le_load<Elf_Ehdr<ElfXX>>(proc, base) )
  , prf_base_( get_base_address() ) {}

template <class ElfXX>
Elf_Image<ElfXX>::Elf_Image(Elf_Image &&) = default;

template <class ElfXX>
Elf_Image<ElfXX>::~Elf_Image() = default;

template <class ElfXX>
const char * Elf_Image<ElfXX>::arch() const
{
  switch(ehdr_.e_machine) {
    case EM_386:
      return "x86";
    
    case EM_X86_64:
      return "x86_64";
    
    case EM_NONE: default:
      throw std::runtime_error("unknown arch");
  }
}

template <class ElfXX>
std::uintptr_t Elf_Image<ElfXX>::preferred_base() const
{
  return prf_base_;
}

template <class ElfXX>
std::ptrdiff_t Elf_Image<ElfXX>::entry() const
{
  return ehdr_.e_entry - prf_base_;
}

template <class ElfXX>
std::uintptr_t Elf_Image<ElfXX>::get_base_address() const
{
  const auto phdrs_base = base_ + ehdr_.e_phoff;
  const auto phdrs_size = ehdr_.e_phnum * ehdr_.e_phentsize;
  std::unique_ptr<std::uint8_t[]> buf (new std::uint8_t[ phdrs_size ]);
  proc_->read_mem(phdrs_base, phdrs_size, buf.get());
  
  auto prfbase = std::numeric_limits<std::uintptr_t>::max();
  for(unsigned int i = 0; i < ehdr_.e_phnum; ++i) {
    const auto phdr = le_load<Elf_Phdr<ElfXX>>(&buf[i * ehdr_.e_phentsize]);
    if(phdr.p_flags & PT_LOAD && phdr.p_vaddr < prfbase)
      prfbase = phdr.p_vaddr;
  }
  return prfbase;
}

} // namespace hdbg

#endif // __HDBG_SRC_BINFMT_ELF_FROM_IMAGE_ELF_IMAGE_HPP__
