#include "MePhysicsColliders.hpp"

#include <algorithm>

namespace me {
namespace algo {

const float EPSILON = 0.000;

CollisionPoints FindSphereSphereCollisionPoints(
	const SphereCollider* a, const TransformComponent& ta,
	const SphereCollider* b, const TransformComponent& tb)
{
	glm::vec3 A = a->center + ta.translation;
	glm::vec3 B = b->center + tb.translation;

	float Ar = a->radius * ta.scale.x;
	float Br = b->radius * tb.scale.x;

	glm::vec3 AtoB = B - A;

	if (glm::length(AtoB) > Ar + Br) {
		return { glm::vec3{0.f}, glm::vec3{0.f}, glm::vec3{0.f}, 0, false };
	}

	glm::vec3 BtoA = A - B;
	A += glm::normalize(AtoB) * Ar;
	B += glm::normalize(BtoA) * Br;

	AtoB = B - A;

	return { A, B, glm::normalize(AtoB), glm::length(AtoB), true };
}


CollisionPoints FindSpherePlaneCollisionPoints(
	const SphereCollider* a, const TransformComponent& ta,
	const PlaneCollider* b,  const TransformComponent& tb)
{
	glm::vec3 A = a->center + ta.translation;
	glm::vec3 N = b->plane;
	glm::normalize(N);

	glm::vec3 P = N * b->distance + tb.translation;

	float Ar = a->radius * ta.scale.x;
	float d = glm::dot((A - P), N);

	if (d > Ar) {
		return { glm::vec3{0.f}, glm::vec3{0.f}, glm::vec3{0.f}, 0, false };
	}

	glm::vec3 B = A - N * d;
	A = A - N * Ar;
	glm::vec3 AtoB = B - A;

	return { A, B, glm::normalize(N), glm::length(AtoB), true };
}


CollisionPoints FindSphereCubeCollisionPoints(
	const SphereCollider* a, const TransformComponent& ta,
	const CubeCollider* b, const TransformComponent& tb)
{
	glm::vec3 A = a->center + ta.translation;
	glm::vec3 B = b->center + tb.translation;
	glm::vec3 P;

	glm::vec3 dim = tb.scale * b->scale * 0.5f;
	glm::vec3 min = B - dim;
	glm::vec3 max = B + dim;

	/* X Axis */
	if (A.x < min.x) P.x = min.x;
	else if (A.x > max.x) P.x = max.x;
	else P.x = A.x;
	/* Y Axis */
	if (A.y < min.y) P.y = min.y;
	else if (A.y > max.y) P.y = max.y;
	else P.y = A.y;
	/* Z Axis */
	if (A.z < min.z) P.z = min.z;
	else if (A.z > max.z) P.z = max.z;
	else P.z = A.z;

	glm::vec3 AtoP = P - A;
	float Ar = a->radius * ta.scale.x;
	float d = glm::length(AtoP);
	if (d > Ar + EPSILON) {
		return { glm::vec3{0.f}, glm::vec3{0.f}, glm::vec3{0.f}, 0, false };
	}

	A += glm::normalize(AtoP) * Ar;

	return { A, P, glm::vec3{0.f}, 0, true };
}


}
}