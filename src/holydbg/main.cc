#include <holydbg/arch_introspection.hpp>

int main()
{
  auto& x64_intr = holydbg::get_arch_introspection("x86_64");
}
