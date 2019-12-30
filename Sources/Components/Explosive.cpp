#include "Components/Explosive.hpp"
namespace Gg {

  namespace Component {
    Explosive::Explosive(): explosivePower{6},eTrigger{ON_COLLISION}
    {}
    Explosive::Explosive(int eP,explosiveTrigger eT):explosivePower{eP},eTrigger{eT}
    {}

    Explosive::Explosive(const Explosive &exp):explosivePower{exp.explosivePower},eTrigger{exp.eTrigger}
     {}
  }
}
