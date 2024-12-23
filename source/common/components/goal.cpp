#include "goal.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include <iostream>

namespace our {
    // Reads collider center & radius from the given json object
    void GoalComponent::deserialize(const nlohmann::json& data)
    {
        if (!data.is_object()) return;

        // Read the position of the goal from the JSON object
        position = data.value("position", position);
        
    }
}