#ifndef __HOLYDBG_BINFMT_FROM_IMAGE_HPP__
#define __HOLYDBG_BINFMT_FROM_IMAGE_HPP__

#include <holydbg/binary_format.hpp>

#include <cstdint>
#include <memory>

namespace hdbg {

class DebugProcess;

HDBG_EXPORT std::unique_ptr<BinaryFormat> binfmt_from_image(const DebugProcess & proc, std::uintptr_t base);

} // namespace hdbg

#endif // __HOLYDBG_BINFMT_FROM_IMAGE_HPP__
