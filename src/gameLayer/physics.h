#pragma once
#include <raylib.h>
#include <raymath.h>



// Vector2 operator overloads
inline Vector2 operator+(const Vector2 &a, const Vector2 &b)
{
	return { a.x + b.x, a.y + b.y };
}

inline Vector2 operator-(const Vector2 &a, const Vector2 &b)
{
	return { a.x - b.x, a.y - b.y };
}

inline Vector2 operator*(const Vector2 &a, float scalar)
{
	return { a.x * scalar, a.y * scalar };
}

inline Vector2 operator/(const Vector2 &a, float scalar)
{
	return { a.x / scalar, a.y / scalar };
}


inline Vector2 &operator*=(Vector2 &a, float scalar)
{
	a.x *= scalar;
	a.y *= scalar;
	return a;
}

inline Vector2 &operator/=(Vector2 &a, float scalar)
{
	a.x /= scalar;
	a.y /= scalar;
	return a;
}

inline Vector2 &operator+=(Vector2 &a, float scalar)
{
	a.x += scalar;
	a.y += scalar;
	return a;
}

inline Vector2 &operator-=(Vector2 &a, float scalar)
{
	a.x -= scalar;
	a.y -= scalar;
	return a;
}


inline bool operator==(const Vector2 &a, const Vector2 &b)
{
	return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Vector2 &a, const Vector2 &b)
{
	return !(a == b);
}

inline Vector2 &operator+=(Vector2 &a, const Vector2 &b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

inline Vector2 &operator-=(Vector2 &a, const Vector2 &b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

inline Vector2 &operator*=(Vector2 &a, const Vector2 &b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

inline Vector2 &operator/=(Vector2 &a, const Vector2 &b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}


struct Transform2D {
	Vector2 pos = {}; // center
	float w = 0; // width
	float h = 0; // height

	Vector2 getCenter() const { return { pos.x, pos.y }; }
	Vector2 getTop() const { return { pos.x, pos.y - h * 0.5f }; }
	Vector2 getBottom() const { return { pos.x, pos.y + h * 0.5f }; }
	Vector2 getLeft() const { return { pos.x - w * 0.5f, pos.y }; }
	Vector2 getRight() const { return { pos.x + w * 0.5f, pos.y }; }
	Vector2 getTopLeft() const { return { pos.x - w * 0.5f, pos.y - h * 0.5f }; }
	Vector2 getTopRight() const { return { pos.x + w * 0.5f, pos.y - h * 0.5f }; }
	Vector2 getBottomLeft() const { return { pos.x - w * 0.5f, pos.y + h * 0.5f }; }
	Vector2 getBottomRight() const { return { pos.x + w * 0.5f, pos.y - h * 0.5f }; }

	// also useful for rendering
	// Axis Align Bounding Box ( A box with no rotation. It is the collision ? box, starting from the top left corner)
	Rectangle getAABB() {
		return { pos.x - w * 0.5f, pos.y - h * 0.5f, w, h };
	}

	bool intersectPoint(Vector2 point, float delta = 0) { // by using the delta we can increase or decrease the size of the box
		Rectangle aabb = getAABB();
		aabb.x -= delta;
		aabb.y -= delta;
		aabb.width += 2 * delta;
		aabb.height += 2 * delta;

		return CheckCollisionPointRec(point, aabb);
	}

	bool intersectTransform(Transform2D other, float delta = 0) {
		Rectangle a = getAABB();
		Rectangle b = other.getAABB();

		a.x -= delta;
		a.y -= delta;
		a.width += 2 * delta;
		a.height += 2 * delta;

		b.x -= delta;
		b.y -= delta;
		b.width += 2 * delta;
		b.height += 2 * delta;

		return CheckCollisionRecs(a, b);
	}
};

struct PhysicalEntity {
	Transform2D transform;
	Vector2 lastPosition = {};

	// Velocity changes Position over time, and Acceleration changes Velocity over time
	Vector2 velocity = {}; // the current speed the object is moving. It is a vector because velocity has also a direction
	Vector2 acceleration = {}; // acceleration increases the velocity overtime instead of changing the position

	void teleport(Vector2 pos) {
		transform.pos = pos;
		lastPosition = pos;
	}

	void updateForces(float deltaTime) {
		// alway update the velocity first and then the position, because you may end up the object to gain energy (a ball jumping higher every bounch) instead of loosing
		velocity += acceleration * deltaTime;
		transform.pos += velocity * deltaTime;

		//velocity -= velocity * 0.1 * deltaTime; // something that will decrease velocity over time. Like when throwing a rock and gradualy falls because it faces a lot of air particles.
		
		// Universal drag (air resistance / friction). The faster the object moves, we exponentially have more drag
		Vector2 dragVector = Vector2{ velocity.x * std::abs(velocity.x), velocity.y * std::abs(velocity.y) };
		float drag = 0.01f; // tweak this for your needs

		// #include <raymath.h>
		if(Vector2Length(dragVector) * drag * deltaTime > Vector2Length(velocity)) {
			velocity = {};
		}
		else {
			velocity -= dragVector * drag * deltaTime;
		}

		if (Vector2Length(velocity) < 0.01) {
			velocity = {};
		}

		acceleration = {};
	}

	// called at the end of the frame (don't forget to call it for every entity)
	void updateFinal() {
		lastPosition = { transform.pos.x, transform.pos.y };
	}

	void applyGravity() {
		acceleration += {0, 20.0};
	}

	Vector2 &getPosition() {
		return transform.pos;
	}
};