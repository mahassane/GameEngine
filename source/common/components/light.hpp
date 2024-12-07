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
        LightType type;
        // glm::vec3 lightDirection;
        // glm::vec3 lightPosition;
        bool enabled;
        glm::vec3 diffuse, specular, ambient;
    
        struct {
            float constant, linear, quadratic;
        } attenuation; // Used for Point and Spot Lights only
        
        struct {
            float inner, outer;
        } spot_angle; // Used for Spot Lights only
    };

    class LightComponent : public Component {
    public:
        Light light;
        static std::string getID() { return "Light"; }
        void deserialize(const nlohmann::json& data) override;
    };
}