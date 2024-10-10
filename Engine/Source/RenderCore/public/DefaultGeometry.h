#pragma once

#include <array.h>
#include <Geometry.h>
#include <Vector.h>

namespace GameEngine
{
	namespace RenderCore
	{
		namespace DefaultGeometry
		{
			Geometry::Ptr Cube()
			{
				constexpr Core::array<Geometry::VertexType, 8> vertices =
				{
					Math::Vector3f(-1.0f, -1.0f, -1.0f),
					Math::Vector3f(-1.0f, +1.0f, -1.0f),
					Math::Vector3f(+1.0f, +1.0f, -1.0f),
					Math::Vector3f(+1.0f, -1.0f, -1.0f),
					Math::Vector3f(-1.0f, -1.0f, +1.0f),
					Math::Vector3f(-1.0f, +1.0f, +1.0f),
					Math::Vector3f(+1.0f, +1.0f, +1.0f),
					Math::Vector3f(+1.0f, -1.0f, +1.0f)
				};

				constexpr Core::array<Geometry::IndexType, 36> indices =
				{
					// front face
					0, 1, 2,
					0, 2, 3,

					// back face
					4, 6, 5,
					4, 7, 6,

					// left face
					4, 5, 1,
					4, 1, 0,

					// right face
					3, 2, 6,
					3, 6, 7,

					// top face
					1, 5, 6,
					1, 6, 2,

					// bottom face
					4, 0, 3,
					4, 3, 7
				};

				return std::make_shared<Geometry>((Geometry::VertexType*)vertices.begin(), vertices.size(), (Geometry::IndexType*)indices.begin(), indices.size());
			}

			Geometry::Ptr Ocrahedron()
			{
				constexpr Core::array<Geometry::VertexType, 6> vertices =
				{
					Math::Vector3f(+0.1f, +0.0f, +0.0f),
					Math::Vector3f(-0.1f, +0.0f, +0.0f),
					Math::Vector3f(+0.0f, +0.1f, +0.0f),
					Math::Vector3f(+0.0f, -0.1f, +0.0f),
					Math::Vector3f(+0.0f, +0.0f, +0.1f),
					Math::Vector3f(+0.0f, +0.0f, -0.1f)
				};

				constexpr Core::array<Geometry::IndexType, 24> indices =
				{
					0, 2, 4,
					4, 2, 1,
					1, 2, 5,
					5, 2, 0,
					0, 3, 5,
					5, 3, 1,
					1, 3, 4,
					4, 3, 0
				};

				return std::make_shared<Geometry>((Geometry::VertexType*)vertices.begin(), vertices.size(), (Geometry::IndexType*)indices.begin(), indices.size());
			}
		}
	}
}