#include <gtest/gtest.h>

#include <hdbg/this_process.hpp>
#include <hdbg/binfmt/from_image.hpp>
#include <hdbg/dbg/local/debug_process.hpp>

class BinaryFormatImageTest
  : public ::testing::Test
{
public:
  BinaryFormatImageTest();
  virtual ~BinaryFormatImageTest();
  
  hdbg::LocalDebugProcess self_;
  const hdbg::BinaryFormat & self_img_;
};

BinaryFormatImageTest::BinaryFormatImageTest()
  : self_( hdbg::this_process::id(), hdbg::DebugProcess::MemRead )
  , self_img_( self_.image() ) {}

BinaryFormatImageTest::~BinaryFormatImageTest() = default;

TEST_F(BinaryFormatImageTest, EnumSections) {
  bool ep_found = false;
  const auto entry_pt = self_img_.entry_point();
  for(const auto & sect : self_img_.sections()) {
    if(sect.flags & hdbg::BinaryFormat::Section::Readable   &&
       sect.flags & hdbg::BinaryFormat::Section::Executable )
    {
      if(entry_pt > sect.vaddr && entry_pt <= (sect.vaddr + sect.vsize)) {
        ep_found = true;
        break;
      }
    }
  }
  ASSERT_TRUE(ep_found);
}

