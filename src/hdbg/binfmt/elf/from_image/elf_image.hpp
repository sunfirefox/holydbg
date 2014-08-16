#ifndef __HDBG_SRC_BINFMT_ELF_FROM_IMAGE_ELF_IMAGE_HPP__
#define __HDBG_SRC_BINFMT_ELF_FROM_IMAGE_ELF_IMAGE_HPP__

#include <hdbg/binary_format.hpp>
#include <hdbg/dbg/debug_process.hpp>

#include "../elf.hpp"

#include <boost/range/iterator_range.hpp>

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
  virtual std::uintptr_t entry() const override;
  
private:
  const DebugProcess * proc_ = nullptr;
  std::uintptr_t base_;
  Elf_Ehdr<ElfXX> ehdr_;
};

typedef Elf_Image<elf32_typedefs> Elf32_Image;
typedef Elf_Image<elf64_typedefs> Elf64_Image;

bool elf_check_image(const DebugProcess & proc, std::uintptr_t base);
std::unique_ptr<BinaryFormat> make_elf_image(const DebugProcess & proc, std::uintptr_t base);

namespace detail {

template <typename T> inline
T rem_le_load(const DebugProcess & proc, std::uintptr_t addr)
{
  T raw_val;
  proc.read_mem(addr, sizeof(T), &raw_val);
  return le_load<T>(&raw_val);
}

std::string rem_load_str(const DebugProcess & proc, std::uintptr_t addr);

} // namespace detail

template <> Elf32_Ehdr le_load(const void * from);
template <> Elf64_Ehdr le_load(const void * from);

template <class ElfXX>
Elf_Image<ElfXX>::Elf_Image(const DebugProcess & proc, std::uintptr_t base)
  : proc_(&proc), base_(base)
  , ehdr_( detail::rem_le_load<Elf_Ehdr<ElfXX>>(proc, base) ) {}

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
      return nullptr;
  }
}

template <class ElfXX> inline
std::uintptr_t Elf_Image<ElfXX>::entry() const
{
  return ehdr_.e_entry;
}

} // namespace hdbg

#endif // __HDBG_SRC_BINFMT_ELF_FROM_IMAGE_ELF_IMAGE_HPP__
