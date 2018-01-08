

#include "ParticleSystem.h"

#pragma region Struct_pt2d
	// constructors
		pt2d::pt2d()
		{
			x = 0;
			y = 0;
		}
		pt2d::pt2d(float in_x, float in_y)
		{
			x = in_x;
			y = in_y;
		}
	// operator overloading
		pt2d pt2d::operator * (float Multiplier)
		{
			pt2d ReturnVal(this->x * Multiplier, this->y * Multiplier);
			return ReturnVal;
		}
		pt2d pt2d::operator / (float Divider)
		{
			pt2d ReturnVal(this->x / Divider, this->y / Divider);
			return ReturnVal;
		}
		pt2d pt2d::operator + (pt2d OtherPoint)
		{
			pt2d ReturnVal(this->x + OtherPoint.x, this->y + OtherPoint.y);
			return ReturnVal;
		}
		pt2d pt2d::operator += (pt2d OtherPoint)
		{
			x += OtherPoint.x;
			y += OtherPoint.y;
			return *this;
		}
		pt2d pt2d::operator - (pt2d OtherPoint)
		{
			pt2d ReturnVal(this->x - OtherPoint.x, this->y - OtherPoint.y);
			return ReturnVal;
		}
	// functions
		void pt2d::clear()
		{
			x = 0;
			y = 0;
		}
		void pt2d::set(float in_x, float in_y)
		{
			x = in_x;
			y = in_y;
		}
		void pt2d::add(float add_x, float add_y)
		{
			x += add_x;
			y += add_y;
		}
		// returns the distance between itself and other point
		float pt2d::distance(pt2d OtherPoint)
		{
			float dx = OtherPoint.x - x;
			float dy = OtherPoint.y - y;
			return sqrt(dx*dx + dy*dy);
		}
		// returns the normalized directional vector to other point
		pt2d pt2d::orthonormal(pt2d OtherPoint)
		{
			float dx = x - OtherPoint.x;
			float dy = y - OtherPoint.y;
			pt2d Temp(dx, dy);
			return Temp / distance(OtherPoint);
		}
#pragma endregion

#pragma region Struct_pList
	// constructor
		pList::pList()
		{
			ParticleNum = 0;
			// generate first sublist for particles
			EntryPoint = new Particle*[SubListSize];
			memset(EntryPoint, NULL, SubListSize*sizeof(void*));
			ActSubList = EntryPoint;
			ActSubListIndex = 0;
		}
	// functions
		// search and returns next particle entry in list
		Particle* pList::GetParticle(bool Reset)
		{
			static Particle** SubListPtr = EntryPoint;
			static int SubListIndex = 0;
			Particle* ReturnParticle = nullptr;
			// search from beginning?
				if (Reset)
				{
					SubListPtr = EntryPoint;
					SubListIndex = 0;
					return nullptr;
				}
			// search next particle in list
				while (1)
				{
					// end of actual sublist?
					if (SubListIndex >= SubListSize - 1)
					{
						// next sublist
						if (*SubListPtr != NULL)
						{
							SubListIndex = 0;
							SubListPtr = (Particle**)*SubListPtr;
						}
						// end of complete list
						else
						{
							SubListPtr = EntryPoint;
							SubListIndex = 0;
							return nullptr;
						}
					}
					// particle contained in actual list entry?
					if (*SubListPtr != NULL)
					{
						++SubListIndex;
						++SubListPtr;
						return *(SubListPtr-1);
					}
					// increment
					++SubListIndex;
					++SubListPtr;
				}
		}
		void pList::AddEntry()
		{
			++ParticleNum;
			Particle** NewListEntryPtr = ActSubList + ActSubListIndex;
			*NewListEntryPtr = new Particle(NewListEntryPtr);
			++ActSubListIndex;
			// allocate new sublist?
			if (ActSubListIndex >= SubListSize - 1)
			{
				// last entry of sublist is link to next sublist
				Particle** NewSublistPtr = new Particle*[SubListSize];
				*(ActSubList + ActSubListIndex) = (Particle*)NewSublistPtr;
				ActSubList = NewSublistPtr;
				memset(ActSubList, NULL, SubListSize * sizeof(void*));
				ActSubListIndex = 0;
			}
		}
		void pList::DefragList()
		{

		}
		void pList::PrintInfo()
		{
			int SubListNum = 1;
			int ParticlesNum = 0;
			float Occupation = 0;

			int SubListIndex = 0;
			Particle** SubListPtr = EntryPoint;
			while (1)
			{
				// end of actual sublist?
				if (SubListIndex >= SubListSize-1)
				{
					// next sublist
					if (*SubListPtr != NULL)
					{
						++SubListNum;
						SubListIndex = 0;
						SubListPtr = (Particle**)*SubListPtr;
					}
					// end of complete list
					else
					{
						goto ListSearchComplete;
					}
				}
				// particle contained in actual list entry?
				if (*SubListPtr != NULL)
				{
					++ParticlesNum;
				}
				// increment
				++SubListIndex;
				++SubListPtr;
			}
			ListSearchComplete:
			Occupation = ((float)ParticlesNum * 100) / ((float)(SubListNum-1)*(SubListSize-1)+ActSubListIndex);
			printf("#### Particle List Info ####\n");
			printf("  Number of Sublists: %i\n", SubListNum);
			printf("  Number of Particles: %i\n", ParticlesNum);
			printf("  Percentage of Occupation: %.2f\n", Occupation);
		}
#pragma endregion

#pragma region Class_Particle
	// private functions
		// -0.5 to 0.5
		float Particle::GenRandFloat()
		{
			int RandInt = rand() % 1000;
			float RandFloat = ((float)RandInt) / 1000 - 0.5;
			return RandFloat;
		}
		// -spread/2 to spread/2
		float Particle::GenRandFloat(float spread)
		{
			return GenRandFloat()*spread;
		}
		// -max to max
		float Particle::GenRandFloat(float min, float max)
		{
			float spread = max - min;
			return GenRandFloat(spread) + spread / 2 + min;
		}
	// constructor
		Particle::Particle(Particle** NewListEntry)
		{
			// store pointer to entry in particle list
			ListEntry = NewListEntry;
			static int IdCount = 0;
			Id = IdCount;
			++IdCount;
			mass = GenRandFloat(0.00000000001, 0.0001);
			radius = sqrt(mass*0.018);
			pos.set(GenRandFloat(1.0), GenRandFloat(1.0));
			vel.set(GenRandFloat(0.01)+0.01*pos.y, GenRandFloat(0.01)-0.01*pos.x);
			temperature = GenRandFloat(0, 10);
		}
	// destructor
		Particle::~Particle()
		{
			// remove pointer to this particle in particle list
			*ListEntry = nullptr;
		}
	// functions
		// decide if explode or merge particles: returns number of resulting particles
		int Particle::Collide(Particle* NewParticles)
		{
			if (CollisionNote == nullptr) return 0;
			// merge particles
				// calculate propertys of resulting particle
				float d_vel = vel.distance(CollisionNote->vel);
				temperature = temperature*mass + CollisionNote->temperature*CollisionNote->mass;
				pt2d newimpuls = (vel*mass) +(CollisionNote->vel*CollisionNote->mass);
				mass += CollisionNote->mass;
				radius = sqrt(mass*0.018);
				temperature /= mass;
				temperature += d_vel*d_vel*1000000;
				vel = newimpuls / mass;
				// delete other particle
				CollisionNote->CollisionNote = nullptr;
				delete CollisionNote;
				CollisionNote = nullptr;
			return 1;
		}
		void Particle::PrintInfo()
		{
			printf("MassPoint (ID: %i)\n", Id);
			printf("  Position: %.2f, %.2f\n", pos.x, pos.y);
			printf("  Velocity: %.2f, %.2f\n", vel.x, vel.y);
			printf("  Mass: %.2f\n", mass);
		}
#pragma endregion

#pragma region Class_ParticleEngine
	// constructor: initialisation
	ParticleEngine::ParticleEngine()
	{
		// generate particles
		for (int i = 0; i < ParticleStartNum; i++)
		{
			ParticleList.AddEntry();
		}
		ParticleList.PrintInfo();
	}
	// functions
		Particle* ParticleEngine::GetParticle(bool Reset)
		{
			return ParticleList.GetParticle(Reset);
		}
		void ParticleEngine::CalcNextTick()
		{
			// delete old acceleration data
			ParticleList.GetParticle(true);
			Particle* ActParticle = ParticleList.GetParticle();
			while (ActParticle != nullptr)
			{
				ActParticle->acc.clear();
				ActParticle = ParticleList.GetParticle();
			}
			// calculate acceleration with collision detection
			// triangle matrix access of particles in list for efficiency
			ParticleList.GetParticle(true);
			ActParticle = ParticleList.GetParticle();  // particle 0
			ActParticle = ParticleList.GetParticle();  // particle 1
			ParticleList.GetParticle(true);
			int TriangleEnd = 1;
			int TriangleIndex = 0;
			while (1)
			{
				// next triangle line?
					if (TriangleEnd == TriangleIndex)
					{
						++TriangleEnd;
						TriangleIndex = 0;
						ParticleList.GetParticle();
						ActParticle = ParticleList.GetParticle();
						ParticleList.GetParticle(true);
					}
				// end of triangle matrix access?
					if (ActParticle == nullptr) break;
				// pick other particle from triangle line
					Particle* OtherParticle = ParticleList.GetParticle();
					++TriangleIndex;
				// acceleration calculation
					float distance = ActParticle->pos.distance(OtherParticle->pos);
					pt2d orthonorm = ActParticle->pos.orthonormal(OtherParticle->pos);
					float massAct = ActParticle->mass;
					float massOther = OtherParticle->mass;
					float factor = (G*massAct*massOther) / distance;
					ActParticle->acc += orthonorm * -factor;
					OtherParticle->acc += orthonorm * factor;
				// collision detection
					if (distance <= ActParticle->radius + OtherParticle->radius)
					{
						if (ActParticle->CollisionNote == nullptr && OtherParticle->CollisionNote == nullptr)
						{
							ActParticle->CollisionNote = OtherParticle;
							OtherParticle->CollisionNote = ActParticle;
						}
					}
			}
			// calculate new velocity and position with acceleration and collision handling and new temperature
				ParticleList.GetParticle(true);
				ActParticle = ParticleList.GetParticle();
				while (ActParticle != nullptr)
				{
					// calculate new temperature
						ActParticle->temperature *= 0.999;
					// calculate new velocity and position with acceleration
						if (ActParticle->CollisionNote == nullptr)
						{
							ActParticle->vel += ActParticle->acc / ActParticle->mass;
							ActParticle->pos += ActParticle->vel;
							if (ActParticle->mass >= 0.008)
							{
								ActParticle->PrintInfo();
								delete ActParticle;
							}
						}
					// collision handling
						else
						{
							Particle* Temp = nullptr;
							ActParticle->Collide(Temp);
							ParticleList.PrintInfo();
						}
					ActParticle = ParticleList.GetParticle();
				}

		}
		void ParticleEngine::ToGravCenter()
		{
			// find gravity center
				GetParticle(true);
				Particle* ActParticle = GetParticle();
				pt2d Center;
				float CenterMass = 0.0;
				while (ActParticle != nullptr)
				{
					float NewCenterMass = CenterMass + ActParticle->mass;
					Center = (ActParticle->pos*ActParticle->mass + Center*CenterMass) / NewCenterMass;
					CenterMass = NewCenterMass;
					ActParticle = GetParticle();
				}
			// transform coordinate system
				GetParticle(true);
				ActParticle = GetParticle();
				while (ActParticle != nullptr)
				{
					ActParticle->pos =  ActParticle->pos - Center;
					ActParticle = GetParticle();
				}
		}
		void ParticleEngine::AddParticle() {
			ParticleList.AddEntry();
		}
#pragma endregion
