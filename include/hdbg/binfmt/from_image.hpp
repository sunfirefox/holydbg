#ifndef __HDBG_BINFMT_FROM_IMAGE_HPP__
#define __HDBG_BINFMT_FROM_IMAGE_HPP__

#include <hdbg/binary_format.hpp>

#include <cstdint>
#include <memory>

namespace hdbg {

class DebugProcess;

HDBG_EXPORT std::unique_ptr<BinaryFormat>
  binfmt_from_image(const DebugProcess & dbg_proc, std::uintptr_t base);

} // namespace hdbg

#endif // __HDBG_BINFMT_FROM_IMAGE_HPP__
