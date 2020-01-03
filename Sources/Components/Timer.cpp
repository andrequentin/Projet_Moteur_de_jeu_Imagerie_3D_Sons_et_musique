#include "Components/Timer.hpp"
namespace Gg {

  namespace Component {
    Timer::Timer():end{std::chrono::system_clock::now() + std::chrono::seconds(5)}
    {}
    Timer::Timer(long millisecs):end{std::chrono::system_clock::now() + std::chrono::milliseconds(millisecs)}
    {}

    Timer::Timer(const Timer &t):end{t.end}
     {}
  }
}
