#include <iostream>
#include <raylib.h>
#include <vector>
#include "Particle.h"

namespace PEFunctions
{
	void DrawParticles(std::vector<PE::Particle*>*& particleList)
	{
		for (int i = 0; i < particleList->size(); i++)
		{
			int
				* r = new int{ (*particleList)[i]->GetID() * 23 },
				* g = new int{ (*particleList)[i]->GetID() * 31 + 200},
				* b = new int{ (*particleList)[i]->GetID() * 57 + 100 },
				* a = new int{ -((*particleList)[i]->GetID() + *r + *b) };
			DrawCircle((*particleList)[i]->GetCurrentX(), (*particleList)[i]->GetCurrentY(), PE::Constants::ParticleRadius, Color{
				(unsigned char)*r,(unsigned char)*g,(unsigned char)*b, 200 });
				//255, 255, 255, 100});
			delete r, g, b, a;
		}
	}
	void SpawnParticle(std::vector<PE::Particle*>*& particleList)
	{
		static int CurrentID = 0;
		PE::Particle* NewParticle = new PE::Particle{ CurrentID, PE::Vector2{ PE::Constants::ParticleRadius ,50.0 }, PE::Vector2{PE::Constants::ParticleRadius * -2,50.0 } };
		particleList->push_back(NewParticle);
		std::cout << "Particle " << CurrentID << " is spawned\n";
		CurrentID++;
	}
	void UpdateParticlePositions(std::vector<PE::Particle*>*& particleList)
	{
		for (int i = 0; i < particleList->size(); i++)
		{
			(*particleList)[i]->UpdatePosition();
		}
	}
}

int main()
{	

	std::vector<PE::Particle*>* ParticleList = new std::vector<PE::Particle*>;
	PE::CollisionManager CollManager(
		(PE::Constants::ScreenWidth / PE::Constants::ParticleRadius ) ,
		(PE::Constants::ScreenHeight / PE::Constants::ParticleRadius ) ,
		//PE::Constants::ScreenWidth / 15,
		//PE::Constants::ScreenHeight / 15,
		ParticleList);

///////////////////////////////////////////////////////////////

	int* ScreenWidth = new int{ GetScreenWidth() };
	int* ScreenHight = new int{ GetScreenHeight() };

	InitWindow(PE::Constants::ScreenWidth, PE::Constants::ScreenHeight, "Physics Simulator");
	SetTargetFPS(30);
	//GetMonitorRefreshRate(GetCurrentMonitor()

	while (!WindowShouldClose())
	{	//		-updatePosition
//		-GroundCollision
//		-UpdateChunks
//		-ParticleParticleCollision
//		-SpawnNewParticle
// 
//		-Render
		if(GetFPS() > 10)
			PEFunctions::SpawnParticle(ParticleList);
		PEFunctions::UpdateParticlePositions(ParticleList);
		
		for (int i = 0; i < 2; i++)
		{
			CollManager.ExecuteGroundCollision();
			CollManager.UpdateChunks();
			CollManager.ExecuteAllParticlesCollision();
		}
		
		//CollManager.PrintChunks();

		//Redering.
		BeginDrawing(); ClearBackground(Color{ 10,10,20 }); 
		PEFunctions::DrawParticles(ParticleList);
		DrawFPS(10, 10); EndDrawing();
	}


	CloseWindow();
	return 0;
}

//the engine should run like this:
// 
// Idea #1
//
// - generate a Particle once per frame
// 
// - read the particles Array and sort them into chunks based on their positions 
//      the Chunks will store the IDs of the Particles inside them,
//      and each chunk will have its own special number based on its position ( X + screenwidth * Y ),
//      and have its placement order inside the dynamic array.
//      all of the chunks will be stored in a dynamic chunk array, 
//      when storing a particle 
//              check if there is a chunk with the position of the particle
//
//
// - execute collision checks on each particle inside the chunks
//
//////////////////////////////////////////
// 
// Idea #2
//
// - generate a Particle once per frame
// 
// - round up all of the particle positions into the chunk size,
// then store the particle using [] based on the round up position into the correct chunk in the chunk array
//
// - execute collision checks on each particle inside the chunks
//
// - render the Particles
//
////////////////////////////////////////////
// 
//	Loop()
//	{
//		-updatePosition
//		-GroundCollision
//		-CollisionGridUpdate
//		-ParticleParticleCollision
//		-SpawnNewParticle
//		-Render
//	}
//