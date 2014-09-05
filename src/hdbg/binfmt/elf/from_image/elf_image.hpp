#ifndef __HDBG_SRC_BINFMT_ELF_FROM_IMAGE_ELF_IMAGE_HPP__
#define __HDBG_SRC_BINFMT_ELF_FROM_IMAGE_ELF_IMAGE_HPP__

#include "../elf.hpp"
#include "../../../arch/endian.hpp"

#include <hdbg/binary_format.hpp>
#include <hdbg/dbg/debug_process.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>

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
  virtual std::ptrdiff_t entry_point() const override;
  virtual std::vector<Section> sections() const override;
  virtual std::vector<std::ptrdiff_t> code_entries() const override;
  
private:
  template <typename Fn> void visit_phdrs(Fn && fn) const;
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
      throw std::runtime_error("unknown arch (binfmt)");
  }
}

template <class ElfXX>
std::uintptr_t Elf_Image<ElfXX>::preferred_base() const
{
  return prf_base_;
}

template <class ElfXX>
std::ptrdiff_t Elf_Image<ElfXX>::entry_point() const
{
  return ehdr_.e_entry - prf_base_;
}

template <class ElfXX>
std::vector<BinaryFormat::Section> Elf_Image<ElfXX>::sections() const
{
  std::vector<Section> sections;
  sections.reserve( ehdr_.e_phnum );
  visit_phdrs([this, &sections] (const Elf_Phdr<ElfXX> & phdr){
    if(phdr.p_flags & PT_LOAD) {
      Section sect;
      sect.raw_offset = phdr.p_offset;
      sect.raw_size = phdr.p_filesz;
      sect.vaddr = phdr.p_vaddr - prf_base_;
      sect.vsize = phdr.p_memsz;
      sect.flags = ((phdr.p_flags & PF_R) ? Section::Flags::Readable   : 0) |
                   ((phdr.p_flags & PF_W) ? Section::Flags::Writable   : 0) |
                   ((phdr.p_flags & PF_X) ? Section::Flags::Executable : 0) ;
      sections.push_back(sect);
    }
  });
  return sections;
}

template <class ElfXX>
std::vector<std::ptrdiff_t> Elf_Image<ElfXX>::code_entries() const
{
  return { entry_point() };
}

template <class ElfXX> template <typename Fn>
void Elf_Image<ElfXX>::visit_phdrs(Fn && fn) const
{
  const auto phdrs_base = base_ + ehdr_.e_phoff;
  const auto phdrs_size = ehdr_.e_phnum * ehdr_.e_phentsize;
  std::unique_ptr<std::uint8_t[]> buf (new std::uint8_t[ phdrs_size ]);
  proc_->read_mem(phdrs_base, phdrs_size, buf.get());
  
  for(unsigned int i = 0; i < ehdr_.e_phnum; ++i) {
    const auto phdr = le_load<Elf_Phdr<ElfXX>>(&buf[i * ehdr_.e_phentsize]);
    fn(phdr);
  }
}

template <class ElfXX>
std::uintptr_t Elf_Image<ElfXX>::get_base_address() const
{
  auto prfbase = std::numeric_limits<std::uintptr_t>::max();
  visit_phdrs([&prfbase] (const Elf_Phdr<ElfXX> & phdr){
    if(phdr.p_flags & PT_LOAD && phdr.p_vaddr < prfbase)
      prfbase = phdr.p_vaddr;
  });
  return prfbase;
}

} // namespace hdbg

#endif // __HDBG_SRC_BINFMT_ELF_FROM_IMAGE_ELF_IMAGE_HPP__
