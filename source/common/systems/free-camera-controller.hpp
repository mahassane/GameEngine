#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/collider.hpp"
#include "../components/goal.hpp"
#include "../components/key.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../components/collider.hpp"
#include <iostream>
using namespace std;

namespace our
{
    int jump_distance = 0;
    bool TreasureFound = false;
    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        bool update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
            {
                return false;
            }

            //cout << "Greating Goal" << endl;

            // Locate the goal component
            GoalComponent *goal = nullptr;
            for (auto entity : world->getEntities())
            {
                goal = entity->getComponent<GoalComponent>();
                //cout << "Goal Doesnt Exist" << endl;
                if (goal)
                {
                    //cout << "goal exists" << endl;
                    break;
                }
            }

            // If no goal is found, return
            if (!goal)
                return false;

            // Access the goal position
            glm::vec3 goalPosition = goal->position;

            // Debugging: Print the goal position
            //std::cout << "Goal Position: " << goalPosition.x << ", "  << goalPosition.y << ", " << goalPosition.z << std::endl;

            // Locate the key component
            KeyComponent *key = nullptr;
            for (auto entity : world->getEntities())
            {
                key = entity->getComponent<KeyComponent>();
                //cout << "Key Doesnt Exist" << endl;
                if (key)
                {
                    //cout << "Key exists" << endl;
                    break;
                }
            }

            // If no key is found, return
            if (!key)
                return false;

            // Access the key position
            glm::vec3 keyPosition = key->position;

            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = camera->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            {
                glm::vec2 delta = app->getMouse().getMouseDelta();
                rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f)
                rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f)
                rotation.x = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if (app->getKeyboard().isPressed(GLFW_KEY_W)){
                 // position += front * (deltaTime * current_sensitivity.z);
                glm::vec3 front_v = front * (deltaTime * current_sensitivity.x);
                glm::vec3 new_position_v = {front_v.x, 0, front_v.z};
                position += new_position_v;
                
                bool collide = detect_collision(world, position);
                if (collide)
                {
                    //Undo collision
                    position -= new_position_v;
                    std::cout << "COLLISION" << std::endl;
                }
            }
                
            if (app->getKeyboard().isPressed(GLFW_KEY_S)){
                // position -= front * (deltaTime * current_sensitivity.z);
                glm::vec3 front_v = -front * (deltaTime * current_sensitivity.z);
                glm::vec3 new_position_v = {front_v.x, 0, front_v.z};
                position += new_position_v;

                bool collide = detect_collision(world, position);
                if (collide)
                {
                    //Undo collision
                    position -= new_position_v;
                    std::cout << "COLLISION" << std::endl;
                }
            }

            
            // Q & E moves the player up and down
            // if (app->getKeyboard().isPressed(GLFW_KEY_Q))
            //     position += up * (deltaTime * current_sensitivity.y);
            // if (app->getKeyboard().isPressed(GLFW_KEY_E))
            //     position -= up * (deltaTime * current_sensitivity.y);

            // A & D moves the player left or right
            if (app->getKeyboard().isPressed(GLFW_KEY_D)){
                // position += right * (deltaTime * current_sensitivity.x);
                glm::vec3 new_position_v = right * (deltaTime * current_sensitivity.x);
                position += new_position_v;
                bool collide = detect_collision(world, position);

                if (collide)
                {
                    //Undo collision
                    position -= new_position_v;
                    std::cout << "COLLISION" << std::endl;
                }
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_A)){
                // position -= right * (deltaTime * current_sensitivity.x);
                glm::vec3 new_position_v = -right * (deltaTime * current_sensitivity.x);
                position += new_position_v;
                bool collide = detect_collision(world, position);

                if (collide)
                {
                    //Undo collision
                    position -= new_position_v;
                    std::cout << "COLLISION" << std::endl;
                }
            }

            // space means the player jumps up by a certain amount of units
            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE) && position.y <= 10)
            {
                // update position.y to add 30 units
                position.y += 1;
                bool collide = detect_collision(world, position);
                if (collide)
                {
                    position.y -= 1;
                }
                // update the jump_distance to 30 units
                jump_distance = 1;
            }

            // the player goes down if the space key is not pressed and stops when the player reaches the ground
            if (!app->getKeyboard().isPressed(GLFW_KEY_SPACE) && position.y > 0)
            {
                position.y -= 1;
            }

            
            if (glm::abs(position.x - glm::abs(keyPosition.x)) < 4 && glm::abs(position.z - glm::abs(keyPosition.z)) < 4)
            {
                TreasureFound = true;
                //cout << "Treasure: " << TreasureFound << endl;
            }

            return detect_exit(world, position, TreasureFound, goalPosition);
        }


        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

        bool detect_collision(World* world, glm::vec3 position)
        {
            //------------------------------------------BEGIN COLLISION----------------------

            bool collide = false;
            //Search for other entity having a collider component
            Entity* other = nullptr;
            ColliderComponent* collider = nullptr;
            for (auto entity : world->getEntities())
            {
                // Get the collider component if it exists
                collider = entity->getComponent<ColliderComponent>();
                // If the collider component exists
                if (collider)
                {
                    other = collider->getOwner();

                    //Get position of other entity
                    glm::vec3 otherPosition = other->localTransform.position;
                    glm::vec3 otherSize = collider->size;
                    //Detect collisions

                    bool collideX = false;
                    bool collideY = false;
                    bool collideZ = false;

                    float absX = glm::abs(position.x - otherPosition.x);
                    float absY = glm::abs(position.y - otherPosition.y);
                    float absZ = glm::abs(position.z - otherPosition.z);

                    if (absX < otherSize.x)
                    {
                        collideX = true;
                    }
                    if (absY < otherSize.y)
                    {
                        collideY = true;
                    }
                    if (absZ < otherSize.z)
                    {
                        collideZ = true;
                    }
                    collide = collideX && collideY && collideZ;

                    // std::cout << "My pos: " << position.x << " " << position.y << " " << position.z << std::endl;
                    // std::cout << "Other pos: " << otherPosition.x << " " << otherPosition.y << " " << otherPosition.z << std::endl;
                    // std::cout << "Diff: " << absX << " " << absY << " " << absZ << std::endl;
                    // std::cout << "Other size: " << otherSize.x << " " << otherSize.y << " " << otherSize.z << std::endl;
                    // float dist = glm::length(otherPosition - position);
                    // std::cout << dist << " " << collide << std::endl;
                    // std::cout << "----------------------------" << std::endl;

                    if (collide)
                    {
                        return collide;
                    }
                }

            }
            return false;
        }

        bool detect_exit(World* world, glm::vec3 position, bool found, glm::vec3 goal)
        {
            // std::cout << "Position: " << position.x << " " << position.y << " " << position.z << std::endl;
            
            //cout << "Found: " << found << endl;
            if (glm::abs(position.x - goal.x) < 6 && glm::abs(position.z - goal.z) < 6)
            {
                //cout << "Reached Glass" << endl;
                //cout << "Found: " << found << endl;
                if(found)
                {
                    //cout << "Game over: " << found;
                    return true;
                }
            }
            //cout << "Game not over: " << found;
            return false;
        }
    };

}
