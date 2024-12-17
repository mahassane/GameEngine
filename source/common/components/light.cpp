#pragma once
#include <glm/glm.hpp>

#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    void LightComponent::deserialize(const nlohmann::json& data) {
        if(!data.is_object()) return;

        // Deserializing type
        std::string typeStr = data.value("type", "DIRECTIONAL");
        if (typeStr == "DIRECTIONAL") {
            light.type = LightType::DIRECTIONAL;
        } else if (typeStr == "POINT") {
            light.type = LightType::POINT;
        } else if (typeStr == "SPOT") {
            light.type = LightType::SPOT;
        }

        // Deserializing enabling
        light.enabled = data.value("enabled", true);

        // Deserializing color
        light.diffuse = glm::vec3(data.value("diffuse", glm::vec3(1.0f, 1.0f, 1.0f)));
        light.specular = glm::vec3(data.value("specular", glm::vec3(1.0f, 1.0f, 1.0f)));
        light.ambient = glm::vec3(data.value("ambient", glm::vec3(0.1f, 0.1f, 0.1f)));

        // Deserializing attenuation
        if (light.type == LightType::POINT || light.type == LightType::SPOT) {
            if (data.contains("attenuation")) {
                auto attenuation = data["attenuation"];
                light.attenuation.constant = attenuation.value("constant", 1.0f);
                light.attenuation.linear = attenuation.value("linear", 0.09f);
                light.attenuation.quadratic = attenuation.value("quadratic", 0.032f);
            }
        }

        // Deserializing direction
        if (light.type == LightType::DIRECTIONAL || light.type == LightType::SPOT) {
            if (data.contains("direction")) {
                auto dir = data["direction"];
                light.lightDirection = glm::vec3(dir[0], dir[1], dir[2]);
            } else {
                // Transform the default local direction using the entity's local-to-world matrix
                light.lightDirection = glm::vec3(this->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 1, 0));
            }
        }

        // Deserialize spot angles
        if (light.type == LightType::SPOT) {
            light.spot_angle.inner = data.value("spot_angle.inner", 0.85f);
            light.spot_angle.outer = data.value("spot_angle.outer", 0.9f);
            // Ensure inner angle < outer angle
            if (light.spot_angle.inner > light.spot_angle.outer) {
                std::swap(light.spot_angle.inner, light.spot_angle.outer);
            }
        }
    }
}