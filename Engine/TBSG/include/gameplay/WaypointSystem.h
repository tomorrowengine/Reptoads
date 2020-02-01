#pragma once
#include "memory/Containers.h"
#include "memory/string.h"
#include "glm/glm.hpp"

namespace core
{
    class Transform;
}

namespace waypoint
{
    struct WaypointIndex
    {
        unsigned int x{ 0 };
        unsigned int y{ 0 };
    };

    
    struct WaypointData
    {
        ptl::vector<ptl::vector<core::Transform>> waypoints{};
        WaypointIndex currentWaypoint{ 0, 0 };
        float alpha{ 0 };
        float distanceToNextPoint{ 0 };
        bool isDone{ true };
        bool shouldStart{ true };
        bool movingUp{ true };
    };

    bool LerpPosition(WaypointData& waypointData, const float& deltaTime, glm::vec3& position, float speed);

    bool LerpRotation(WaypointData& waypointData, const float& deltaTime, glm::quat& rotation, float speed);

    bool LerpTransform(WaypointData& waypointData, const float& deltaTime, core::Transform& transform, float speed);

    bool LerpTransform(WaypointData& waypointData, const float& deltaTime, glm::vec3& position, glm::quat& rotation, float speed);

    bool TransformAlongSpline(WaypointData& waypointData, const float& deltaTime, core::Transform& transform, float speed);
    
    bool TransformAlongSpline(WaypointData& waypointData, const float& deltaTime, glm::vec3& position, glm::quat& rotation, float speed);

    bool MoveCharacterUp(WaypointData& waypointData, const float& deltaTime, core::Transform& transform, float speed, float height);

    bool MoveCharacterDown(WaypointData& waypointData, const float& deltaTime, core::Transform& transform, float speed, float height);

    bool MoveCharacter(WaypointData& characterWaypointData, const WaypointData& cameraWaypointData, const float& moveUpPoint, const float& moveDownPoint, const float& deltaTime, core::Transform& transform, float speed, float height);

    bool SnapToNextWaypoint(WaypointData& waypointData, glm::vec3& position, glm::quat& rotation);

    float GetWaypointDistance(WaypointData& waypointData);

    WaypointIndex GetWaypointIndexFromString(std::istringstream& ss, std::string& token);
}
