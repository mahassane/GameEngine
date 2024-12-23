#include "pit.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include <iostream>

namespace our {
    // Reads collider center & radius from the given json object
    void PitComponent::deserialize(const nlohmann::json& data)
    {
        if (!data.is_object()) return;

        // Read the position of the pit from the JSON object
        position = data.value("position", position);
        
        // Read the size of the pit from the JSON object
        size = data.value("size", size);
        
    }
}