#include "mesh-renderer.hpp"
#include "../asset-loader.hpp"

namespace our
{
    // Receives the mesh & material from the AssetLoader by the names given in the json object
    void MeshRendererComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        // Notice how we just get a string from the json file and pass it to the AssetLoader to get us the actual asset
        // TODO: (Req 8) Get the material and the mesh from the AssetLoader by their names
        // which are defined with the keys "mesh" and "material" in data.
        // Hint: To get a value of type T from a json object "data" where the key corresponding to the value is "key",
        // you can use write: data["key"].get<T>().
        // Look at "source/common/asset-loader.hpp" to know how to use the static class AssetLoader.
        if (data.contains("mesh") && data["mesh"].is_string())
        {
            std::string meshName = data["mesh"].get<std::string>();
            mesh = AssetLoader<Mesh>::get(meshName);
        }
        else
        {
            mesh = nullptr; // Default to nullptr if no valid mesh is provided
        }

        if (data.contains("material") && data["material"].is_string())
        {
            std::string materialName = data["material"].get<std::string>();
            material = AssetLoader<Material>::get(materialName);
        }
        else
        {
            material = nullptr; // Default to nullptr if no valid material is provided
        }
    }
}