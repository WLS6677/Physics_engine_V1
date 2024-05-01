#include <raylib.h>
#include <iostream>
#include "particle.h"
#include <algorithm>

void drawAllParticles(std::vector<VerletParticle*>& Group)
{
	for (VerletParticle* Particle : Group)
	{
		DrawCircle(Particle->getX() * 20.0f, Particle->getY() * 20.0f, Particle->getRadius() * 20.0f,
			
			//the colour values.

			Color{
			/* Red */  (unsigned char)(Particle->getY() * 5.0f) ,
			/* Green */(unsigned char)(Particle->getID() * 2.0f + 100.0f),
			/* Blue */ (unsigned char)(Particle->getX() * 3.0f),
			/* Alpha */(unsigned char)(Particle->getY() + (Particle->GetPosition() - Particle->GetOldPosition()).ToFloat() * 255.0f)});
	}
}
void updateAllPhyisics(std::vector<VerletParticle*>& Group, float& Time, Grid& TheGrid)
{
	for (VerletParticle* Particle : Group)
	{
		Particle->UpdatePosition(Time);
		//Particle->Print();
	}

	for (VerletParticle* Particle : Group)
	{
		Particle->SolveGroundCollision();
	}

	//TheGrid.SolveCollisionWithGrid(Group);
	
	for (size_t i = 0; i <
		//Group.size() / 150 + 1;
		1;
		i++)
	{
		for (VerletParticle* Particle1 : Group)
		{
			for (VerletParticle* Particle2 : Group)
			{
				if (Particle1->getID() != Particle2->getID())
					VerletParticle::SolveCollision(*Particle1, *Particle2);
			}
		}
	}
	
}
void SpawnParticle(std::vector<VerletParticle*>& particles)
{
	static int ParticleCount = 1;
	if (
		//GetFrameTime() < 1.0/10)
		ParticleCount <= 500 )
	{
		VerletParticle* NewOne = new VerletParticle{ ParticleCount };
		ParticleCount++;
		particles.push_back(NewOne);
	//std::cout << ParticleCount << std::endl;
	}
}

int main()
{
	int* ScreenWidth = new int{ GetScreenWidth() }, * ScreenHight = new int{ GetScreenHeight() };

	InitWindow(1600, 900,
		//*ScreenWidth, *ScreenHight,
		"Physics Simulator");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
	//std::cout <<  << std::endl;

	std::vector<VerletParticle*> Particles;
	//int ParticleCount = 0;
	 
	Grid TheGrid;
	{
	int* hi1 = new int[5] { 1, 4, 5, 1, 2 };
	int* hi2 = new int[5] { 1, 2, 3, 4, 5 };

	int* hello = Helper::MergeSort::Merge(hi1, 5, hi2, 5);

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << hi1[i] << " ";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < 5; i++)
	{
		std::cout << hi2[i] << " ";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < 10; i++)
	{
		std::cout << hello[i] << " ";
	}
	std::cout << std::endl;
}
	
	while (!WindowShouldClose())
	{
		//std::cout << "drawing started" << std::endl;
		float DeltaTime = 0.01f;

		SpawnParticle(Particles);
		updateAllPhyisics(Particles, DeltaTime, TheGrid);
		//DrawTheGrid(TheGrid);

		//Redering.
		BeginDrawing();
		ClearBackground(Color{ 10,10,20 });

		drawAllParticles(Particles);
		DrawFPS(10, 10);

		EndDrawing();
		//std::cout << "drawing ended" << std::endl;
	}


	CloseWindow();
	return 0;
}