/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies.
* Erin Catto makes no representations about the suitability 
* of this software for any purpose.  
* It is provided "as is" without express or implied warranty.
*/

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <unordered_set>
#include <map>
#include "MathUtils.h"
#include "Arbiter.h"

struct Body;
struct Joint;

struct World
{
	World(Vec2 gravity, int iterations) : gravity(gravity), iterations(iterations) {}

	// Body 관련
	void Add(Body* body);
	void Remove(Body* body);

	// Joint 관련
	void Add(Joint* joint);
	void Remove(Joint* joint);
	void Clear();

	void Step(float dt);

	void BroadPhase();

	bool IsValidBody(Body* body) { return validBodies.find(body) != validBodies.end(); }

	std::vector<Body*> bodies;
	std::unordered_set<Body*> validBodies;
	std::vector<Joint*> joints;
	std::unordered_set<Joint*> validjoints;
	std::map<ArbiterKey, Arbiter> arbiters;
	Vec2 gravity;
	int iterations;
	static bool accumulateImpulses;
	static bool warmStarting;
	static bool positionCorrection;
};

#endif
