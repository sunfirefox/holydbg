#include <gtest/gtest.h>

#include <holydbg/this_process.hpp>
#include <holydbg/binfmt/from_image.hpp>
#include <holydbg/dbg/local/debug_process.hpp>

hdbg::LocalDebugProcess self ( hdbg::this_process::id(), 
                               hdbg::DebugProcess::OpenFlags::MemRead  |
                               hdbg::DebugProcess::OpenFlags::MemWrite );

TEST(BinaryFormatImageTest, FromSelfImage) {
  auto self_img = hdbg::binfmt_from_image(self, self.image_base());
  ASSERT_TRUE(self_img != nullptr);
}
