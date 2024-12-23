#include "key.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include <iostream>

namespace our {
    // Reads collider center & radius from the given json object
    void KeyComponent::deserialize(const nlohmann::json& data)
    {
        if (!data.is_object()) return;

        pos = data.value("pos", pos);
        
    }
}