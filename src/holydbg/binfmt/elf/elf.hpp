#ifndef __HOLYDBG_SRC_BINFMT_ELF_HPP__
#define __HOLYDBG_SRC_BINFMT_ELF_HPP__

#include <elf.h>

namespace hdbg {

struct elf32_typedefs
{
  typedef Elf32_Half    Half;
  typedef Elf32_Word    Word;
  typedef Elf32_Sword   Sword;
  typedef Elf32_Xword   Xword;
  typedef Elf32_Sxword  Sxword;
  typedef Elf32_Addr    Addr;
  typedef Elf32_Off     Off;
  typedef Elf32_Section Section;
  typedef Elf32_Versym  Versym;
};

struct elf64_typedefs
{
  typedef Elf64_Half    Half;
  typedef Elf64_Word    Word;
  typedef Elf64_Sword   Sword;
  typedef Elf64_Xword   Xword;
  typedef Elf64_Sxword  Sxword;
  typedef Elf64_Addr    Addr;
  typedef Elf64_Off     Off;
  typedef Elf64_Section Section;
  typedef Elf64_Versym  Versym;
};

template <class ElfXX>
struct Elf_Ehdr
{
  unsigned char        e_ident[EI_NIDENT]; // Magic number and other info
  typename ElfXX::Half e_type;             // Object file type
  typename ElfXX::Half e_machine;          // Architecture
  typename ElfXX::Word e_version;          // Object file version
  typename ElfXX::Addr e_entry;            // Entry point virtual address
  typename ElfXX::Off  e_phoff;            // Program header table file offset
  typename ElfXX::Off  e_shoff;            // Section header table file offset
  typename ElfXX::Word e_flags;            // Processor-specific flags
  typename ElfXX::Half e_ehsize;           // ELF header size in bytes
  typename ElfXX::Half e_phentsize;        // Program header table entry size
  typename ElfXX::Half e_phnum;            // Program header table entry count
  typename ElfXX::Half e_shentsize;        // Section header table entry size
  typename ElfXX::Half e_shnum;            // Section header table entry count
  typename ElfXX::Half e_shstrndx;         // Section header string table index
};

typedef Elf_Ehdr<elf32_typedefs> Elf32_Ehdr;
typedef Elf_Ehdr<elf64_typedefs> Elf64_Ehdr;

template <class ElfXX> struct Elf_Shdr;

template <>
struct Elf_Shdr<elf32_typedefs>
{
  Elf32_Word sh_name;      // Section name (string tbl index)
  Elf32_Word sh_type;      // Section type
  Elf32_Word sh_flags;     // Section flags
  Elf32_Addr sh_addr;      // Section virtual addr at execution
  Elf32_Off  sh_offset;    // Section file offset
  Elf32_Word sh_size;      // Section size in bytes
  Elf32_Word sh_link;      // Link to another section
  Elf32_Word sh_info;      // Additional section information
  Elf32_Word sh_addralign; // Section alignment
  Elf32_Word sh_entsize;   // Entry size if section holds table
};

template <>
struct Elf_Shdr<elf64_typedefs>
{
  Elf64_Word  sh_name;      // Section name (string tbl index)
  Elf64_Word  sh_type;      // Section type
  Elf64_Xword sh_flags;     // Section flags
  Elf64_Addr  sh_addr;      // Section virtual addr at execution
  Elf64_Off   sh_offset;    // Section file offset
  Elf64_Xword sh_size;      // Section size in bytes
  Elf64_Word  sh_link;      // Link to another section
  Elf64_Word  sh_info;      // Additional section information
  Elf64_Xword sh_addralign; // Section alignment
  Elf64_Xword sh_entsize;   // Entry size if section holds table
};

typedef Elf_Shdr<elf32_typedefs> Elf32_Shdr;
typedef Elf_Shdr<elf64_typedefs> Elf64_Shdr;

template <class ElfXX> struct Elf_Sym;

template <>
struct Elf_Sym<elf32_typedefs>
{
  Elf32_Word    st_name;  // Symbol name (string tbl index)
  Elf32_Addr    st_value; // Symbol value
  Elf32_Word    st_size;  // Symbol size
  unsigned char st_info;  // Symbol type and binding
  unsigned char st_other; // Symbol visibility
  Elf32_Section st_shndx; // Section index
};

template <>
struct Elf_Sym<elf64_typedefs>
{
  Elf64_Word    st_name;  // Symbol name (string tbl index)
  unsigned char st_info;  // Symbol type and binding
  unsigned char st_other; // Symbol visibility
  Elf64_Section st_shndx; // Section index
  Elf64_Addr    st_value; // Symbol value
  Elf64_Xword   st_size;  // Symbol size
};

template <class ElfXX>
struct Elf_Syminfo
{
  typename ElfXX::Half si_boundto; // Direct bindings, symbol bound to
  typename ElfXX::Half si_flags;   // Per symbol flags
};

typedef Elf_Syminfo<elf32_typedefs> Elf32_Syminfo;
typedef Elf_Syminfo<elf64_typedefs> Elf64_Syminfo;

template <class ElfXX> struct Elf_Rel;

template <>
struct Elf_Rel<elf32_typedefs>
{
  Elf32_Addr r_offset; // Address
  Elf32_Word r_info;   // Relocation type and symbol index
};

template <>
struct Elf_Rel<elf64_typedefs>
{
  Elf64_Addr  r_offset; // Address
  Elf64_Xword r_info;   // Relocation type and symbol index
};

typedef Elf_Rel<elf32_typedefs> Elf32_Rel;
typedef Elf_Rel<elf64_typedefs> Elf64_Rel;

template <class ElfXX> struct Elf_Rela;

template <>
struct Elf_Rela<elf32_typedefs>
{
  Elf32_Addr  r_offset; // Address
  Elf32_Word  r_info;   // Relocation type and symbol index
  Elf32_Sword r_addend; // Addend
};

template <>
struct Elf_Rela<elf64_typedefs>
{
  Elf32_Addr   r_offset; // Address
  Elf32_Xword  r_info;   // Relocation type and symbol index
  Elf32_Sxword r_addend; // Addend
};

typedef Elf_Rela<elf32_typedefs> Elf32_Rela;
typedef Elf_Rela<elf64_typedefs> Elf64_Rela;

template <class ElfXX> struct Elf_Phdr;

template <>
struct Elf_Phdr<elf32_typedefs>
{
  Elf32_Word p_type;   // Segment type
  Elf32_Off  p_offset; // Segment file offset
  Elf32_Addr p_vaddr;  // Segment virtual address
  Elf32_Addr p_paddr;  // Segment physical address
  Elf32_Word p_filesz; // Segment size in file
  Elf32_Word p_memsz;  // Segment size in memory
  Elf32_Word p_flags;  // Segment flags
  Elf32_Word p_align;  // Segment alignment
};

template <>
struct Elf_Phdr<elf64_typedefs>
{
  Elf64_Word  p_type;   // Segment type
  Elf64_Word  p_flags;  // Segment flags
  Elf64_Off   p_offset; // Segment file offset
  Elf64_Addr  p_vaddr;  // Segment virtual address
  Elf64_Addr  p_paddr;  // Segment physical address
  Elf64_Xword p_filesz; // Segment size in file
  Elf64_Xword p_memsz;  // Segment size in memory
  Elf64_Xword p_align;  // Segment alignment
};

typedef Elf_Phdr<elf32_typedefs> Elf32_Phdr;
typedef Elf_Phdr<elf64_typedefs> Elf64_Phdr;

template <class ElfXX> struct Elf_Dyn;

template <>
struct Elf_Dyn<elf32_typedefs>
{
  Elf32_Sword  d_tag; // Dynamic entry type
  union {
    Elf32_Word d_val; // Integer value
    Elf32_Addr d_ptr; // Address value
  } d_un;
};

template <>
struct Elf_Dyn<elf64_typedefs>
{
  Elf64_Sxword  d_tag; // Dynamic entry type
  union {
    Elf64_Xword d_val; // Integer value
    Elf64_Addr  d_ptr; // Address value
  } d_un;
};

typedef Elf_Dyn<elf32_typedefs> Elf32_Dyn;
typedef Elf_Dyn<elf64_typedefs> Elf64_Dyn;

template <class ElfXX>
struct Elf_Verdef
{
  typename ElfXX::Half vd_version; // Version revision
  typename ElfXX::Half vd_flags;   // Version information
  typename ElfXX::Half vd_ndx;     // Version index
  typename ElfXX::Half vd_cnt;     // Number of associated aux entries
  typename ElfXX::Word vd_hash;    // Version name hash value
  typename ElfXX::Word vd_aux;     // Offset in bytes to verdaux array
  typename ElfXX::Word vd_next;    // Offset in bytes to next verdef entry
};

typedef Elf_Verdef<elf32_typedefs> Elf32_Verdef;
typedef Elf_Verdef<elf64_typedefs> Elf64_Verdef;

template <class ElfXX>
struct Elf_Verdaux
{
  typename ElfXX::Word vda_name; // Version or dependency names
  typename ElfXX::Word vda_next; // Offset in bytes to next verdaux entry
};

typedef Elf_Verdaux<elf32_typedefs> Elf32_Verdaux;
typedef Elf_Verdaux<elf64_typedefs> Elf64_Verdaux;

template <class ElfXX>
struct Elf_Verneed
{
  typename ElfXX::Half vn_version; // Version of structure
  typename ElfXX::Half vn_cnt;     // Number of associated aux entries
  typename ElfXX::Word vn_file;    // Offset of filename for this dependency
  typename ElfXX::Word vn_aux;     // Offset in bytes to vernaux array
  typename ElfXX::Word vn_next;    // Offset in bytes to next verneed entry
};

typedef Elf_Verneed<elf32_typedefs> Elf32_Verneed;
typedef Elf_Verneed<elf64_typedefs> Elf64_Verneed;

template <class ElfXX>
struct Elf_Vernaux
{
  typename ElfXX::Word vna_hash;  // Hash value of dependency name
  typename ElfXX::Half vna_flags; // Dependency specific information
  typename ElfXX::Half vna_other; // Unused
  typename ElfXX::Word vna_name;  // Dependency name string offset
  typename ElfXX::Word vna_next;  // Offset in bytes to next vernaux entry
};

typedef Elf_Vernaux<elf32_typedefs> Elf32_Vernaux;
typedef Elf_Vernaux<elf64_typedefs> Elf64_Vernaux;

template <class ElfXX>
struct Elf_Nhdr
{
  typename ElfXX::Word n_nasmesz; // Length of the note's name
  typename ElfXX::Word n_descsz;  // Length of the note's descriptor
  typename ElfXX::Word n_type;    // Type of the note
};

typedef Elf_Nhdr<elf32_typedefs> Elf32_Nhdr;
typedef Elf_Nhdr<elf64_typedefs> Elf64_Nhdr;

template <class ElfXX> struct Elf_Move;

template <>
struct Elf_Move<elf32_typedefs>
{
  Elf32_Xword m_value;   // Symbol value
  Elf32_Word  m_info;    // Size and index
  Elf32_Word  m_poffset; // Symbol offset
  Elf32_Half  m_repeat;  // Repeat count
  Elf32_Half  m_stride;  // Stride info
};

template <>
struct Elf_Move<elf64_typedefs>
{
  Elf64_Xword m_value;   // Symbol value
  Elf64_Xword m_info;    // Size and index
  Elf64_Xword m_poffset; // Symbol offset
  Elf64_Half m_repeat;   // Repeat count
  Elf64_Half m_stride;   // Stride info
};

typedef Elf_Move<elf32_typedefs> Elf32_Move;
typedef Elf_Move<elf64_typedefs> Elf64_Move;

} // namespace hdbg

#endif // __HOLYDBG_SRC_BINFMT_ELF_HPP__
