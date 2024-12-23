#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    //ColliderComponent
    class KeyComponent: public Component {
    public:

        glm::vec3 position = { 0, 0, 0 };

        glm::vec3 getPos() const { return position; }
        // The ID of this component type is "Collider"
        static std::string getID() { return "Key"; }

        // Reads key position from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}