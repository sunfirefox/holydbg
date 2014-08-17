#include <hdbg/dbg/debuggee.hpp>

namespace hdbg {

Debuggee::Debuggee() = default;
Debuggee::Debuggee(Debuggee &&) = default;
Debuggee::~Debuggee() = default;

Debuggee & Debuggee::operator=(Debuggee &&) = default;

void Debuggee::run()
{
  while(attached())
    run_until_next_event();
}

} // namespace hdbg
