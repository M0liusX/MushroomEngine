#pragma once

#include "MeCommon.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace me {

struct CollisionPoints {
	glm::vec3 a;      // Furthest point of a into b
	glm::vec3 b;      // Furthest point of b into a
	glm::vec3 normal; // b Ea normalized
	float depth;      // Length of b Ea
	bool hasCollision;
};

/* Define Colliders */
struct SphereCollider;
struct PlaneCollider;
struct CubeCollider;
struct Collider {
	virtual CollisionPoints TestCollision(
		const TransformComponent& ta, const Collider* collider,
		const TransformComponent& tb) const = 0;

	virtual CollisionPoints TestCollision(
		const TransformComponent& ta, const SphereCollider* sphere,
		const TransformComponent& tb) const = 0;

	virtual CollisionPoints TestCollision(
			const TransformComponent & ta, const PlaneCollider * plane,
			const TransformComponent & tb) const = 0;

	virtual CollisionPoints TestCollision(
		const TransformComponent& ta, const CubeCollider* cube,
		const TransformComponent& tb) const = 0;

	virtual glm::mat4 GetMat4(TransformComponent& t) const = 0;
};

namespace algo {
	CollisionPoints FindSphereSphereCollisionPoints(
		const SphereCollider* a, const TransformComponent& ta,
		const SphereCollider* b, const TransformComponent& tb);

	CollisionPoints FindSphereCubeCollisionPoints(
		const SphereCollider* a, const TransformComponent& ta,
		const CubeCollider* b, const TransformComponent& tb);

	CollisionPoints FindSpherePlaneCollisionPoints(
		const SphereCollider* a, const TransformComponent& ta,
		const PlaneCollider* b,  const TransformComponent& tb);
}  // namespace algo

struct SphereCollider: Collider
{
	glm::vec3 center;
	float radius;

	CollisionPoints TestCollision(
		const TransformComponent& ta, const Collider* collider,
		const TransformComponent& tb) const override
	{
		return collider->TestCollision(tb, this, ta);
	}

	CollisionPoints TestCollision(
		const TransformComponent& ta, const SphereCollider* sphere,
		const TransformComponent& tb) const override
	{
		return algo::FindSphereSphereCollisionPoints(sphere, tb, this, ta);
	}

	CollisionPoints TestCollision(
		const TransformComponent& ta, const PlaneCollider* plane,
		const TransformComponent& tb) const override
	{
		return algo::FindSpherePlaneCollisionPoints(this, ta, plane, tb);
	}

	CollisionPoints TestCollision(
		const TransformComponent& ta, const CubeCollider* cube,
		const TransformComponent& tb) const override
	{
		return algo::FindSphereCubeCollisionPoints(this, ta, cube, tb);
	}

	glm::mat4 GetMat4(TransformComponent& t) const override
	{
		glm::mat4 transform = t.mat4();
		transform[0][0] *= radius;
		transform[1][1] *= radius;
		transform[2][2] *= radius;
		transform[3] += glm::vec4{ center.x, center.y, center.z, 0.0f };
		return transform;
	}

	static SphereCollider* create(glm::vec3 center, float radius)
	{
		SphereCollider* sc = new SphereCollider;
		sc->center = center;
		sc->radius = radius;
		return sc;
	}
};

struct PlaneCollider: Collider
{
	glm::vec3 plane;
	float distance;

	CollisionPoints TestCollision(
		const TransformComponent& ta, const Collider* collider,
		const TransformComponent& tb) const override
	{
		return collider->TestCollision(ta, this, tb);
	}

	CollisionPoints TestCollision(
		const TransformComponent& ta, const SphereCollider* sphere,
		const TransformComponent& tb) const override
	{
		return algo::FindSpherePlaneCollisionPoints(sphere, tb, this, ta);
	}

	CollisionPoints TestCollision(
		const TransformComponent& ta, const PlaneCollider* plane,
		const TransformComponent& tb) const override
	{
		return {}; // No plane v plane
	}

	CollisionPoints TestCollision(
		const TransformComponent& ta, const CubeCollider* cube,
		const TransformComponent& tb) const override
	{
		return {};  // TODO
	}

	glm::mat4 GetMat4(TransformComponent& t) const override
	{
		glm::mat4 transform = t.mat4();
		transform[3] += glm::vec4(plane * distance, 0.f);
		return transform;
	}

	static PlaneCollider* create(glm::vec3 plane, float distance)
	{
		PlaneCollider* pc = new PlaneCollider;
		pc->plane = plane;
		pc->distance = distance;
		return pc;
	}
};

struct CubeCollider : Collider
{
	glm::vec3 center;
	glm::vec3 scale;

	CollisionPoints TestCollision(
		const TransformComponent& ta, const Collider* collider,
		const TransformComponent& tb) const override
	{
		return collider->TestCollision(ta, this, tb);
	}

	CollisionPoints TestCollision(
		const TransformComponent& ta, const SphereCollider* sphere,
		const TransformComponent& tb) const override
	{
		return algo::FindSphereCubeCollisionPoints(sphere, tb, this, ta);
	}

	CollisionPoints TestCollision(
		const TransformComponent& ta, const PlaneCollider* plane,
		const TransformComponent& tb) const override
	{
		return {};  // TODO
	}

	CollisionPoints TestCollision(
		const TransformComponent& ta, const CubeCollider* cube,
		const TransformComponent& tb) const override
	{
		return {};  // TODO
	}

	glm::mat4 GetMat4(TransformComponent& t) const override
	{
		glm::mat4 transform = t.mat4();
		transform[0][0] *= scale.x;
		transform[1][1] *= scale.y;
		transform[2][2] *= scale.z;
		transform[3] += glm::vec4{ center.x, center.y, center.z, 0.0f };
		return transform;
	}

	static CubeCollider* create(glm::vec3 center, glm::vec3 scale)
	{
		CubeCollider* sc = new CubeCollider;
		sc->center = center;
		sc->scale = scale;
		return sc;
	}
};

}  // namespace me
