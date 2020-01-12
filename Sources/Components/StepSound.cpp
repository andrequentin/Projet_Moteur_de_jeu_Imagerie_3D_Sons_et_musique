#include "Components/StepSound.hpp"
namespace Gg {

  namespace Component {
    StepSound::StepSound()
    {}
    StepSound::StepSound(FMOD::Studio::EventInstance*s) :
      stepeventInstance{s}
    {}

  }
}
