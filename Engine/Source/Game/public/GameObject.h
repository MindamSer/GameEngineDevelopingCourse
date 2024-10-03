#pragma once

#include <RenderObject.h>
#include <RenderThread.h>
#include <Vector.h>

#include <cmath>

namespace GameEngine
{
	class GameObject
	{
	public:
		GameObject() = default;

		GameObject(Math::Vector3f position)
			:m_Position(position) {}

	public:
		Render::RenderObject** GetRenderObjectRef() { return &m_RenderObject; }

		void SetPosition(Math::Vector3f position, size_t frame)
		{
			m_Position = position;
		}

		Math::Vector3f GetPosition()
		{
			return m_Position;
		}

		void SetSpeed(Math::Vector3f speed)
		{
			m_Speed = speed;
		}

		Math::Vector3f GetSpeed()
		{
			return m_Speed;
		}

		virtual void Update(float dt, size_t frame)
		{
			m_Position = m_Position + m_Speed * dt;

			m_Speed = m_Speed * 0.999f;
			if (m_Speed.GetLength() < 0.001f)
			{
				m_Speed = Math::Vector3f::Zero();
			}

			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(m_Position, frame);
			}
		}

	protected:
		Render::RenderObject* m_RenderObject = nullptr;

		Math::Vector3f m_Position = Math::Vector3f::Zero();
		Math::Vector3f m_Speed = Math::Vector3f::Zero();
	};

	class SinusGameObject : public GameObject
	{
	public:
		SinusGameObject() = default;

		SinusGameObject(Math::Vector3f position)
			:GameObject(position) {}

	public:
		void Update(float dt, size_t frame) override
		{
			m_LiveTime += dt;

			m_Speed = Math::Vector3f(0.0f, 1.0f, 0.0f) * sin(m_LiveTime * m_Omega + m_Phase);

			m_Position = m_Position + m_Speed * dt;

			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(m_Position, frame);
			}
		}

	protected:
		float m_LiveTime = 0.0f;
		float m_Omega = (rand() % 100) / 10.;
		float m_Phase = (rand() % 100) / 10.;
	};

	class GravityGameObject : public GameObject
	{
	public:
		GravityGameObject() = default;

		GravityGameObject(Math::Vector3f position)
			:GameObject(position) {}

	public:
		void Update(float dt, size_t frame) override
		{
			m_Position = m_Position + m_Speed * dt;
			if (m_Position.y < m_Floor)
			{
				m_Speed.y *= -m_Bouncy;
				m_Position = m_Position + m_Speed * dt * 2.0f;
			}
			
			m_Speed = m_Speed * 0.999f + Math::Vector3f(0.0f, -20.0f, 0.0f) * dt;
			if (m_Speed.GetLength() < 0.01f)
			{
				m_Speed = Math::Vector3f::Zero();
			}
			
			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(m_Position, frame);
			}
		}

	protected:
		float m_Floor = -10.0f;
		float m_Bouncy = 0.9f + (rand() % 50 ) / 1000.;
	};

	class ControlGameObject : public GameObject
	{
	public:
		ControlGameObject() = default;

		ControlGameObject(Math::Vector3f position)
			:GameObject(position) {}
	};
}