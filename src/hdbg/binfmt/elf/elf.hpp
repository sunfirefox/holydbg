#ifndef __HDBG_SRC_BINFMT_ELF_ELF_HPP__
#define __HDBG_SRC_BINFMT_ELF_ELF_HPP__

#include <cstdint>

namespace hdbg {

struct elf32_typedefs
{
  typedef std::uint16_t Half;
  typedef std::uint32_t Word;
  typedef std::int32_t  Sword;
  typedef std::uint64_t Xword;
  typedef std::int64_t  Sxword;
  typedef std::uint32_t Addr;
  typedef std::uint32_t Off;
  typedef std::uint16_t Section;
  typedef Half          Versym;
};

struct elf64_typedefs
{
  typedef std::uint16_t Half;
  typedef std::uint32_t Word;
  typedef std::int32_t  Sword;
  typedef std::uint64_t Xword;
  typedef std::int64_t  Sxword;
  typedef std::uint64_t Addr;
  typedef std::uint64_t Off;
  typedef std::uint16_t Section;
  typedef Half          Versym;
};

typedef elf32_typedefs Elf32;
typedef elf64_typedefs Elf64;

constexpr unsigned int EI_NIDENT = 16;

enum {
  EI_MAG0,       // File identification byte 0 index
  EI_MAG1,       // File identification byte 1 index
  EI_MAG2,       // File identification byte 2 index
  EI_MAG3,       // File identification byte 3 index
  EI_CLASS,      // File class byte index
  EI_DATA,       // Data encoding byte index
  EI_VERSION,    // File version byte index
  EI_OSABI,      // OS ABI identification
  EI_ABIVERSION, // ABI version
  EI_PAD,        // Byte index of padding bytes
};

enum {
  ELFMAG0 = 0x7f, // Magic number byte 0
  ELFMAG1 = 'E',  // Magic number byte 1
  ELFMAG2 = 'L',  // Magic number byte 2
  ELFMAG3 = 'F'   // Magic number byte 3
};

// Conglomeration of the identification bytes, for easy testing as a word.
constexpr const char * ELFMAG = "\177ELF";
constexpr unsigned int SELFMAG = 4;

enum {
  ELFCLASSNONE, // Invalid class
  ELFCLASS32,   // 32-bit objects
  ELFCLASS64,   // 64-bit objects
  ELFCLASSNUM
};

enum {
  ELFDATANONE, // Invalid data encoding
  ELFDATA2LSB, // 2's complement, little endian
  ELFDATA2MSB, // 2's complement, big endian
  ELFDATANUM
};

enum {
 ELFOSABI_NONE       = 0,  // UNIX System V ABI
 ELFOSABI_SYSV       = 0,  // Alias.
 ELFOSABI_HPUX       = 1,  // HP-UX
 ELFOSABI_NETBSD     = 2,  // NetBSD.
 ELFOSABI_GNU        = 3,  // Object uses GNU ELF extensions.
 ELFOSABI_LINUX      = 3,  // Compatibility alias.
 ELFOSABI_SOLARIS    = 6,  // Sun Solaris.
 ELFOSABI_AIX        = 7,  // IBM AIX.
 ELFOSABI_IRIX       = 8,  // SGI Irix.
 ELFOSABI_FREEBSD    = 9,  // FreeBSD.
 ELFOSABI_TRU64      = 10, // Compaq TRU64 UNIX.
 ELFOSABI_MODESTO    = 11, // Novell Modesto.
 ELFOSABI_OPENBSD    = 12, // OpenBSD.
 ELFOSABI_ARM_AEABI  = 64, // ARM EABI
 ELFOSABI_ARM        = 97, // ARM
 ELFOSABI_STANDALONE = 255 // Standalone (embedded) application
};

enum { // Legal values for e_type (object file type).
  ET_NONE   = 0,      // No file type
  ET_REL    = 1,      // Relocatable file
  ET_EXEC   = 2,      // Executable file
  ET_DYN    = 3,      // Shared object file
  ET_CORE   = 4,      // Core file
  ET_NUM    = 5,      // Number of defined types
  ET_LOOS   = 0xfe00, // OS-specific range start
  ET_HIOS   = 0xfeff, // OS-specific range end
  ET_LOPROC = 0xff00, // Processor-specific range start
  ET_HIPROC = 0xffff, // Processor-specific range end
};

enum {
  EM_NONE        = 0,   // No machine
  EM_M32         = 1,   // AT&T WE 32100
  EM_SPARC       = 2,   // SUN SPARC
  EM_386         = 3,   // Intel 80386
  EM_68K         = 4,   // Motorola m68k family
  EM_88K         = 5,   // Motorola m88k family
  EM_860         = 7,   // Intel 80860
  EM_MIPS        = 8,   // MIPS R3000 big-endian
  EM_S370        = 9,   // IBM System/370
  EM_MIPS_RS3_LE = 10,  // MIPS R3000 little-endian
  
  EM_PARISC      = 15,  // HPPA
  EM_VPP500      = 17,  // Fujitsu VPP500
  EM_SPARC32PLUS = 18,  // Sun's "v8plus"
  EM_960         = 19,  // Intel 80960
  EM_PPC         = 20,  // PowerPC
  EM_PPC64       = 21,  // PowerPC 64-bit
  EM_S390        = 22,  // IBM S390
  
  EM_V800        = 36,  // NEC V800 series
  EM_FR20        = 37,  // Fujitsu FR20
  EM_RH32        = 38,  // TRW RH-32
  EM_RCE         = 39,  // Motorola RCE
  EM_ARM         = 40,  // ARM
  EM_FAKE_ALPHA  = 41,  // Digital Alpha
  EM_SH          = 42,  // Hitachi SH
  EM_SPARCV9     = 43,  // SPARC v9 64-bit
  EM_TRICORE     = 44,  // Siemens Tricore
  EM_ARC         = 45,  // Argonaut RISC Core
  EM_H8_300      = 46,  // Hitachi H8/300
  EM_H8_300H     = 47,  // Hitachi H8/300H
  EM_H8S         = 48,  // Hitachi H8S
  EM_H8_500      = 49,  // Hitachi H8/500
  EM_IA_64       = 50,  // Intel Merced
  EM_MIPS_X      = 51,  // Stanford MIPS-X
  EM_COLDFIRE    = 52,  // Motorola Coldfire
  EM_68HC12      = 53,  // Motorola M68HC12
  EM_MMA         = 54,  // Fujitsu MMA Multimedia Accelerator
  EM_PCP         = 55,  // Siemens PCP
  EM_NCPU        = 56,  // Sony nCPU embeeded RISC
  EM_NDR1        = 57,  // Denso NDR1 microprocessor
  EM_STARCORE    = 58,  // Motorola Start*Core processor
  EM_ME16        = 59,  // Toyota ME16 processor
  EM_ST100       = 60,  // STMicroelectronic ST100 processor
  EM_TINYJ       = 61,  // Advanced Logic Corp. Tinyj emb.fam
  EM_X86_64      = 62,  // AMD x86-64 architecture
  EM_PDSP        = 63,  // Sony DSP Processor
  
  EM_FX66        = 66,  // Siemens FX66 microcontroller
  EM_ST9PLUS     = 67,  // STMicroelectronics ST9+ 8/16 mc
  EM_ST7         = 68,  // STmicroelectronics ST7 8 bit mc
  EM_68HC16      = 69,  // Motorola MC68HC16 microcontroller
  EM_68HC11      = 70,  // Motorola MC68HC11 microcontroller
  EM_68HC08      = 71,  // Motorola MC68HC08 microcontroller
  EM_68HC05      = 72,  // Motorola MC68HC05 microcontroller
  EM_SVX         = 73,  // Silicon Graphics SVx
  EM_ST19        = 74,  // STMicroelectronics ST19 8 bit mc
  EM_VAX         = 75,  // Digital VAX
  EM_CRIS        = 76,  // Axis Communications 32-bit embedded processor
  EM_JAVELIN     = 77,  // Infineon Technologies 32-bit embedded processor
  EM_FIREPATH    = 78,  // Element 14 64-bit DSP Processor
  EM_ZSP         = 79,  // LSI Logic 16-bit DSP Processor
  EM_MMIX        = 80,  // Donald Knuth's educational 64-bit processor
  EM_HUANY       = 81,  // Harvard University machine-independent object files
  EM_PRISM       = 82,  // SiTera Prism
  EM_AVR         = 83,  // Atmel AVR 8-bit microcontroller
  EM_FR30        = 84,  // Fujitsu FR30
  EM_D10V        = 85,  // Mitsubishi D10V
  EM_D30V        = 86,  // Mitsubishi D30V
  EM_V850        = 87,  // NEC v850
  EM_M32R        = 88,  // Mitsubishi M32R
  EM_MN10300     = 89,  // Matsushita MN10300
  EM_MN10200     = 90,  // Matsushita MN10200
  EM_PJ          = 91,  // picoJava
  EM_OPENRISC    = 92,  // OpenRISC 32-bit embedded processor
  EM_ARC_A5      = 93,  // ARC Cores Tangent-A5
  EM_XTENSA      = 94,  // Tensilica Xtensa Architecture
  EM_AARCH64     = 183, // ARM AARCH64
  EM_TILEPRO     = 188, // Tilera TILEPro
  EM_MICROBLAZE  = 189, // Xilinx MicroBlaze
  EM_TILEGX      = 191, // Tilera TILE-Gx
  EM_NUM         = 192,
  
  EM_ALPHA       = 0x9026
};

enum { // Legal values for e_version (version).
  EV_NONE    = 0, // Invalid ELF version
  EV_CURRENT = 1, // Current version
  EV_NUM     = 2,
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

typedef Elf_Ehdr<Elf32> Elf32_Ehdr;
typedef Elf_Ehdr<Elf64> Elf64_Ehdr;

enum { // Special section indices.
  SHN_UNDEF     = 0,      // Undefined section
  SHN_LORESERVE = 0xff00, // Start of reserved indices
  SHN_LOPROC    = 0xff00, // Start of processor-specific
  SHN_BEFORE    = 0xff00, // Order section before all others (Solaris).
  SHN_AFTER     = 0xff01, // Order section after all others (Solaris).
  SHN_HIPROC    = 0xff1f, // End of processor-specific
  SHN_LOOS      = 0xff20, // Start of OS-specific
  SHN_HIOS      = 0xff3f, // End of OS-specific
  SHN_ABS       = 0xfff1, // Associated symbol is absolute
  SHN_COMMON    = 0xfff2, // Associated symbol is common
  SHN_XINDEX    = 0xffff, // Index is in extra table.
  SHN_HIRESERVE = 0xffff, // End of reserved indices
};

enum { // Legal values for sh_type (section type).
  SHT_NULL           = 0,          // Section header table entry unused
  SHT_PROGBITS       = 1,          // Program data
  SHT_SYMTAB         = 2,          // Symbol table
  SHT_STRTAB         = 3,          // String table
  SHT_RELA           = 4,          // Relocation entries with addends
  SHT_HASH           = 5,          // Symbol hash table
  SHT_DYNAMIC        = 6,          // Dynamic linking information
  SHT_NOTE           = 7,          // Notes
  SHT_NOBITS         = 8,          // Program space with no data (bss)
  SHT_REL            = 9,          // Relocation entries, no addends
  SHT_SHLIB          = 10,         // Reserved
  SHT_DYNSYM         = 11,         // Dynamic linker symbol table
  SHT_INIT_ARRAY     = 14,         // Array of constructors
  SHT_FINI_ARRAY     = 15,         // Array of destructors
  SHT_PREINIT_ARRAY  = 16,         // Array of pre-constructors
  SHT_GROUP          = 17,         // Section group
  SHT_SYMTAB_SHNDX   = 18,         // Extended section indeces
  SHT_NUM            = 19,         // Number of defined types.
  SHT_LOOS           = 0x60000000, // Start OS-specific.
  SHT_GNU_ATTRIBUTES = 0x6ffffff5, // Object attributes.
  SHT_GNU_HASH       = 0x6ffffff6, // GNU-style hash table.
  SHT_GNU_LIBLIST    = 0x6ffffff7, // Prelink library list
  SHT_CHECKSUM       = 0x6ffffff8, // Checksum for DSO content.
  SHT_LOSUNW         = 0x6ffffffa, // Sun-specific low bound.
  SHT_SUNW_move      = 0x6ffffffa,
  SHT_SUNW_COMDAT    = 0x6ffffffb,
  SHT_SUNW_syminfo   = 0x6ffffffc,
  SHT_GNU_verdef     = 0x6ffffffd, // Version definition section.
  SHT_GNU_verneed    = 0x6ffffffe, // Version needs section.
  SHT_GNU_versym     = 0x6fffffff, // Version symbol table.
  SHT_HISUNW         = 0x6fffffff, // Sun-specific high bound.
  SHT_HIOS           = 0x6fffffff, // End OS-specific type
  SHT_LOPROC         = 0x70000000, // Start of processor-specific
  SHT_HIPROC         = 0x7fffffff, // End of processor-specific
  SHT_LOUSER         = 0x80000000, // Start of application-specific
  SHT_HIUSER         = 0x8fffffff, // End of application-specific
};

enum { // Legal values for sh_flags (section flags).
  SHF_WRITE            = 1 << 0,     // Writable
  SHF_ALLOC            = 1 << 1,     // Occupies memory during execution
  SHF_EXECINSTR        = 1 << 2,     // Executable
  SHF_MERGE            = 1 << 4,     // Might be merged
  SHF_STRINGS          = 1 << 5,     // Contains nul-terminated strings
  SHF_INFO_LINK        = 1 << 6,     // `sh_info' contains SHT index
  SHF_LINK_ORDER       = 1 << 7,     // Preserve order after combining
  SHF_OS_NONCONFORMING = 1 << 8,     // Non-standard OS specific handling required
  SHF_GROUP            = 1 << 9,     // Section is member of a group.
  SHF_TLS              = 1 << 10,    // Section hold thread-local data.
  SHF_MASKOS           = 0x0ff00000, // OS-specific.
  SHF_MASKPROC         = 0xf0000000, // Processor-specific
  SHF_ORDERED          = 1 << 30,    // Special ordering requirement (Solaris).
  SHF_EXCLUDE          = 1 << 31,    // Section is excluded unless referenced or allocated (Solaris).
};

enum {
  GRP_COMDAT = 0x1, // Mark group as COMDAT.
};

template <class ElfXX> struct Elf_Shdr;

template <>
struct Elf_Shdr<Elf32>
{
  Elf32::Word sh_name;      // Section name (string tbl index)
  Elf32::Word sh_type;      // Section type
  Elf32::Word sh_flags;     // Section flags
  Elf32::Addr sh_addr;      // Section virtual addr at execution
  Elf32::Off  sh_offset;    // Section file offset
  Elf32::Word sh_size;      // Section size in bytes
  Elf32::Word sh_link;      // Link to another section
  Elf32::Word sh_info;      // Additional section information
  Elf32::Word sh_addralign; // Section alignment
  Elf32::Word sh_entsize;   // Entry size if section holds table
};

template <>
struct Elf_Shdr<Elf64>
{
  Elf64::Word  sh_name;      // Section name (string tbl index)
  Elf64::Word  sh_type;      // Section type
  Elf64::Xword sh_flags;     // Section flags
  Elf64::Addr  sh_addr;      // Section virtual addr at execution
  Elf64::Off   sh_offset;    // Section file offset
  Elf64::Xword sh_size;      // Section size in bytes
  Elf64::Word  sh_link;      // Link to another section
  Elf64::Word  sh_info;      // Additional section information
  Elf64::Xword sh_addralign; // Section alignment
  Elf64::Xword sh_entsize;   // Entry size if section holds table
};

typedef Elf_Shdr<Elf32> Elf32_Shdr;
typedef Elf_Shdr<Elf64> Elf64_Shdr;

template <class ElfXX> struct Elf_Sym; // Symbol table entry

template <>
struct Elf_Sym<Elf32>
{
  Elf32::Word    st_name;  // Symbol name (string tbl index)
  Elf32::Addr    st_value; // Symbol value
  Elf32::Word    st_size;  // Symbol size
  unsigned char st_info;   // Symbol type and binding
  unsigned char st_other;  // Symbol visibility
  Elf32::Section st_shndx; // Section index
};

template <>
struct Elf_Sym<Elf64>
{
  Elf64::Word    st_name;  // Symbol name (string tbl index)
  unsigned char  st_info;  // Symbol type and binding
  unsigned char  st_other; // Symbol visibility
  Elf64::Section st_shndx; // Section index
  Elf64::Addr    st_value; // Symbol value
  Elf64::Xword   st_size;  // Symbol size
};

enum {
  SYMINFO_BT_SELF       = 0xffff, // Symbol bound to self
  SYMINFO_BT_PARENT     = 0xfffe, // Symbol bound to parent
  SYMINFO_BT_LOWRESERVE = 0xff00, // Beginning of reserved entries
};

enum { // Possible bitmasks for si_flags.
  SYMINFO_FLG_DIRECT   = 0x0001, // Direct bound symbol
  SYMINFO_FLG_PASSTHRU = 0x0002, // Pass-thru symbol for translator
  SYMINFO_FLG_COPY     = 0x0004, // Symbol is a copy-reloc
  SYMINFO_FLG_LAZYLOAD = 0x0008, // Symbol bound to object to be lazy loaded
};

enum { // Syminfo version values.
  SYMINFO_NONE    = 0,
  SYMINFO_CURRENT = 1,
  SYMINFO_NUM     = 2,
};

enum { // Legal values for ST_BIND subfield of st_info (symbol binding).
  STB_LOCAL      = 0,  // Local symbol
  STB_GLOBAL     = 1,  // Global symbol
  STB_WEAK       = 2,  // Weak symbol
  STB_NUM        = 3,  // Number of defined types.
  STB_LOOS       = 10, // Start of OS-specific
  STB_GNU_UNIQUE = 10, // Unique symbol.
  STB_HIOS       = 12, // End of OS-specific
  STB_LOPROC     = 13, // Start of processor-specific
  STB_HIPROC     = 15, // End of processor-specific
};

enum { // Legal values for ST_TYPE subfield of st_info (symbol type).
  STT_NOTYPE    = 0,  // Symbol type is unspecified
  STT_OBJECT    = 1,  // Symbol is a data object
  STT_FUNC      = 2,  // Symbol is a code object
  STT_SECTION   = 3,  // Symbol associated with a section
  STT_FILE      = 4,  // Symbol's name is file name
  STT_COMMON    = 5,  // Symbol is a common data object
  STT_TLS       = 6,  // Symbol is thread-local data object
  STT_NUM       = 7,  // Number of defined types.
  STT_LOOS      = 10, // Start of OS-specific
  STT_GNU_IFUNC = 10, // Symbol is indirect code object
  STT_HIOS      = 12, // End of OS-specific
  STT_LOPROC    = 13, // Start of processor-specific
  STT_HIPROC    = 15, // End of processor-specific
};

// Symbol table indices are found in the hash buckets and chain table
// of a symbol hash table section.  This special index value indicates
// the end of a chain, meaning no further symbols are found in that bucket.
enum {
  STN_UNDEF = 0, // End of a chain.
};


enum { // Symbol visibility specification encoded in the st_other field.
  STV_DEFAULT   = 0, // Default symbol visibility rules
  STV_INTERNAL  = 1, // Processor specific hidden class
  STV_HIDDEN    = 2, // Sym unavailable in other modules
  STV_PROTECTED = 3, // Not preemptible, not exported
};

// The syminfo section if available contains additional information about
// every dynamic symbol.

template <class ElfXX>
struct Elf_Syminfo
{
  typename ElfXX::Half si_boundto; // Direct bindings, symbol bound to
  typename ElfXX::Half si_flags;   // Per symbol flags
};

typedef Elf_Syminfo<Elf32> Elf32_Syminfo;
typedef Elf_Syminfo<Elf64> Elf64_Syminfo;

template <class ElfXX> struct Elf_Rel;

template <>
struct Elf_Rel<Elf32>
{
  Elf32::Addr r_offset; // Address
  Elf32::Word r_info;   // Relocation type and symbol index
};

template <>
struct Elf_Rel<Elf64>
{
  Elf64::Addr  r_offset; // Address
  Elf64::Xword r_info;   // Relocation type and symbol index
};

typedef Elf_Rel<Elf32> Elf32_Rel;
typedef Elf_Rel<Elf64> Elf64_Rel;

template <class ElfXX> struct Elf_Rela;

template <>
struct Elf_Rela<Elf32>
{
  Elf32::Addr  r_offset; // Address
  Elf32::Word  r_info;   // Relocation type and symbol index
  Elf32::Sword r_addend; // Addend
};

template <>
struct Elf_Rela<Elf64>
{
  Elf32::Addr   r_offset; // Address
  Elf32::Xword  r_info;   // Relocation type and symbol index
  Elf32::Sxword r_addend; // Addend
};

typedef Elf_Rela<Elf32> Elf32_Rela;
typedef Elf_Rela<Elf64> Elf64_Rela;

template <class ElfXX> struct Elf_Phdr;

template <>
struct Elf_Phdr<Elf32>
{
  Elf32::Word p_type;   // Segment type
  Elf32::Off  p_offset; // Segment file offset
  Elf32::Addr p_vaddr;  // Segment virtual address
  Elf32::Addr p_paddr;  // Segment physical address
  Elf32::Word p_filesz; // Segment size in file
  Elf32::Word p_memsz;  // Segment size in memory
  Elf32::Word p_flags;  // Segment flags
  Elf32::Word p_align;  // Segment alignment
};

template <>
struct Elf_Phdr<Elf64>
{
  Elf64::Word  p_type;   // Segment type
  Elf64::Word  p_flags;  // Segment flags
  Elf64::Off   p_offset; // Segment file offset
  Elf64::Addr  p_vaddr;  // Segment virtual address
  Elf64::Addr  p_paddr;  // Segment physical address
  Elf64::Xword p_filesz; // Segment size in file
  Elf64::Xword p_memsz;  // Segment size in memory
  Elf64::Xword p_align;  // Segment alignment
};

typedef Elf_Phdr<Elf32> Elf32_Phdr;
typedef Elf_Phdr<Elf64> Elf64_Phdr;

// Special value for e_phnum.  This indicates that the real number of
// program headers is too large to fit into e_phnum.  Instead the real
// value is in the field sh_info of section 0.
enum {
  PN_XNUM = 0xffff,
};

enum { // Legal values for p_type (segment type).
  PT_NULL         = 0,          // Program header table entry unused
  PT_LOAD         = 1,          // Loadable program segment
  PT_DYNAMIC      = 2,          // Dynamic linking information
  PT_INTERP       = 3,          // Program interpreter
  PT_NOTE         = 4,          // Auxiliary information
  PT_SHLIB        = 5,          // Reserved
  PT_PHDR         = 6,          // Entry for header table itself
  PT_TLS          = 7,          // Thread-local storage segment
  PT_NUM          = 8,          // Number of defined types
  PT_LOOS         = 0x60000000, // Start of OS-specific
  PT_GNU_EH_FRAME = 0x6474e550, // GCC .eh_frame_hdr segment
  PT_GNU_STACK    = 0x6474e551, // Indicates stack executability
  PT_GNU_RELRO    = 0x6474e552, // Read-only after relocation
  PT_LOSUNW       = 0x6ffffffa,
  PT_SUNWBSS      = 0x6ffffffa, // Sun Specific segment
  PT_SUNWSTACK    = 0x6ffffffb, // Stack segment
  PT_HISUNW       = 0x6fffffff,
  PT_HIOS         = 0x6fffffff, // End of OS-specific
  PT_LOPROC       = 0x70000000, // Start of processor-specific
  PT_HIPROC       = 0x7fffffff, // End of processor-specific
};

enum {
  PF_X        = 1 << 0,     // Segment is executable
  PF_W        = 1 << 1,     // Segment is writable
  PF_R        = 1 << 2,     // Segment is readable
  PF_MASKOS   = 0x0ff00000, // OS-specific
  PF_MASKPROC = 0xf0000000, // Processor-specific
};

enum { // Legal values for note segment descriptor types for core files.
  NT_PRSTATUS         = 1,          // Contains copy of prstatus struct
  NT_FPREGSET         = 2,          // Contains copy of fpregset struct
  NT_PRPSINFO         = 3,          // Contains copy of prpsinfo struct
  NT_PRXREG           = 4,          // Contains copy of prxregset struct
  NT_TASKSTRUCT       = 4,          // Contains copy of task structure
  NT_PLATFORM         = 5,          // String from sysinfo(SI_PLATFORM)
  NT_AUXV             = 6,          // Contains copy of auxv array
  NT_GWINDOWS         = 7,          // Contains copy of gwindows struct
  NT_ASRS             = 8,          // Contains copy of asrset struct
  NT_PSTATUS          = 10,         // Contains copy of pstatus struct
  NT_PSINFO           = 13,         // Contains copy of psinfo struct
  NT_PRCRED           = 14,         // Contains copy of prcred struct
  NT_UTSNAME          = 15,         // Contains copy of utsname struct
  NT_LWPSTATUS        = 16,         // Contains copy of lwpstatus struct
  NT_LWPSINFO         = 17,         // Contains copy of lwpinfo struct
  NT_PRFPXREG         = 20,         // Contains copy of fprxregset struct
  NT_SIGINFO          = 0x53494749, // Contains copy of siginfo_t, size might increase
  NT_FILE             = 0x46494c45, // Contains information about mapped files
  NT_PRXFPREG         = 0x46e62b7f, // Contains copy of user_fxsr_struct
  NT_PPC_VMX          = 0x100,      // PowerPC Altivec/VMX registers
  NT_PPC_SPE          = 0x101,      // PowerPC SPE/EVR registers
  NT_PPC_VSX          = 0x102,      // PowerPC VSX registers
  NT_386_TLS          = 0x200,      // i386 TLS slots (struct user_desc)
  NT_386_IOPERM       = 0x201,      // x86 io permission bitmap (1=deny)
  NT_X86_XSTATE       = 0x202,      // x86 extended state using xsave
  NT_S390_HIGH_GPRS   = 0x300,      // s390 upper register halves
  NT_S390_TIMER       = 0x301,      // s390 timer register
  NT_S390_TODCMP      = 0x302,      // s390 TOD clock comparator register
  NT_S390_TODPREG     = 0x303,      // s390 TOD programmable register
  NT_S390_CTRS        = 0x304,      // s390 control registers
  NT_S390_PREFIX      = 0x305,      // s390 prefix register
  NT_S390_LAST_BREAK  = 0x306,      // s390 breaking event address
  NT_S390_SYSTEM_CALL = 0x307,      // s390 system call restart data
  NT_S390_TDB         = 0x308,      // s390 transaction diagnostic block
  NT_ARM_VFP          = 0x400,      // ARM VFP/NEON registers
  NT_ARM_TLS          = 0x401,      // ARM TLS register
  NT_ARM_HW_BREAK     = 0x402,      // ARM hardware breakpoint registers
  NT_ARM_HW_WATCH     = 0x403,      // ARM hardware watchpoint registers
};

enum { // Legal values for the note segment descriptor types for object files.
  NT_VERSION = 1, // Contains a version string.
};


template <class ElfXX> struct Elf_Dyn;

template <>
struct Elf_Dyn<Elf32>
{
  Elf32::Sword  d_tag; // Dynamic entry type
  union {
    Elf32::Word d_val; // Integer value
    Elf32::Addr d_ptr; // Address value
  } d_un;
};

template <>
struct Elf_Dyn<Elf64>
{
  Elf64::Sxword  d_tag; // Dynamic entry type
  union {
    Elf64::Xword d_val; // Integer value
    Elf64::Addr  d_ptr; // Address value
  } d_un;
};

typedef Elf_Dyn<Elf32> Elf32_Dyn;
typedef Elf_Dyn<Elf64> Elf64_Dyn;

enum { // Legal values for d_tag (dynamic entry type).
  DT_NULL            = 0,          // Marks end of dynamic section
  DT_NEEDED          = 1,          // Name of needed library
  DT_PLTRELSZ        = 2,          // Size in bytes of PLT relocs
  DT_PLTGOT          = 3,          // Processor defined value
  DT_HASH            = 4,          // Address of symbol hash table
  DT_STRTAB          = 5,          // Address of string table
  DT_SYMTAB          = 6,          // Address of symbol table
  DT_RELA            = 7,          // Address of Rela relocs
  DT_RELASZ          = 8,          // Total size of Rela relocs
  DT_RELAENT         = 9,          // Size of one Rela reloc
  DT_STRSZ           = 10,         // Size of string table
  DT_SYMENT          = 11,         // Size of one symbol table entry
  DT_INIT            = 12,         // Address of init function
  DT_FINI            = 13,         // Address of termination function
  DT_SONAME          = 14,         // Name of shared object
  DT_RPATH           = 15,         // Library search path (deprecated)
  DT_SYMBOLIC        = 16,         // Start symbol search here
  DT_REL             = 17,         // Address of Rel relocs
  DT_RELSZ           = 18,         // Total size of Rel relocs
  DT_RELENT          = 19,         // Size of one Rel reloc
  DT_PLTREL          = 20,         // Type of reloc in PLT
  DT_DEBUG           = 21,         // For debugging; unspecified
  DT_TEXTREL         = 22,         // Reloc might modify .text
  DT_JMPREL          = 23,         // Address of PLT relocs
  DT_BIND_NOW        = 24,         // Process relocations of object
  DT_INIT_ARRAY      = 25,         // Array with addresses of init fct
  DT_FINI_ARRAY      = 26,         // Array with addresses of fini fct
  DT_INIT_ARRAYSZ    = 27,         // Size in bytes of DT_INIT_ARRAY
  DT_FINI_ARRAYSZ    = 28,         // Size in bytes of DT_FINI_ARRAY
  DT_RUNPATH         = 29,         // Library search path
  DT_FLAGS           = 30,         // Flags for the object being loaded
  DT_ENCODING        = 32,         // Start of encoded range
  DT_PREINIT_ARRAY   = 32,         // Array with addresses of preinit fct
  DT_PREINIT_ARRAYSZ = 33,         // size in bytes of DT_PREINIT_ARRAY
  DT_NUM             = 34,         // Number used
  DT_LOOS            = 0x6000000d, // Start of OS-specific
  DT_HIOS            = 0x6ffff000, // End of OS-specific
  DT_LOPROC          = 0x70000000, // Start of processor-specific
  DT_HIPROC          = 0x7fffffff, // End of processor-specific
  DT_PROCNUM         = 0x35,       // Most used by any processor
};

// DT_* entries which fall between DT_VALRNGHI & DT_VALRNGLO use the
// Dyn.d_un.d_val field of the Elf*_Dyn structure.  This follows Sun's
// approach.
enum {
  DT_VALRNGLO       = 0x6ffffd00,
  DT_GNU_PRELINKED  = 0x6ffffdf5, // Prelinking timestamp
  DT_GNU_CONFLICTSZ = 0x6ffffdf6, // Size of conflict section
  DT_GNU_LIBLISTSZ  = 0x6ffffdf7, // Size of library list
  DT_CHECKSUM       = 0x6ffffdf8,
  DT_PLTPADSZ       = 0x6ffffdf9,
  DT_MOVEENT        = 0x6ffffdfa,
  DT_MOVESZ         = 0x6ffffdfb,
  DT_FEATURE_1      = 0x6ffffdfc, // Feature selection (DTF_*).
  DT_POSFLAG_1      = 0x6ffffdfd, // Flags for DT_* entries, effecting the following DT_* entry.
  DT_SYMINSZ        = 0x6ffffdfe, // Size of syminfo table (in bytes)
  DT_SYMINENT       = 0x6ffffdff, // Entry size of syminfo
  DT_VALRNGHI       = 0x6ffffdff,
  DT_VALNUM         = 12,
};

// DT_* entries which fall between DT_ADDRRNGHI & DT_ADDRRNGLO use the
// Dyn.d_un.d_ptr field of the Elf*_Dyn structure.
// 
//  If any adjustment is made to the ELF object after it has been
// built these entries will need to be adjusted. 
enum {
  DT_ADDRRNGLO    = 0x6ffffe00,
  DT_GNU_HASH     = 0x6ffffef5, // GNU-style hash table.
  DT_TLSDESC_PLT  = 0x6ffffef6,
  DT_TLSDESC_GOT  = 0x6ffffef7,
  DT_GNU_CONFLICT = 0x6ffffef8, // Start of conflict section
  DT_GNU_LIBLIST  = 0x6ffffef9, // Library list
  DT_CONFIG       = 0x6ffffefa, // Configuration information.
  DT_DEPAUDIT     = 0x6ffffefb, // Dependency auditing.
  DT_AUDIT        = 0x6ffffefc, // Object auditing.
  DT_PLTPAD       = 0x6ffffefd, // PLT padding.
  DT_MOVETAB      = 0x6ffffefe, // Move table.
  DT_SYMINFO      = 0x6ffffeff, // Syminfo table.
  DT_ADDRRNGHI    = 0x6ffffeff,
  DT_ADDRNUM      = 11
};

// The versioning entry types.  The next are defined as part of the
// GNU extension.
enum {
  DT_VERSYM    = 0x6ffffff0,
  DT_RELACOUNT = 0x6ffffff9,
  DT_RELCOUNT  = 0x6ffffffa,
};

enum { // These were chosen by Sun.
  DT_FLAGS_1       = 0x6ffffffb, // State flags, see DF_1_* below.
  DT_VERDEF        = 0x6ffffffc, // Address of version definition table
  DT_VERDEFNUM     = 0x6ffffffd, // Number of version definitions
  DT_VERNEED       = 0x6ffffffe, // Address of table with needed versions
  DT_VERNEEDNUM    = 0x6fffffff, // Number of needed versions
  DT_VERSIONTAGNUM = 16
};

// Sun added these machine-independent extensions in the "processor-specific"
// range. Be compatible.
enum {
  DT_AUXILIARY = 0x7ffffffd, // Shared object to load before self
  DT_FILTER    = 0x7fffffff, // Shared object to get values from
  DT_EXTRANUM  = 3
};

enum { // Values of `d_un.d_val' in the DT_FLAGS entry.
  DF_ORIGIN     = 0x00000001, // Object may use DF_ORIGIN
  DF_SYMBOLIC   = 0x00000002, // Symbol resolutions starts here
  DF_TEXTREL    = 0x00000004, // Object contains text relocations
  DF_BIND_NOW   = 0x00000008, // No lazy binding for this object
  DF_STATIC_TLS = 0x00000010, // Module uses the static TLS model
};

// State flags selectable in the `d_un.d_val' element of the DT_FLAGS_1
// entry in the dynamic section. 
enum {
  DF_1_NOW        = 0x00000001, // Set RTLD_NOW for this object. 
  DF_1_GLOBAL     = 0x00000002, // Set RTLD_GLOBAL for this object. 
  DF_1_GROUP      = 0x00000004, // Set RTLD_GROUP for this object. 
  DF_1_NODELETE   = 0x00000008, // Set RTLD_NODELETE for this object.*/
  DF_1_LOADFLTR   = 0x00000010, // Trigger filtee loading at runtime.*/
  DF_1_INITFIRST  = 0x00000020, // Set RTLD_INITFIRST for this object*/
  DF_1_NOOPEN     = 0x00000040, // Set RTLD_NOOPEN for this object. 
  DF_1_ORIGIN     = 0x00000080, // $ORIGIN must be handled. 
  DF_1_DIRECT     = 0x00000100, // Direct binding enabled. 
  DF_1_TRANS      = 0x00000200,
  DF_1_INTERPOSE  = 0x00000400, // Object is used to interpose. 
  DF_1_NODEFLIB   = 0x00000800, // Ignore default lib search path. 
  DF_1_NODUMP     = 0x00001000, // Object can't be dldump'ed. 
  DF_1_CONFALT    = 0x00002000, // Configuration alternative created.*/
  DF_1_ENDFILTEE  = 0x00004000, // Filtee terminates filters search.
  DF_1_DISPRELDNE = 0x00008000, // Disp reloc applied at build time.
  DF_1_DISPRELPND = 0x00010000, // Disp reloc applied at run-time. 
  DF_1_NODIRECT   = 0x00020000, // Object has no-direct binding.
  DF_1_IGNMULDEF  = 0x00040000,
  DF_1_NOKSYMS    = 0x00080000,
  DF_1_NOHDR      = 0x00100000,
  DF_1_EDITED     = 0x00200000, // Object is modified after built. 
  DF_1_NORELOC    = 0x00400000,
  DF_1_SYMINTPOSE = 0x00800000, // Object has individual interposers. 
  DF_1_GLOBAUDIT  = 0x01000000, // Global auditin required. 
  DF_1_SINGLETON  = 0x02000000, // Singleton symbols are used. 
};

enum { // Flags for the feature selection in DT_FEATURE_1.
  DTF_1_PARINIT = 0x00000001,
  DTF_1_CONFEXP = 0x00000002,
};

enum { // Flags in the DT_POSFLAG_1 entry effecting only the next DT_* entry.
  DF_P1_LAZYLOAD  = 0x00000001, // Lazyload following object.
  DF_P1_GROUPPERM = 0x00000002, // Symbols from next object are not generally available.
};

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

typedef Elf_Verdef<Elf32> Elf32_Verdef;
typedef Elf_Verdef<Elf64> Elf64_Verdef;

enum { // Legal values for vd_version (version revision).
  VER_DEF_NONE    = 0, // No version
  VER_DEF_CURRENT = 1, // Current version
  VER_DEF_NUM     = 2, // Given version number
};

enum { // Legal values for vd_flags (version information flags).
  VER_FLG_BASE = 0x1, // Version definition of file itself
  VER_FLG_WEAK = 0x2, // Weak version identifier
};

enum { // Versym symbol index values.
  VER_NDX_LOCAL     = 0,      // Symbol is local.
  VER_NDX_GLOBAL    = 1,      // Symbol is global.
  VER_NDX_LORESERVE = 0xff00, // Beginning of reserved entries.
  VER_NDX_ELIMINATE = 0xff01, // Symbol is to be eliminated.
};

template <class ElfXX>
struct Elf_Verdaux
{
  typename ElfXX::Word vda_name; // Version or dependency names
  typename ElfXX::Word vda_next; // Offset in bytes to next verdaux entry
};

typedef Elf_Verdaux<Elf32> Elf32_Verdaux;
typedef Elf_Verdaux<Elf64> Elf64_Verdaux;

template <class ElfXX>
struct Elf_Verneed
{
  typename ElfXX::Half vn_version; // Version of structure
  typename ElfXX::Half vn_cnt;     // Number of associated aux entries
  typename ElfXX::Word vn_file;    // Offset of filename for this dependency
  typename ElfXX::Word vn_aux;     // Offset in bytes to vernaux array
  typename ElfXX::Word vn_next;    // Offset in bytes to next verneed entry
};

typedef Elf_Verneed<Elf32> Elf32_Verneed;
typedef Elf_Verneed<Elf64> Elf64_Verneed;

enum { // Legal values for vn_version (version revision).
  VER_NEED_NONE    = 0, // No version
  VER_NEED_CURRENT = 1, // Current version
  VER_NEED_NUM     = 2, // Given version number
};

template <class ElfXX>
struct Elf_Vernaux
{
  typename ElfXX::Word vna_hash;  // Hash value of dependency name
  typename ElfXX::Half vna_flags; // Dependency specific information
  typename ElfXX::Half vna_other; // Unused
  typename ElfXX::Word vna_name;  // Dependency name string offset
  typename ElfXX::Word vna_next;  // Offset in bytes to next vernaux entry
};

typedef Elf_Vernaux<Elf32> Elf32_Vernaux;
typedef Elf_Vernaux<Elf64> Elf64_Vernaux;

template <class ElfXX>
struct Elf_Nhdr
{
  typename ElfXX::Word n_nasmesz; // Length of the note's name
  typename ElfXX::Word n_descsz;  // Length of the note's descriptor
  typename ElfXX::Word n_type;    // Type of the note
};

typedef Elf_Nhdr<Elf32> Elf32_Nhdr;
typedef Elf_Nhdr<Elf64> Elf64_Nhdr;

template <class ElfXX> struct Elf_Move;

template <>
struct Elf_Move<Elf32>
{
  Elf32::Xword m_value;   // Symbol value
  Elf32::Word  m_info;    // Size and index
  Elf32::Word  m_poffset; // Symbol offset
  Elf32::Half  m_repeat;  // Repeat count
  Elf32::Half  m_stride;  // Stride info
};

template <>
struct Elf_Move<Elf64>
{
  Elf64::Xword m_value;   // Symbol value
  Elf64::Xword m_info;    // Size and index
  Elf64::Xword m_poffset; // Symbol offset
  Elf64::Half  m_repeat;  // Repeat count
  Elf64::Half  m_stride;  // Stride info
};

typedef Elf_Move<Elf32> Elf32_Move;
typedef Elf_Move<Elf64> Elf64_Move;

} // namespace hdbg

#endif // __HDBG_SRC_BINFMT_ELF_ELF_HPP__
