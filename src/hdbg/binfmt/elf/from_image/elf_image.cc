#include "elf_image.hpp"

#include <hdbg/dbg/debug_process.hpp>

#include <elf.h>

#include <algorithm>
#include <cstddef>
#include <sstream>

namespace hdbg {

std::unique_ptr<BinaryFormat> make_elf_image(const DebugProcess & dbg_proc, std::uintptr_t base)
{
  std::uint8_t img_ident[EI_NIDENT];
  dbg_proc.read_mem(base, sizeof(img_ident), img_ident);
  
  switch(img_ident[EI_CLASS]) {
    case ELFCLASS32:
      return std::unique_ptr<BinaryFormat>( new Elf32_Image(dbg_proc, base) );
    
    case ELFCLASS64:
      return std::unique_ptr<BinaryFormat>( new Elf64_Image(dbg_proc, base) );
    
    default:
      return nullptr;
  }
}

bool elf_check_image(const DebugProcess & dbg_proc, std::uintptr_t base)
{
  std::uint8_t img_elfmag[SELFMAG];
  dbg_proc.read_mem(base, SELFMAG, img_elfmag);
  return std::equal(std::begin(img_elfmag), std::end(img_elfmag), ELFMAG);
}

namespace detail {

std::string rem_load_str(const DebugProcess & proc, std::uintptr_t addr)
{
  unsigned int offs = 0;
  std::ostringstream oss;
  char c;
  do {
    proc.read_mem(addr + offs, sizeof(char), &c);
    oss << c;
    ++offs;
  } while(c != '\0');
  return oss.str();
}

} // namespace detail

template <>
Elf32_Ehdr le_load(const void * from)
{
  const auto bytes = reinterpret_cast<const std::uint8_t *>(from);
  Elf32_Ehdr ehdr;
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_ident), ehdr.e_ident);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_type), ehdr.e_type);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_machine), ehdr.e_machine);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_version), ehdr.e_version);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_entry), ehdr.e_entry);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_phoff), ehdr.e_phoff);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_shoff), ehdr.e_shoff);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_flags), ehdr.e_flags);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_ehsize), ehdr.e_ehsize);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_phentsize), ehdr.e_phentsize);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_phnum), ehdr.e_phnum);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_shentsize), ehdr.e_shentsize);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_shnum), ehdr.e_shnum);
  le_load_into(bytes + offsetof(Elf32_Ehdr, e_shstrndx), ehdr.e_shstrndx);
  return ehdr;
}

template <>
Elf64_Ehdr le_load(const void * from)
{
  const auto bytes = reinterpret_cast<const std::uint8_t *>(from);
  Elf64_Ehdr ehdr;
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_ident), ehdr.e_ident);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_type), ehdr.e_type);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_machine), ehdr.e_machine);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_version), ehdr.e_version);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_entry), ehdr.e_entry);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_phoff), ehdr.e_phoff);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_shoff), ehdr.e_shoff);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_flags), ehdr.e_flags);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_ehsize), ehdr.e_ehsize);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_phentsize), ehdr.e_phentsize);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_phnum), ehdr.e_phnum);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_shentsize), ehdr.e_shentsize);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_shnum), ehdr.e_shnum);
  le_load_into(bytes + offsetof(Elf64_Ehdr, e_shstrndx), ehdr.e_shstrndx);
  return ehdr;
}

} // namespace hdbg
