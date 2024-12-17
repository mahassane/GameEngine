#pragma once

#include "../ecs/component.hpp"
#include <glm/glm.hpp>

namespace our {
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    struct Light {
        // Here we define our light. First member specifies its type.
        // glm::vec3 lightDirection;
        // glm::vec3 lightPosition;

        // Default
        LightType type = LightType::DIRECTIONAL;
        bool enabled = true; 

        // Color
        glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);

        // Direction
        glm::vec3 lightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
    
        struct {
            float constant = 1.0f;
            float linear = 0.09f;
            float quadratic = 0.032f;
        } attenuation; // Used for Point and Spot Lights only
        
        struct {
            float inner = 45.0f;
            float outer = 50.5f;
        } spot_angle; // Used for Spot Lights only
    };

    class LightComponent : public Component {
    public:
        Light light;
        static std::string getID() { return "Light"; }
        void deserialize(const nlohmann::json& data) override;
    };
}