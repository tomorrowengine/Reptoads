#pragma once
#pragma warning(push)
#pragma warning( disable : 4127)
#include <glm/mat4x2.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#pragma warning( pop ) 
#include <core/Assertion.h>

namespace scene {
	
	class SceneNode;
}

namespace core {
class Transform
{
	
	friend scene::SceneNode;

public:
	explicit Transform(Transform* parent = nullptr)
		: pos({}),
		  rot(glm::identity<glm::quat>()),
		  scale({1, 1, 1}),
		  parent(parent)
	{
	}
	explicit Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale, Transform* parent = nullptr)
		: pos(pos),
		  rot(rot),
		  scale(scale),
		  parent(parent)
	{
	}
	explicit Transform(const glm::mat4& matrix, Transform* parent = nullptr) 
		: parent(parent)
	{
		glm::vec3 scew;
		glm::vec4 perspective;
		bool ret = glm::decompose(matrix, scale, rot, pos, scew, perspective);
		ASSERT(ret);
	}
	explicit Transform(const glm::dmat4& matrix, Transform* parent = nullptr) 
		: parent(parent)
	{
		glm::dvec3 dscew;
		glm::dvec4 dperspective;

		glm::dvec3 dpos;
		glm::dquat drot;
		glm::dvec3 dscale;

		bool ret = glm::decompose(matrix, dscale, drot, dpos, dscew, dperspective);
		ASSERT(ret);

		pos = dpos;
		rot = drot;
		scale = dscale;
	}

    const glm::mat4& GetLocalModelMatrix() const
    {
		if (!localModelMatrixDirty)
			return localModelMatrix;

        localModelMatrix = glm::mat4{ 1 };


		localModelMatrix = glm::translate(localModelMatrix, pos);
		localModelMatrix = localModelMatrix * glm::toMat4(rot);
		localModelMatrix = glm::scale(localModelMatrix, scale);



        localModelMatrixDirty = false;

        return localModelMatrix;
    }

	glm::mat4 GetWorldModelMatrix() const
	{
		if(parent != nullptr) {
			return parent->GetWorldModelMatrix() * GetLocalModelMatrix();
		} else {
			return GetLocalModelMatrix();
		}
	}

    glm::vec3 GetPos() const { return pos; }
    void SetPos(glm::vec3 newPosition)
    {
        this->pos = newPosition;
        localModelMatrixDirty = true;
    }
    glm::quat GetRot() const { return rot; }
    void SetRot(glm::quat newRotation)
    {
        ASSERT(glm::epsilonEqual(glm::length(rot), 1.f, 0.1f));
        this->rot = newRotation;
        localModelMatrixDirty = true;
    }
    glm::vec3 GetScale() const { return scale; }
    void SetScale(glm::vec3 newScale)
    {
        this->scale = newScale;
        localModelMatrixDirty = true;
    }

	Transform* GetParent() { return parent; }
	const Transform* GetParent() const { return parent; }
private:
    glm::vec3 pos {};
    glm::quat rot {};
    glm::vec3 scale {};

	Transform* parent = nullptr;

    mutable bool localModelMatrixDirty = true;
    mutable glm::mat4 localModelMatrix = glm::mat4{1.0f};
};

    inline core::Transform MakeDefaultTransform()
    {
        return core::Transform{glm::vec3{0, 0, 0 }, glm::quat({0.f, 0.f, 0.f}), {1,1,1} };
    }
}
