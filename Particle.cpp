#include <iostream>
#include <vector>
#include "Particle.h"
#include <thread>

//				      ||
//					  ||
//		not my code   \/
//
float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}
//
// everything else is
// 
/////////////////////////////////////////





//struct Vector2
PE::Vector2::Vector2(const double& inputX, const double& inputY)
{
	x = inputX;
	y = inputY;
}
PE::Vector2::Vector2(const Vector2& otherVector)
{
	this->x = otherVector.x;
	this->y = otherVector.y;
}
PE::Vector2::~Vector2()
{

}
void PE::Vector2::SetX(const double& input)
{
	this->x = input;
}
double PE::Vector2::GetX()
{
	return this->x;
}
void PE::Vector2::SetY(const double& input)
{
	this->y = input;
}
double PE::Vector2::GetY()
{
	return this->y;
}
void PE::Vector2::operator=(const PE::Vector2& secondVector)
{
	this->x = secondVector.x;
	this->y = secondVector.y;
}
void PE::Vector2::operator+=(const PE::Vector2& secondVector)
{
	this->x += secondVector.x;
	this->y += secondVector.y;
}
void PE::Vector2::operator-=(const PE::Vector2& secondVector)
{
	this->x -= secondVector.x;
	this->y -= secondVector.y;
}
PE::Vector2 PE::Vector2::operator+(const PE::Vector2& secondVector)
{
	PE::Vector2 Output{ this->x + secondVector.x,this->y + secondVector.y };
	return Output;
}
PE::Vector2 PE::Vector2::operator-(const PE::Vector2& secondVector)
{
	PE::Vector2 Output{ this->x - secondVector.x,this->y - secondVector.y };
	return Output;
}
PE::Vector2 PE::Vector2::operator*(const double& input)
{
	PE::Vector2 Output{ this->x * input,this->y * input };
	return Output;
}
PE::Vector2 PE::Vector2::operator/(const double& input)
{
	PE::Vector2 Output{ this->x / input,this->y / input };
	return Output;
}
PE::Vector2 PE::Vector2::GetNormal()
{
	return *this * (double)Q_rsqrt((this->x * this->x) + (this->y * this->y));
}
double PE::Vector2::GetMagnitude()
{
	return 1 / Q_rsqrt((this->x * this->x) + (this->y * this->y));
}





























//DynamicArray
PE::DynamicIntArray::DynamicIntArray()
{
	this->Array = nullptr;
	this->Size = 0;
}
void PE::DynamicIntArray::Pushback(const int& input)
{
	int* NewArray = new int[this->Size + 1];
	if (this->Array != nullptr)
	{
		for (int i = 0; i < this->Size; i++)
		{
			NewArray[i] = this->Array[i];
		}
	}
	NewArray[this->Size] = input;
	this->Size++;

	delete[] this->Array;
	this->Array = NewArray;
}
void PE::DynamicIntArray::Clear()
{
	if (this->Array != nullptr)
	{
		delete[] this->Array;
	}
	this->Array = nullptr;
	this->Size = 0;
}
int PE::DynamicIntArray::GetSize()
{
	return this->Size;
}
int PE::DynamicIntArray::operator[](const int& index)
{
	if (0 <= index && index < this->Size)
	{
		return this->Array[index];
	}
}



























//class Particle
PE::Particle::Particle(const int& id, const PE::Vector2& curPos, const  PE::Vector2& oldPos)
{
	this->ID = id;
	this->CurrentPosition = curPos;
	this->OldPosition = oldPos;
}
void PE::Particle::UpdatePosition()
{
	PE::Vector2* NewPostion = new PE::Vector2;

//		NewPos = CurrentPos + (CurrentPos - OldPos ) + GravityVector 

	*NewPostion = this->CurrentPosition * (1 + PE::Constants::Friction) - this->OldPosition * PE::Constants::Friction + PE::Constants::GravityVector;

	this->OldPosition = this->CurrentPosition;
	this->CurrentPosition = *NewPostion;

	delete NewPostion;
}
int PE::Particle::GetID()
{
	return this->ID;
}
PE::Vector2 PE::Particle::GetCurrentPos()
{
	return this->CurrentPosition;
}
void PE::Particle::SetCurrentPos(const PE::Vector2& input)
{
	this->CurrentPosition = input;
}
double PE::Particle::GetCurrentX()
{
	return this->CurrentPosition.GetX();
}
double PE::Particle::GetCurrentY()
{
	return this->CurrentPosition.GetY();
}
PE::Vector2 PE::Particle::GetOldPos()
{
	return this->OldPosition;
}
void PE::Particle::SetOldPos(const PE::Vector2& input)
{
	this->OldPosition = input;
}
double PE::Particle::GetOldX()
{
	return this->OldPosition.GetX();
}
double PE::Particle::GetOldY()
{
	return this->OldPosition.GetY();
}



























//class collision manager
PE::CollisionManager::CollisionManager(const int& gridSizeX, const int& gridSizeY, std::vector<PE::Particle*>*& particleListPtr)
{
	this->ChunkCountX = gridSizeX;
	this->ChunkCountY = gridSizeY;
	this->ChunkList = new DynamicIntArray[this->ChunkCountX * this->ChunkCountY];
	this->ParticleList = particleListPtr;

	IndexReader = new int[9]
	{
		-this->ChunkCountX -1,
		-this->ChunkCountX ,
		-this->ChunkCountX +1,
		-1,
		+1,
		this->ChunkCountX - 1,
		this->ChunkCountX,
		this->ChunkCountX + 1
	};
}
void PE::CollisionManager::UpdateChunks()
{
	for (int y = 0; y < this->ChunkCountY; y++)
	{
		for (int x = 0; x < this->ChunkCountX; x++)
		{
			this->ChunkList[x + (y * this->ChunkCountX)].Clear();
		}
	}

	//std::cout << "Updating Chunks...\n";

	for (int i = 0; i < this->ParticleList->size(); i++)
	{
		int ConvertedX = (int)(((*this->ParticleList)[i]->GetCurrentX() / PE::Constants::ScreenWidth) * this->ChunkCountX);
		int ConvertedY = (int)(((*this->ParticleList)[i]->GetCurrentY() / PE::Constants::ScreenHeight) * this->ChunkCountY);
		

		for (int OffsetY = -1; OffsetY <= 1; OffsetY++)
		{
			for (int OffsetX = -1; OffsetX <= 1; OffsetX++)
			{
				int FinalIndex = (ConvertedX + OffsetX) + ((ConvertedY + OffsetY) * this->ChunkCountX);
			if (0 <= FinalIndex && FinalIndex < this->ChunkCountX * this->ChunkCountY)
			{
					this->ChunkList[FinalIndex].Pushback((*this->ParticleList)[i]->GetID());
				}
			}
		}
		//std::cout << "\n we fucking stored in (" << ConvertedX << ", " << ConvertedY << ").\n";
	}

	/*for (int i = 0; i < this->ParticleList->size(); i++)
	{
		int ConvertedX = (int)(((*this->ParticleList)[i]->GetCurrentX() / PE::Constants::ScreenWidth) * this->ChunkCountX);
		int ConvertedY = (int)(((*this->ParticleList)[i]->GetCurrentY() / PE::Constants::ScreenHeight) * this->ChunkCountY);


				int FinalIndex = ConvertedX + (ConvertedY * this->ChunkCountX);
				if (0 <= FinalIndex && FinalIndex < this->ChunkCountX * this->ChunkCountY)
				{
					this->ChunkList[FinalIndex].Pushback((*this->ParticleList)[i]->GetID());
				}
	}
	*/
//
// step #1:
// 	   loop on each particle
// 
// step #2:
//		convert cords to grid index,
//		from ( 543.62, 792.5 )
//		to ( 5, 7 ) for example
// 
// step #3
//     then store its ID in the Vector<int>
// 	   with the converted Index
// 
//  WE FUCKING DID IT YAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAYYYYYYYYYYYY
// 
// step #4
//		the particle needs to be checked by the 8 neighbouring chunks
//		not just the chunk its inside
//
}
void PE::CollisionManager::ExecuteGroundCollision()
{
	for (int i = 0; i < this->ParticleList->size(); i++)
	{
		
		if ((*this->ParticleList)[i]->GetCurrentX() < 0 + PE::Constants::ParticleRadius)//Left
		{
			PE::Vector2* VelocityVec = new PE::Vector2{ (*this->ParticleList)[i]->GetCurrentPos() - (*this->ParticleList)[i]->GetOldPos() };

			(*this->ParticleList)[i]->SetCurrentPos(Vector2{	 0 + PE::Constants::ParticleRadius,														 (*this->ParticleList)[i]->GetCurrentY()	});
			(*this->ParticleList)[i]->SetOldPos(Vector2{		 0 + PE::Constants::ParticleRadius + (VelocityVec->GetX() * PE::Constants::Friction),	 (*this->ParticleList)[i]->GetOldY()		});
		}
		if ((*this->ParticleList)[i]->GetCurrentX() > PE::Constants::ScreenWidth - PE::Constants::ParticleRadius)//Right
		{
			PE::Vector2* VelocityVec = new PE::Vector2{ (*this->ParticleList)[i]->GetCurrentPos() - (*this->ParticleList)[i]->GetOldPos() };

			(*this->ParticleList)[i]->SetCurrentPos(Vector2{ PE::Constants::ScreenWidth - PE::Constants::ParticleRadius,											     	 (*this->ParticleList)[i]->GetCurrentY() });
			(*this->ParticleList)[i]->SetOldPos(Vector2{     PE::Constants::ScreenWidth - PE::Constants::ParticleRadius + (VelocityVec->GetX() * PE::Constants::Friction),	 (*this->ParticleList)[i]->GetOldY() });
		}
		if ((*this->ParticleList)[i]->GetCurrentY() < 0 + PE::Constants::ParticleRadius)//Top
		{
			PE::Vector2* VelocityVec = new PE::Vector2{ (*this->ParticleList)[i]->GetCurrentPos() - (*this->ParticleList)[i]->GetOldPos() };

			(*this->ParticleList)[i]->SetCurrentPos(Vector2{	(*this->ParticleList)[i]->GetCurrentX()	,			0 + PE::Constants::ParticleRadius					 });
			(*this->ParticleList)[i]->SetOldPos(Vector2{		(*this->ParticleList)[i]->GetOldX(),	  			0 + PE::Constants::ParticleRadius+ (VelocityVec->GetY() * PE::Constants::Friction)	 });
		}
		if ((*this->ParticleList)[i]->GetCurrentY() > PE::Constants::ScreenHeight - PE::Constants::ParticleRadius)//Bottom
		{
			PE::Vector2* VelocityVec = new PE::Vector2{ (*this->ParticleList)[i]->GetCurrentPos() - (*this->ParticleList)[i]->GetOldPos() };

			(*this->ParticleList)[i]->SetCurrentPos(Vector2{ (*this->ParticleList)[i]->GetCurrentX()	,			PE::Constants::ScreenHeight - PE::Constants::ParticleRadius });
			(*this->ParticleList)[i]->SetOldPos(Vector2{     (*this->ParticleList)[i]->GetOldX(),	  			    PE::Constants::ScreenHeight - PE::Constants::ParticleRadius + (VelocityVec->GetY() * PE::Constants::Friction) });
		}

		//				
		// how the Particle Should look like when detected:
		// 	
		//	  before:	o	|		after:	o	|
		//				  1 |				  o |3
		//				    |2				   4|.
		//					| o				  o |
		//
		// 1: OldPos
		// 2: CurrentPos
		// 
		// 3: Corrected OldPos			= Wall + velocityVector(-X) * friction 
		// 4: Corrected CurrentPos		= Wall
		//
	}
}
void PE::CollisionManager::ExecuteAllParticlesCollision()
{
	for (int ChunkY = 0; ChunkY < this->ChunkCountY; ChunkY++){
		for (int ChunkX = 0; ChunkX < this->ChunkCountX; ChunkX++)
		{
			//std::cout << "C:(" << ChunkX << ", " << ChunkY << "), ";

			int* CurrentChunkIndex = new int{ ChunkX + (ChunkY * this->ChunkCountX) };
			if (this->ChunkList[*CurrentChunkIndex].GetSize() == 0)
				continue;

			ExecuteChunkCollision(*CurrentChunkIndex);

			delete CurrentChunkIndex;
		}
		//std::cout << "\n";
	}
	//std::cout << "\n";
}
void PE::CollisionManager::ExecuteMultiThreadedCollision()
{
	//ExecuteChunkCollision();
//
// we need to prepare N number of Functions for the N threads to execute
//  
}
void PE::CollisionManager::ExecuteChunkCollision(const int& chunkIndex)
{
	for (int Particle1 = 0; Particle1 < this->ChunkList[chunkIndex].GetSize(); Particle1++)
	{
		
		for (int Particle2 = 0; Particle2 < this->ChunkList[chunkIndex].GetSize(); Particle2++)
		{
			
			if (CheckCollision(this->ChunkList[chunkIndex][Particle1], this->ChunkList[chunkIndex][Particle2]) )
			{
			//std::cout << "H:" << this->ChunkList[chunkIndex][Particle1] << " ";
			//std::cout << "E:" << this->ChunkList[chunkIndex][Particle2] << "\n";
				FixCollision(this->ChunkList[chunkIndex][Particle1], this->ChunkList[chunkIndex][Particle2]);
			}
		}
	}
	//
	// each particle inside the chunk will be compared to each other particle inside the chunk
	//
}
bool PE::CollisionManager::CheckCollision(const int& particle1,const int& particle2)
{
	if (particle1 == particle2)
		return false;

	PE::Vector2* DeltaDistance = new PE::Vector2{ (*this->ParticleList)[particle1]->GetCurrentPos() - (*this->ParticleList)[particle2]->GetCurrentPos() };

	//std::cout << particle1 << ", " << particle2 << ", (" << DeltaDistance->GetX() << ", " << DeltaDistance->GetY() << "), magnitute : " << DeltaDistance->GetMagnitude() << "\n   ";

	if (DeltaDistance->GetMagnitude() < 2 * PE::Constants::ParticleRadius)
	{
		delete DeltaDistance;
		return true;
	}
	delete DeltaDistance;
	return false;
}
void PE::CollisionManager::FixCollision(const int& particle1, const int& particle2)
{


	PE::Vector2* Distance = new PE::Vector2{ (*this->ParticleList)[particle1]->GetCurrentPos() - (*this->ParticleList)[particle2]->GetCurrentPos() };
	PE::Vector2* FixVector = new PE::Vector2{ (((Distance->GetNormal() * 2 * PE::Constants::ParticleRadius) - *Distance) * PE::Constants::Friction) / 2 };

	(*this->ParticleList)[particle2]->SetCurrentPos((*this->ParticleList)[particle2]->GetCurrentPos() - *FixVector);
	(*this->ParticleList)[particle1]->SetCurrentPos((*this->ParticleList)[particle1]->GetCurrentPos() + *FixVector);

	delete Distance, FixVector;
	//
	//math:
	// final Pos = currentPos +- (2*radius*DistanceNormalVec - DistanceVec)
	//
}
void PE::CollisionManager::PrintChunks()
{
	std::cout << "--------------- chunk --------------\n{ " << std::endl;
	for (int y = 0; y < this->ChunkCountY; y++)
	{
		for (int x = 0; x < this->ChunkCountX; x++)
		{
			std::cout << " (";
			for (int i = 0; i < this->ChunkList[x + (this->ChunkCountX * y)].GetSize(); i++)
			{
				std::cout << this->ChunkList[x + (this->ChunkCountX * y)][i] << ",";
			}
			std::cout << "),";
		}
		std::cout << "\n";
	}
	std::cout << "";
}

//
// #1 we are going to convert the [x,Y] indexs to a single index number
// #2 devide all of them equaly into the Number of threads we are going to use.
// 
// #3 then execute the portion of the chunks thats relevant for each thread
// #4 then convert them back using / for y and % for x on the Grids width.
//  
//