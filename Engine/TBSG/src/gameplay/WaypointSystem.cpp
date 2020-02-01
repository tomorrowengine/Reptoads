#include "gameplay/WaypointSystem.h"

#include "gameplay/Transform.h"
#include "glm/gtx/compatibility.hpp"
#include "glm/gtx/string_cast.hpp"

#include "glm/ext.hpp"

#include "glm/gtx/spline.hpp"

#include "sstream"

namespace waypoint
{
    bool LerpPosition(WaypointData& waypointData, const float& deltaTime, glm::vec3& position, float speed)
    {
        if(waypointData.isDone)
        {
            return true;
        }

        const glm::vec3 startingPosition = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y].GetPos();
        glm::vec3 targetPosition;

        if (waypointData.currentWaypoint.y + 1 >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
        {
            targetPosition = waypointData.waypoints[waypointData.currentWaypoint.x + 1][0].GetPos();
        }
        else
        {
            targetPosition = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y + 1].GetPos();
        }

        waypointData.alpha += deltaTime * speed;

        if (glm::epsilonEqual(waypointData.alpha, 1.0f, 0.01f) || waypointData.alpha > 1.f)
        {
            waypointData.alpha = 0;
            position = targetPosition;

            if (waypointData.currentWaypoint.y >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
            {
                ++waypointData.currentWaypoint.x;
                waypointData.currentWaypoint.y = 0;
                waypointData.isDone = true;
                return true;
            }

            ++waypointData.currentWaypoint.y;

            return false;
        }
        position = glm::lerp(startingPosition, targetPosition, waypointData.alpha);

        return false;
    }

    
    bool LerpRotation(WaypointData& waypointData, const float& deltaTime, glm::quat& rotation, float speed)
    {
        if (waypointData.isDone)
        {
            return true;
        }

        const glm::quat startingRotation = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y].GetRot();
        glm::quat targetRotation;

        if (waypointData.currentWaypoint.y + 1 >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
        {
            targetRotation = waypointData.waypoints[waypointData.currentWaypoint.x + 1][0].GetRot();
        }
        else
        {
            targetRotation = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y + 1].GetRot();
        }

        waypointData.alpha += deltaTime * speed;

        if (glm::epsilonEqual(waypointData.alpha, 1.0f, 0.01f) || waypointData.alpha > 1.f)
        {
            waypointData.alpha = 0;
            rotation = targetRotation;

            if (waypointData.currentWaypoint.y >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
            {
                ++waypointData.currentWaypoint.x;
                waypointData.currentWaypoint.y = 0;
                waypointData.isDone = true;
                return true;
            }

            ++waypointData.currentWaypoint.y;

            return false;
        }
        rotation = glm::lerp(startingRotation, targetRotation, waypointData.alpha);

        return false;
    }

    
    bool LerpTransform(WaypointData& waypointData, const float& deltaTime, core::Transform& transform, float speed)
    {
        if (waypointData.isDone)
        {
            return true;
        }

        const core::Transform startingTransform = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y];
        core::Transform targetTransform;

        if (waypointData.currentWaypoint.y + 1 >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
        {
            if (waypointData.currentWaypoint.x + 1 >= waypointData.waypoints.size())
            {
                waypointData.isDone = true;
                return true;
            }
            targetTransform = waypointData.waypoints[waypointData.currentWaypoint.x + 1][0];
        }
        else
        {
            targetTransform = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y + 1];
        }

        const float distance = glm::length(targetTransform.GetPos() - startingTransform.GetPos());
        waypointData.alpha += (deltaTime * speed) / distance;

        if (glm::epsilonEqual(waypointData.alpha, 1.0f, 0.01f) || waypointData.alpha > 1.f)
        {
            waypointData.alpha = 0;
            transform.SetPos(targetTransform.GetPos());
            transform.SetRot(targetTransform.GetRot());

            ++waypointData.currentWaypoint.y;

            if (waypointData.currentWaypoint.y >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
            {
                ++waypointData.currentWaypoint.x;
                waypointData.currentWaypoint.y = 0;
                waypointData.isDone = true;
                return true;
            }

            return false;
        }

        transform.SetPos(glm::lerp(startingTransform.GetPos(), targetTransform.GetPos(), waypointData.alpha));
        transform.SetRot(glm::slerp(startingTransform.GetRot(), targetTransform.GetRot(), waypointData.alpha));

        auto delta = glm::inverse(targetTransform.GetRot()) * startingTransform.GetRot();
        

        return false;
    }

    bool LerpTransform(WaypointData& waypointData, const float& deltaTime, glm::vec3& position, glm::quat& rotation, float speed)
    {
        if (waypointData.isDone)
        {
            return true;
        }

        const core::Transform startingTransform = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y];
        core::Transform targetTransform;

        if (waypointData.currentWaypoint.y + 1 >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
        {
            if(waypointData.currentWaypoint.x + 1 >= waypointData.waypoints.size())
            {
                waypointData.isDone = true;
                return true;
            }
            targetTransform = waypointData.waypoints[waypointData.currentWaypoint.x + 1][0];
        }
        else
        {
            targetTransform = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y + 1];
        }

        const float distance = glm::length(targetTransform.GetPos() - startingTransform.GetPos());
        waypointData.alpha += (deltaTime * speed) / distance;

        if (glm::epsilonEqual(waypointData.alpha, 1.0f, 0.01f) || waypointData.alpha > 1.f)
        {
            waypointData.alpha = 0;
            position = targetTransform.GetPos();
            rotation = targetTransform.GetRot();

            ++waypointData.currentWaypoint.y;

            if (waypointData.currentWaypoint.y >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
            {
                ++waypointData.currentWaypoint.x;
                waypointData.currentWaypoint.y = 0;
                waypointData.isDone = true;
                return true;
            }

            return false;
        }

        position = glm::lerp(startingTransform.GetPos(), targetTransform.GetPos(), waypointData.alpha);
        rotation = glm::slerp(startingTransform.GetRot(), targetTransform.GetRot(), waypointData.alpha);

        return false;
    }

    bool TransformAlongSpline(WaypointData& waypointData, const float& deltaTime, core::Transform& transform,
        float speed)
    {
        if (waypointData.isDone)
        {
            return true;
        }

        core::Transform beforeTransform;
        const core::Transform startingTransform = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y];
        core::Transform targetTransform;
        core::Transform afterTransform;

        const auto beforeWaypointX = static_cast<int>(waypointData.currentWaypoint.x);
        const auto beforeWaypointY = static_cast<int>(waypointData.currentWaypoint.y);

        
        if(beforeWaypointY - 1 < 0)
        {
            if (beforeWaypointX - 1 < 0)
            {
                beforeTransform = startingTransform;
            }
            else
            {
                beforeTransform = waypointData.waypoints[beforeWaypointX - 1].back();
            }
        }
        else
        {
            beforeTransform = waypointData.waypoints[beforeWaypointX][beforeWaypointY - 1];
        }

        auto afterWaypointX = static_cast<int>(waypointData.currentWaypoint.x);
        auto afterWaypointY = static_cast<int>(waypointData.currentWaypoint.y);

        
        if (waypointData.currentWaypoint.y + 1 >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
        {
            afterWaypointY = 0;
            ++afterWaypointX;
            if (waypointData.currentWaypoint.x + 1 >= waypointData.waypoints.size())
            {

                waypointData.isDone = true;
                return true;
            }
            targetTransform = waypointData.waypoints[waypointData.currentWaypoint.x + 1][0];
        }
        else
        {
            ++afterWaypointY;
            targetTransform = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y + 1];
        }

        
        if (static_cast<unsigned int>(afterWaypointY + 1) >= waypointData.waypoints[afterWaypointX].size())
        {
            if (static_cast<unsigned int>(afterWaypointX + 1) >= waypointData.waypoints.size())
            {
                const glm::vec3 dir = glm::normalize(targetTransform.GetPos() - startingTransform.GetPos());

                afterTransform = waypointData.waypoints[afterWaypointX][afterWaypointY];

                afterTransform.SetPos(afterTransform.GetPos() + (dir * 100.f));
            }
            else
            {
                afterTransform = waypointData.waypoints[afterWaypointX + 1][0];
            }
        }
        else
        {
            afterTransform = waypointData.waypoints[afterWaypointX][afterWaypointY + 1];
        }

        const float distance = glm::length(targetTransform.GetPos() - startingTransform.GetPos());
        waypointData.alpha += (deltaTime * speed) / distance;

        
        const float epsilon = 0.01f;

        if (glm::epsilonEqual(waypointData.alpha, 1.0f, epsilon) || waypointData.alpha > 1.f)
        {
            waypointData.alpha = 0;
            transform.SetPos(targetTransform.GetPos());
            transform.SetRot(targetTransform.GetRot());

            ++waypointData.currentWaypoint.y;

            if (waypointData.currentWaypoint.y >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
            {
                ++waypointData.currentWaypoint.x;
                waypointData.currentWaypoint.y = 0;
                waypointData.isDone = true;
                return true;
            }

            return false;
        }

        transform.SetPos(glm::catmullRom(beforeTransform.GetPos(), startingTransform.GetPos(), targetTransform.GetPos(), afterTransform.GetPos(), waypointData.alpha));
        transform.SetRot(glm::slerp(startingTransform.GetRot(), targetTransform.GetRot(), waypointData.alpha));
        

        return false;
    }

    bool TransformAlongSpline(WaypointData& waypointData, const float& deltaTime, glm::vec3& position,
        glm::quat& rotation, float speed)
    {
        core::Transform transform{position, rotation, glm::vec3{1,1,1}};
        const bool result = TransformAlongSpline(waypointData, deltaTime, transform, speed);

        position = transform.GetPos();
        rotation = transform.GetRot();

        return result;
    }

    bool MoveCharacterUp(WaypointData& waypointData, const float& deltaTime, core::Transform& transform, float speed,
        float height)
    {
        if(waypointData.isDone)
        {
            return true;
        }

        const glm::vec3 startPosition{waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y].GetPos() };
        const glm::vec3 targetPosition{waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y].GetPos() + glm::vec3{0, height, 0}};

        waypointData.alpha += (speed * deltaTime) / height;

        const float epsilon = 0.1f;
        if (glm::epsilonEqual(waypointData.alpha, 1.0f, epsilon) || waypointData.alpha > 1.f)
        {
            if(waypointData.currentWaypoint.x + 1 < waypointData.waypoints.size())
            {
                ++waypointData.currentWaypoint.x;
            }
            transform.SetPos(targetPosition);
            waypointData.isDone = true;
            waypointData.alpha = 0;
            return true;
        }

        transform.SetPos(glm::lerp(startPosition, targetPosition, waypointData.alpha));
        return false;
    }

    bool MoveCharacterDown(WaypointData& waypointData, const float& deltaTime, core::Transform& transform, float speed,
        float height)
    {
        if (waypointData.isDone)
        {
            return true;
        }

        const glm::vec3 startPosition{ waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y].GetPos() + glm::vec3{0, height, 0} };
        const glm::vec3 targetPosition{ waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y].GetPos() };

        waypointData.alpha += (speed * deltaTime) / height;

        const float epsilon = 0.01f;
        if (glm::epsilonEqual(waypointData.alpha, 1.0f, epsilon) || waypointData.alpha > 1.f)
        {
            transform.SetPos(targetPosition);
            waypointData.isDone = true;
            return true;
        }

        transform.SetPos(glm::lerp(startPosition, targetPosition, waypointData.alpha));
        return false;
    }

    bool MoveCharacter(WaypointData& characterWaypoints, const WaypointData& cameraWaypoints,
        const float& moveUpPoint, const float& moveDownPoint, const float& deltaTime, core::Transform& transform,
        float speed, float height)
    {
        if (glm::epsilonEqual(cameraWaypoints.alpha, moveUpPoint - 0.1f, 0.05f) && !characterWaypoints.shouldStart && characterWaypoints.isDone)
        {
            characterWaypoints.shouldStart = true;
            characterWaypoints.movingUp = true;
        }
        if (cameraWaypoints.alpha >= moveUpPoint && cameraWaypoints.alpha < moveDownPoint && characterWaypoints.isDone && characterWaypoints.shouldStart)
        {
            characterWaypoints.shouldStart = false;
            characterWaypoints.isDone = false;
            characterWaypoints.alpha = 0.f;
        }
        if (glm::epsilonEqual(cameraWaypoints.alpha, moveDownPoint - 0.1f, 0.05f) && !characterWaypoints.shouldStart)
        {
            characterWaypoints.shouldStart = true;
            characterWaypoints.movingUp = false;
        }
        if ((cameraWaypoints.alpha >= moveDownPoint || cameraWaypoints.alpha == 0.f) && characterWaypoints.shouldStart &&
            !characterWaypoints.movingUp)
        {
            characterWaypoints.shouldStart = false;
            characterWaypoints.isDone = false;
            characterWaypoints.alpha = 0.f;

            transform.SetPos(glm::vec3{ characterWaypoints.waypoints[characterWaypoints.currentWaypoint.x][characterWaypoints.currentWaypoint.y].GetPos() + glm::vec3{ 0, height, 0 } });
        }

        if(!characterWaypoints.isDone && !characterWaypoints.shouldStart)
        {
            if (characterWaypoints.movingUp)
            {
                waypoint::MoveCharacterUp(characterWaypoints, deltaTime, transform, speed, height);
            }
            else
            {
                waypoint::MoveCharacterDown(characterWaypoints, deltaTime, transform, speed, height);
                if (characterWaypoints.isDone)
                {
                    return true;
                }
            }
        }
        

        return false;
    }

    bool SnapToNextWaypoint(WaypointData& waypointData, glm::vec3& position, glm::quat& rotation)
    {
        core::Transform targetTransform;

        if (waypointData.currentWaypoint.y + 1 >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
        {
            if (waypointData.currentWaypoint.x + 1 >= waypointData.waypoints.size())
            {
                return true;
            }
            targetTransform = waypointData.waypoints[waypointData.currentWaypoint.x + 1][0];
        }
        else
        {
            targetTransform = waypointData.waypoints[waypointData.currentWaypoint.x][waypointData.currentWaypoint.y + 1];
        }

        position = targetTransform.GetPos();
        rotation = targetTransform.GetRot();

        

        ++waypointData.currentWaypoint.y;

        if (waypointData.currentWaypoint.y >= waypointData.waypoints[waypointData.currentWaypoint.x].size())
        {
            ++waypointData.currentWaypoint.x;
            waypointData.currentWaypoint.y = 0;
            waypointData.alpha = 0;
            return true;
        }

        return false;
    }

    float GetWaypointDistance(WaypointData& waypointData)
    {
        float distance = 0.f;

        for (size_t i = 0; i < waypointData.waypoints[waypointData.currentWaypoint.x].size(); ++i)
        {
            if (i + 1 < waypointData.waypoints[waypointData.currentWaypoint.x].size())
            {
                distance += glm::length(waypointData.waypoints[waypointData.currentWaypoint.x][i + 1].GetPos() - waypointData.waypoints[waypointData.currentWaypoint.x][i].GetPos());
            }
            else if(waypointData.currentWaypoint.x + 1 < waypointData.waypoints.size())
            {
                distance += glm::length(waypointData.waypoints[waypointData.currentWaypoint.x + 1][0].GetPos() - waypointData.waypoints[waypointData.currentWaypoint.x][i].GetPos());
            }
        }

        return distance;
    }

    WaypointIndex GetWaypointIndexFromString(std::istringstream& ss, std::string& token)
    {
        std::getline(ss, token, '_');
        const unsigned int x = stoi(token);

        std::getline(ss, token, '_');
        const unsigned int y = stoi(token);

        return waypoint::WaypointIndex{x, y};
    }
}
