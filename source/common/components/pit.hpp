#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our 
{
    //PitComponent
    class PitComponent: public Component {
    public:

        glm::vec3 position = { 0.0f, 0.0f, 0.0f }; // Position of the pit
        glm::vec3 size = { 0.0f, 0.0f, 0.0f };     // Dimensions of the pit (length, width, depth)

        // The ID of this component type is "Pit"
        static std::string getID() { return "Pit"; }

        // Reads pit position & siza from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}