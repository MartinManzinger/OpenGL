#pragma once

#include <iostream>
#include <math.h>
#define PI 3.1415926535897932384626433832795


struct pt2d
{
public:
	// variables
		float x, y;
	// constructors
		pt2d();
		pt2d(float in_x, float in_y);
	// operator overloading
		pt2d operator * (float Multiplier);
		pt2d operator / (float Divider);
		pt2d operator + (pt2d OtherPoint);
		pt2d operator += (pt2d OtherPoint);
		pt2d operator - (pt2d OtherPoint);
	// functions
		void clear();
		void set(float in_x, float in_y);
		void add(float add_x = 0, float add_y = 0);
		// returns the distance between itself and other point
		float distance(pt2d OtherPoint);
		// returns the normalized directional vector to other point
		pt2d orthonormal(pt2d OtherPoint);
};

class Particle
{
private:
	// private variables
		Particle** ListEntry;
	// private functions
		// -0.5 to 0.5
		float GenRandFloat();
		// -spread/2 to spread/2
		float GenRandFloat(float spread);
		// -max to max
		float GenRandFloat(float min, float max);
public:
	// variables
		int Id;
		pt2d pos;
		pt2d vel;
		pt2d acc;
		float mass;
		float radius;
		float temperature;
		Particle* CollisionNote; // helper flag to note occuring collision
	// constructor
		Particle(Particle** NewListEntry);
	// destructor
		~Particle();
	// functions
		// decide if explode or merge particles: returns number of resulting particles
		int Collide(Particle* NewParticles);
		void PrintInfo();
};

// dynamic list class to manage particles (with linked sublists)
struct pList
{
private:
	// settings
		const int SubListSize = 10;
	// private variables
		Particle** EntryPoint;
		Particle** ActSubList;
		int ActSubListIndex;
public:
	// variables
		int ParticleNum;
	// constructor
		pList();
	// functions
		Particle* GetParticle(bool Reset = 0);
		void AddEntry();
		void DefragList();
		void PrintInfo();
};

class ParticleEngine
{
private:
	// settings
	const int ParticleStartNum = 810;
	const float G = 0.003f;
	// private variables
	pList ParticleList;
public:
	// constructor: initialisation
		ParticleEngine();
	// functions
		Particle* GetParticle(bool Reset=0);
		void CalcNextTick();
		void ParticleEngine::ToGravCenter();
};
