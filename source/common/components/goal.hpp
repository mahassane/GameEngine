#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our 
{
    //GoalComponent
    class GoalComponent: public Component {
    public:

        glm::vec3 position = { 0.0f, 0.0f, 0.0f }; // Position of the goal 

        // The ID of this component type is "Goal"
        static std::string getID() { return "Goal"; }

        // Reads goal position from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}