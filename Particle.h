#include <vector>
#pragma once

namespace PE
{
	struct Vector2
	{
	private:
		double x, y;
	public:
		Vector2(const double& inputX = 0.0, const double& inputY = 0.0);
		Vector2(const Vector2& otherVector);
		~Vector2();

		void SetX(const double& input);
		double GetX();

		void SetY(const double& input);
		double GetY();

		void operator=(const Vector2& secondVector);
		void operator+=(const Vector2& secondVector);
		void operator-=(const Vector2& secondVector);

		Vector2 operator+(const Vector2& secondVector);
		Vector2 operator-(const Vector2& secondVector);
		Vector2 operator*(const double& input);
		Vector2 operator/(const double& input);

		Vector2 GetNormal();
		double GetMagnitude();
	};
	struct DynamicIntArray
	{
	private:
		int* Array;
		int Size;
	public:
		DynamicIntArray();
		void Pushback(const int& Input);
		void Clear();
		int GetSize();
		int operator[](const int& Index);

		//an array that reassigns itself each time i pushback
	};

	namespace Constants
	{
		const int ScreenWidth{ 1600 }, ScreenHeight{ 900 }, ThreadCount{};
		const Vector2 GravityVector{ 0.0 , 0.5 };
		const float ParticleRadius{ 15.0 };
		const double Friction{ 0.98 };
	}

	class Particle
	{
	private:
		Vector2 CurrentPosition, OldPosition;
		int ID;
	public:
		Particle(const int& id, const Vector2& curPos, const  Vector2& oldPos);
		void UpdatePosition();
		int GetID();
		PE::Vector2 GetCurrentPos();
		void SetCurrentPos(const PE::Vector2& input);
		double GetCurrentX();
		double GetCurrentY();
		PE::Vector2 GetOldPos();
		void SetOldPos(const PE::Vector2& input);
		double GetOldX();
		double GetOldY();
	};
	class CollisionManager
	{
	private:
		int ChunkCountX, ChunkCountY;
		DynamicIntArray* ChunkList;
		std::vector<PE::Particle*>* ParticleList;

		int* IndexReader;
	public:
		CollisionManager(const int& gridSizeX, const int& gridSizeY, std::vector<PE::Particle*>*& particleListPtr);
		void UpdateChunks();
		void ExecuteGroundCollision();
		void ExecuteAllParticlesCollision();
		void ExecuteMultiThreadedCollision();
		void ExecuteChunkCollision(const int& chunkIndex);
		bool CheckCollision(const int& particle1, const int& particle2);
		void FixCollision(const int& particle1, const int& particle2);

		void PrintChunks();

	//
	// - round up all of the particle positions into the chunk size,
	// then store the particle using [] based on the round up position into the correct chunk in the chunk array
	//
	};
}

//
// The Particle Position Update System:
// 
//		Velocity = DeltaPosition / Time + (Accelaration * Time)
// 
//		NewPos = (Velcocity + (Gravity * time) ) * Time + CurrentPos
// 
//		NewPos = (CurrentPos - OldPos + (Gravity * time) ) * Time + CurrentPos
//
// NewPos and Time will be a temporary Vector, Old and Current will be part of the Particle
//