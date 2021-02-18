#include "Camera.h"


namespace At0::VulkanTesting
{
	bool Camera::IsMoving() const
	{
		return Keys.Left || Keys.Right || Keys.Up || Keys.Down || Keys.Forward || Keys.Backward;
	}

	void Camera::SetPerspective(float fov, float aspect, float nearZ, float farZ)
	{
		m_FoV = fov;
		m_NearZ = nearZ;
		m_FarZ = farZ;
		Matrices.Perspective = glm::perspective(glm::radians(fov), aspect, nearZ, farZ);
		if (FlipY)
		{
			Matrices.Perspective[1][1] *= -1.0f;
		}
	}

	void Camera::UpdateAspectRatio(float aspect)
	{
		Matrices.Perspective = glm::perspective(glm::radians(m_FoV), aspect, m_NearZ, m_FarZ);
		if (FlipY)
		{
			Matrices.Perspective[1][1] *= -1.0f;
		}
	}

	void Camera::SetPosition(glm::vec3 pos)
	{
		Position = pos;
		UpdateViewMatrix();
	}

	void Camera::SetRotation(glm::vec3 rotation)
	{
		Rotation = rotation;
		UpdateViewMatrix();
	}

	void Camera::Rotate(glm::vec3 delta)
	{
		Rotation += delta;
		UpdateViewMatrix();
	}

	void Camera::Translate(glm::vec3 delta)
	{
		Position += delta;
		UpdateViewMatrix();
	}

	void Camera::Update(float dt)
	{
		Updated = false;

		if (Type == CameraType::FirstPerson)
		{
			if (IsMoving())
			{
				glm::vec3 camFront;
				camFront.x = -cos(glm::radians(Rotation.x)) * sin(glm::radians(Rotation.y));
				camFront.y = sin(glm::radians(Rotation.x));
				camFront.z = cos(glm::radians(Rotation.x)) * cos(glm::radians(Rotation.y));
				camFront = glm::normalize(camFront);

				float moveSpeed = dt * MovementSpeed;

				if (Keys.Forward)
					Position += camFront * moveSpeed;
				if (Keys.Backward)
					Position -= camFront * moveSpeed;
				if (Keys.Left)
					Position -= glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) *
								moveSpeed;
				if (Keys.Right)
					Position += glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) *
								moveSpeed;
				if (Keys.Up)
					Position.y += 1.0f * moveSpeed;
				if (Keys.Down)
					Position.y -= 1.0f * moveSpeed;

				UpdateViewMatrix();
			}

			// if (RotationBuffer != glm::vec2{ 0.0f, 0.0f })
			//{
			//	Rotate(glm::vec3(RotationBuffer * (/*dt * */ RotationSpeed), 0.0f));
			//	RotationBuffer = { 0.0f, 0.0f };

			//	changed = true;
			//}

			// if (changed)
		}
	}

	void Camera::UpdateViewMatrix()
	{
		glm::mat4 rotM = glm::mat4(1.0f);
		glm::mat4 transM;

		rotM = glm::rotate(
			rotM, glm::radians(Rotation.x * (FlipY ? -1.0f : 1.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 translation = Position;
		if (FlipY)
		{
			translation.y *= -1.0f;
		}
		transM = glm::translate(glm::mat4(1.0f), translation);

		if (Type == CameraType::FirstPerson)
		{
			Matrices.View = rotM * transM;
		}
		else
		{
			Matrices.View = transM * rotM;
		}

		ViewPos = glm::vec4(Position, 0.0f) * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);

		Updated = true;
	}
}  // namespace At0::VulkanTesting
