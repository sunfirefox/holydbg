#include <holydbg/binfmt/from_image.hpp>

#include "elf/from_image/elf_image.hpp"

namespace hdbg {

std::unique_ptr<BinaryFormat> binfmt_from_image(const DebugProcess & proc, std::uintptr_t base)
{
  if(elf_check_image(proc, base))
    return make_elf_image(proc, base);
  return nullptr;
}

} // namespace hdbg
