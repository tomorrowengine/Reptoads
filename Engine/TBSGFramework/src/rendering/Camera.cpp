#include <rendering/Camera.h>

#include <glm/gtx/quaternion.hpp>

namespace gfx
{
	Camera::Camera()
		: m_ViewDirty(true)
		, m_InverseViewDirty(true)
		, m_ProjectionDirty(true)
		, m_InverseProjectionDirty(true)
		, m_vFoV(45.0f)
		, m_AspectRatio(1.0f)
		, m_zNear(0.1f)
		, m_zFar(100.0f)
	{
		
		
		
	}

	Camera::~Camera()
	{
		
	}

	void Camera::set_LookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
	{
		
		
		

		m_ViewMatrix = glm::lookAt(eye, target, up);
		m_Translation = eye;
		m_Rotation = glm::quat_cast(glm::transpose(m_ViewMatrix));

		m_InverseViewDirty = true;
		m_ViewDirty = false;
	}

	glm::mat4 Camera::get_ViewMatrix() const
	{
		if (m_ViewDirty) {
			UpdateViewMatrix();
		}
		return m_ViewMatrix;
	}

	glm::mat4 Camera::get_InverseViewMatrix() const
	{
		if (m_InverseViewDirty) {
			m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
			m_InverseViewDirty = false;
		}

		return m_InverseViewMatrix;
	}

	void Camera::set_Projection(float fovy, float aspect, float zNear, float zFar)
	{
		m_vFoV = fovy;
		m_AspectRatio = aspect;
		m_zNear = zNear;
		m_zFar = zFar;

		m_ProjectionDirty = true;
		m_InverseProjectionDirty = true;
	}

	glm::mat4 Camera::get_ProjectionMatrix() const
	{
		if (m_ProjectionDirty) {
			UpdateProjectionMatrix();
		}

		return m_ProjectionMatrix;
	}

	glm::mat4 Camera::get_InverseProjectionMatrix() const
	{
		if (m_InverseProjectionDirty) {
			UpdateInverseProjectionMatrix();
		}

		return m_InverseProjectionMatrix;
	}

	void Camera::set_FoV(float fovy)
	{
		if (m_vFoV != fovy) {
			m_vFoV = fovy;
			m_ProjectionDirty = true;
			m_InverseProjectionDirty = true;
		}
	}

	float Camera::get_FoV() const
	{
		return m_vFoV;
	}

	void Camera::set_Translation(glm::vec3 translation)
	{
		m_Translation = translation;
		m_ViewDirty = true;
	}

	glm::vec3 Camera::get_Translation() const
	{
		return m_Translation;
	}

	void Camera::set_Rotation(glm::quat rotation)
	{
		m_Rotation = rotation;
		m_ViewDirty = true;
	}

	glm::quat Camera::get_Rotation() const
	{
		return m_Rotation;
	}

	void Camera::Translate(glm::vec3 translation, Space space)
	{
		switch (space) {
		case Space::Local:
		{
			
			m_Translation += m_Rotation * translation;
		}
		break;
		case Space::World:
		{
			m_Translation += translation;
		}
		break;
		}

		

		m_ViewDirty = true;
		m_InverseViewDirty = true;
	}

	void Camera::Rotate(glm::quat quaternion)
	{
		m_Rotation = m_Rotation * quaternion;

		m_ViewDirty = true;
		m_InverseViewDirty = true;
	}

	void Camera::UpdateViewMatrix() const
	{
		glm::mat4 rotationMatrix = glm::transpose(glm::toMat4(m_Rotation));
		glm::mat4 translationMatrix = glm::translate(glm::mat4{1}, -m_Translation);

		
		m_ViewMatrix = rotationMatrix * translationMatrix;

		m_InverseViewDirty = true;
		m_ViewDirty = false;
	}

	void Camera::UpdateInverseViewMatrix() const
	{
		if (m_ViewDirty) {
			UpdateViewMatrix();
		}

		m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
		m_InverseViewDirty = false;
	}

	void Camera::UpdateProjectionMatrix() const
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_vFoV), m_AspectRatio, m_zNear, m_zFar); 

		m_ProjectionDirty = false;
		m_InverseProjectionDirty = true;
	}

	void Camera::UpdateInverseProjectionMatrix() const
	{
		if (m_ProjectionDirty) {
			UpdateProjectionMatrix();
		}

		m_InverseProjectionMatrix = glm::inverse(m_ProjectionMatrix);
		m_InverseProjectionDirty = false;
	}

}
