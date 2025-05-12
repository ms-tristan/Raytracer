#ifndef SRC_EVENTSMANAGER_INPUTMANAGER_COMMANDS_ICOMMAND_HPP_
#define SRC_EVENTSMANAGER_INPUTMANAGER_COMMANDS_ICOMMAND_HPP_

#include <memory>
#include "../../../Camera/Camera.hpp"
#include "../../../Math/Vector3D/Vector3D.hpp"
#include "../../../Primitive/IPrimitive.hpp"

namespace RayTracer {

class ICommand {
 public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual bool isActive() const = 0;
};

}  // namespace RayTracer

#endif  // SRC_EVENTSMANAGER_INPUTMANAGER_COMMANDS_ICOMMAND_HPP_
