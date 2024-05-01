#pragma once
#include <raylib.h>
#include <math.h>
#include <vector>
#include <iostream>

namespace Helper
{
	namespace MergeSort
	{
		int* Merge(int* LeftArr, const int& LeftLength, int* RightArr, const int& RightLength);
		void CopyArr(const int* ReadArr, const int& ReadFrom,int* WriteArr, const int& Length);
		void MergeSort(int* OrigArr,const int& Length);
	}
	int GenerateHash(const int& x, const int& y);
}

struct vector2
{
protected:
	float x, y;
public:
	vector2(float X = 0, float Y = 0)
	{
		x = X;
		y = Y;
	}
	~vector2()
	{
		"dead vector";
	}
	void setX(const float& newX)
	{
		x = newX;
	}
	float getX()
	{
		return x;
	}
	void setY(const float& newY)
	{
		y = newY;
	}
	float getY()
	{
		return y;
	}
	void clear()
	{
		x = y = 0;
	}
	void operator=(const vector2& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
	}
	void operator+=(const vector2& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
	}
	void operator-=(const vector2& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
	}
	vector2 operator+(const vector2& rhs)
	{
		vector2 Final{ this->x + rhs.x, this->y + rhs.y };
		return Final;
	}
	vector2 operator-(const vector2& rhs)
	{
		vector2 Final{ this->x - rhs.x, this->y - rhs.y };
		return Final;
	}
	vector2 operator*(const float& rhs)
	{
		vector2 Final{ this->getX() * rhs, this->getY() * rhs };
		return Final;
	}
	void operator*=(const float& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
	}
	vector2 operator/(const float& rhs)
	{
		vector2 Final{ this->getX() / rhs, this->getY() / rhs };
		return Final;
	}
	void operator/=(const float& rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
	}
	float ToFloat()
	{
		return sqrtf(x * x+ y * y);
	}
};

/*
class particle
{
protected:
	int ID;
	float radius;
	vector2 position,  velocity,  acceleration;
public:
	particle(int& id)
	{
		//std::cout << "a particle is made" << std::endl;
		radius = 0.5f;
		ID = id ;
		position = vector2{ 1.0f,3.0f };
		velocity = vector2{ 20.0f, 10.0f };
		acceleration = vector2{ 0, 9.8f };
		//affectingForces.push_back(vector2{ 0, 9.81f * mass });
	} 
	void updatePhyisics(float& deltaTime)
	{
		velocity += acceleration * deltaTime;
		position += velocity * deltaTime;
	}
	void UpdatePositionVerlet(float& DT //Delta Time )
	{
		vector2 CurrentPosition, OldPosition, Accelaration;

		vector2* Velocity = new vector2{ CurrentPosition - OldPosition };
		OldPosition = CurrentPosition;

		CurrentPosition += *Velocity + Accelaration * DT * DT;
	}
	void SolveGroundCollision()
	{
		static float* FrictionConst = new float{ -1.15f }, *Scale = new float{ 20 * 2 };

		vector2* HalfScreenVect = new vector2{ GetScreenWidth() / *Scale,GetScreenHeight() / *Scale };
		vector2* TranslatedVect = new vector2{ this->position - *HalfScreenVect};



		if (fabsf(TranslatedVect->getX()) > fabsf(HalfScreenVect->getX() - this->radius))
		{
			float* SignOfPosition = new float{ TranslatedVect->getX() / fabsf(TranslatedVect->getX()) };
			this->position.setX(*SignOfPosition * fabsf(HalfScreenVect->getX() - this->radius) + HalfScreenVect->getX());

			//this->velocity.setX(this->velocity.getX() * *FrictionConst);
			this->velocity += vector2{ this->velocity.getX() * *FrictionConst, 0 };
		}



		if (fabsf(TranslatedVect->getY()) > fabsf(HalfScreenVect->getY() - this->radius))
		{
			float* SignOfPosition = new float{ TranslatedVect->getY() / fabsf(TranslatedVect->getY()) };
			this->position.setY(*SignOfPosition * fabsf(HalfScreenVect->getY() - this->radius) + HalfScreenVect->getY());

			//this->velocity.setY(this->velocity.getY() * *FrictionConst);
			this->velocity += vector2{ 0,  this->velocity.getY() * *FrictionConst};
		}

		//if (this->position.ToFloat() > HalfScreenVect->ToFloat() * 3)
		//{
		//	this->~particle();
		//}

		delete HalfScreenVect, TranslatedVect;

		//screenW, screenH; 
	}
	static void SolveCollision(particle& lhs, particle& rhs)
	 {
		vector2* CollisionVect = new vector2{ lhs.position - rhs.position };

		if (CollisionVect->ToFloat() < lhs.radius * 2.0f)
		{
			vector2* OverLap = new vector2 { (*CollisionVect / CollisionVect->ToFloat()) * (lhs.radius * 2.0f - CollisionVect->ToFloat()) / 2};

			lhs.position += *OverLap;
			rhs.position -= *OverLap;

			lhs.velocity += *OverLap * 25.0f;
			rhs.velocity -= *OverLap * 25.0f;

			delete OverLap;
		}
		delete CollisionVect;
	}
	float getX()
	{
		return position.getX();
	}
	float getY()
	{
		return position.getY();
	}
	float getRadius()
	{
		return this->radius;
	}
	int getID()
	{
		return this->ID;
	}
	~particle()
	{
		std::cout << "fuck you particle" << std::endl;
	}
	void Print()
	{
		std::cout << "   position: {" << position.getX() << ", " << position.getY() << " }" << "\n	ID : " << ID << std::endl;
	}
};
*/

class VerletParticle
{
protected:
	int ID;
	float radius;
	vector2 Position, OldPosition, Acceleration;
public:
	VerletParticle(int& id)
	{
		std::cout << "verlet particle " << id << " is made" << std::endl;
		radius = 1.0f;
		ID = id;
		OldPosition = vector2{1, 1};
		Position = OldPosition + vector2{ radius + 0.01f, 0.01f };
		Acceleration = vector2{ 0, 98.0f };
	}
	void UpdatePosition(float& DT /*Delta Time*/)
	{
		vector2* Velocity = new vector2{ Position - OldPosition };
		OldPosition = Position;

		Position += *Velocity + Acceleration * DT * DT;
		delete Velocity;
	}
	void SolveGroundCollision()
	{
		static float* Scale = new float{ 20 * 2 };

		//translate the cordinates from [0 - 100] to [-50 - 50] for example.

		vector2* HalfScreenVect = new vector2{ GetScreenWidth() / *Scale,GetScreenHeight() / *Scale };
		vector2* TranslatedVect = new vector2{ this->Position - *HalfScreenVect };

		//check for the side borders collision

		if (fabsf(TranslatedVect->getX()) > fabsf(HalfScreenVect->getX() - this->radius))
		{
			float* SignOfPosition = new float{ TranslatedVect->getX() / fabsf(TranslatedVect->getX()) };
			this->Position.setX(*SignOfPosition * fabsf(HalfScreenVect->getX() - this->radius) + HalfScreenVect->getX());
		}

		//check for top and bottom borders collision

		if (fabsf(TranslatedVect->getY()) > fabsf(HalfScreenVect->getY() - this->radius))
		{
			float* SignOfPosition = new float{ TranslatedVect->getY() / fabsf(TranslatedVect->getY()) };
			this->Position.setY(*SignOfPosition * fabsf(HalfScreenVect->getY() - this->radius) + HalfScreenVect->getY());
		}

		delete HalfScreenVect, TranslatedVect;
	}
	static void SolveCollision(VerletParticle& lhs, VerletParticle& rhs)
	{
		vector2* CollisionVect = new vector2{ lhs.Position - rhs.Position };

		if (CollisionVect->ToFloat() < lhs.radius * 2.0f)
		{
			vector2* OverLap = new vector2{ (*CollisionVect / CollisionVect->ToFloat()) * (lhs.radius * 2.0f - CollisionVect->ToFloat()) / 2 };

			lhs.Position += *OverLap;
			rhs.Position -= *OverLap;

			delete OverLap;
		}
		delete CollisionVect;
	}
	float getX()
	{
		return Position.getX();
	}
	float getY()
	{
		return Position.getY();
	}
	vector2 GetPosition()
	{
		return this->Position;
	}
	vector2 GetOldPosition()
	{
		return this->OldPosition;
	}
	float getRadius()
	{
		return this->radius;
	}
	int getID()
	{
		return this->ID;
	}
	~VerletParticle()
	{
		std::cout << "fuck you verlet particle" << std::endl;
	}
	void Print()
	{
		std::cout << "   position: {" << Position.getX() << ", " << Position.getY() << " }" << "\n	ID : " << ID << std::endl;
	}
};

class Grid
{
protected:

public:
	int GridWidth, GridHeight;

	Grid()
		:GridWidth{ GetScreenWidth() / 20 }, GridHeight{ GetScreenHeight() / 20} {}

	void SolveCollisionWithGrid(std::vector<VerletParticle*>& Group);
	void SolveCollisionWithSpacialArr(std::vector<VerletParticle*>& Group);
};
struct Cell
{
	int Count;
	int* IDArray;

	Cell()
		: IDArray{ nullptr }, Count{ 0 } {}


	int operator[](int Index)
	{
		return IDArray[Index];
	}

	void AddParticle(VerletParticle& Particle)
	{
		if (IDArray == nullptr)
		{
			Count = 1;
			IDArray = new int[Count]{ Particle.getID() };
		}
		else
		{
			int* TempArr = new int[Count];
			for (int i = 0; i < Count; i++)
			{
				TempArr[i] = IDArray[i];
			}

			int* IDArray = new int[Count + 1];
			for (int i = 0; i < Count; i++)
			{
				 IDArray[i] = TempArr[i];
			}
			delete[] TempArr;

			//then increment the Count and add the New ID.
			IDArray[Count++] = Particle.getID();
		}
	}
	~Cell()
	{
		delete[] IDArray;
	}
};


//class particleGroup
//{
//	std::vector<particle> vct;
//public:
//	particleGroup()
//	{
//
//	}
//	particle operator[] (int index)
//	{
//		return vct.at(index);
//	}
//};

