#include <hdbg/dbg/debuggee.hpp>

namespace hdbg {

Debuggee::Debuggee() = default;
Debuggee::Debuggee(Debuggee &&) = default;
Debuggee::~Debuggee() = default;

Debuggee & Debuggee::operator=(Debuggee &&) = default;

} // namespace hdbg
