#include "particle.h"

/*void Grid::AddParticle(particle* Particle)
{
	int PrtX = Particle->getX(), PrtY = Particle->getY();

	if (ParticleCountArray[PrtX , PrtY] != 0)
	{
		//update the number of particles in the cell.

		ParticleCountArray[ PrtX, PrtY] += 1;

		
		        | <--- screenWidth * 2 ---> |           __
			{ 0, null, 0, null, 0, null, 0, null        /\
			, 0, null, 0, null, 0, null, 0, null   screenHeight
		    , 0, null, 0, null, 0, null, 0, null        \/
			, 0, null, 0, null, 0, null, 0, null }      --

		//Updating the ID-Array array

		int* TempArr = new int[ParticleCountArray[PrtX, PrtY]];
		for (int i = 0; i < ParticleCountArray[PrtX, PrtY] - 1; i++)
		{
			TempArr[i] = *(ParticleIdArrayArray[PrtX, PrtY] + i);
			//std::cout << *(ParticleIdArrayArray[PrtX, PrtY] + i) << "\n";
		}
		TempArr[ParticleCountArray[PrtX, PrtY] - 1] = Particle->getID();
		//std::cout << Particle->getID() << "was added \n";

		delete ParticleIdArrayArray[PrtX, PrtY];
		ParticleIdArrayArray[PrtX, PrtY] = TempArr;
	}
	else 
	{
		ParticleCountArray[PrtX , PrtY] += 1;

		int* TempPrt = new int[1] {Particle->getID()};
		ParticleIdArrayArray[PrtX , PrtY] = TempPrt;

		//std::cout << "new cell \n" << ParticleCountArray[PrtX, PrtY] << " was added\n";
	}
} */

void Grid::SolveCollisionWithGrid(std::vector<VerletParticle*>& Group)
{
	Cell** TheIDGrid = new Cell*[GridWidth, GridHeight];

	for (int x = 0; x < GridWidth; x++)
	{
		for (int y = 0; y < GridHeight; y++)
		{
			TheIDGrid[x, y] = new Cell{};
		}
	}

	//Writing the IDs to the Array.
	for (auto& Particle : Group)
	{
		TheIDGrid[

			(int)Particle->getX(), //x
				(int)Particle->getY() * GridWidth //y

		]->AddParticle(*Particle);
	}

	//Reading from the array//
	for (int X = 1; X < GridWidth - 1; X++)
	{
		for (int Y = 1; Y < GridHeight - 1; Y++)
		{
			Cell* CurrentCell = TheIDGrid[X, Y];
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					Cell* NeighborCell = TheIDGrid[x, y];
					for (int P1 = 0; P1 < CurrentCell->Count; P1++)
					{
						for (int P2 = 0; P2 < NeighborCell->Count; P2++)
						{
							if ((*CurrentCell)[P1] != (*NeighborCell)[P2])
								VerletParticle::SolveCollision(*Group.at((*CurrentCell)[P1] - 1), *Group.at((*NeighborCell)[P2] - 1));
						}
					}
				}
			}
		}
	}
	delete[] TheIDGrid;
}

int* Helper::MergeSort::Merge(int* LeftArr, const int& LeftLength, int* RightArr, const int& RightLength)
{
	int* FinalArr = new int[LeftLength + RightLength];
	int* LeftArrCounter = new int{ 0 }, * RightArrCounter = new int{ 0 };

	for (size_t FinalArrCounter = 0; FinalArrCounter < LeftLength + RightLength; FinalArrCounter++)
	{
		//check if the right side has a bigger value
		if (LeftArr[*LeftArrCounter] >= RightArr[*RightArrCounter])
		{
			std::cout << "LeftSide chosen";
			//write the left side to the next element.
			FinalArr[FinalArrCounter] = LeftArr[*LeftArrCounter];
			(*LeftArrCounter)++;
			std::cout << " " << * LeftArrCounter << std::endl;
		}
		else
		{
			std::cout << "RightSide chosen";
			FinalArr[FinalArrCounter] = RightArr[*RightArrCounter];
			(*RightArrCounter)++;
			std::cout << " " << * RightArrCounter << std::endl;
		}
	}
	delete LeftArrCounter, RightArrCounter;
	 
	return FinalArr;

	/*
	int* FinalArr = new int[LeftLength + RightLength];
	int* RightCounter = new int{ 0 };
	int* FinalArrCounter = new int{ 0 };

	for (int LeftCounter = 0; LeftCounter < LeftLength; LeftCounter++)
	{
		//check if the right array has a bigger value than the left side and loop undil it no longer is bigger.
		while (RightArr[*RightCounter] > LeftArr[LeftCounter] && *RightCounter < RightLength)
		{
			FinalArr[*FinalArrCounter] = RightArr[*RightCounter];
			*RightCounter++;
			*FinalArrCounter++;
		}

		FinalArr[*FinalArrCounter] = LeftArr[LeftCounter];
		*FinalArrCounter++;
	}
	

	delete[] LeftArr, RightArr;
	delete RightCounter, FinalArrCounter;
	return FinalArr;
	*/
}
void Helper::MergeSort::CopyArr(const int* ReadArr,const int& ReadFrom,int* WriteArr, const int& Length)
{
	for (size_t i = 0; i < Length; i++)
	{
		WriteArr[i] = ReadArr[ReadFrom + i];
	}
}
void Helper::MergeSort::MergeSort(int* OrigArr,const int& Length)
{
	// check if there is only one element in the array.
	if (Length == 1) 
	{
		return;
	}

	//allocating memory for the two halves.
	int* LeftHalf = new int[Length / 2];
	int* RightHalf = new int[Length / 2 + Length % 2];

	//assigning the arrays with the values in the orignal array.
	Helper::MergeSort::CopyArr(OrigArr, 0, LeftHalf, Length / 2);
	Helper::MergeSort::CopyArr(OrigArr, Length / 2, RightHalf, Length / 2 + Length % 2);

	//sorting each half.
	Helper::MergeSort::MergeSort(LeftHalf, Length / 2);
	Helper::MergeSort::MergeSort(RightHalf, Length / 2 + Length % 2);

	//mergining the two halves.

}

int Helper::GenerateHash(const int& x, const int& y)
{
	//return abs((x * 2767) * ((y * 4591) * (x + 67) + (y * 6599) * (x * 3167) * (y + 89)));
	return abs(x * 7817 + y * 9737333);
}
void Grid::SolveCollisionWithSpacialArr(std::vector<VerletParticle*>& Group)
{
	//making an array parallel to the index list with each value having a special number depending on the position.
	int* KeyArr = new int[Group.size()];
	int* IndexArr = new int[Group.size()];

	//the Grid we are gonna be searching with.
	int* KeyGrid = new int[GridWidth, GridHeight];

	for (int i = 0; i < Group.size(); i++)
	{
		//generate a hash for the position vector
		KeyArr[i] = Helper::GenerateHash((int)Group.at(i)->getX(), (int)Group.at(i)->getY()) % (GridWidth * GridHeight);
	}



	for (int x = 0; x < GridWidth; x++)
	{
		for (int y = 0; y < GridHeight; y++)
		{
			KeyGrid[x, y] = Helper::GenerateHash(x, y) % (GridWidth * GridHeight);
		}
	}

	int* Counter1 = new int{ 0 }, * Counter2 = new int{ 0 };
	for (int x = 1; x < GridWidth - 1; x++)
	{
		for (int y = 1; y < GridHeight - 1; y++)
		{
			//check if the cell is empty.
			if (KeyArr[x, y] == -1)
				continue;
			else
			{
				//rotate on all particles in the current cell
				while (KeyArr[*Counter1] == KeyGrid[x, y])
				{
					for (size_t OffstX = -1; OffstX <= 1; OffstX++)
					{
						for (size_t OffstY = -1; OffstY <= 1; OffstY++) 
						{
							//check if the cell is empty
							if (KeyArr[OffstX, OffstY] == -1)
								continue;
							else
							{
								//rotate on all of the neighbouring cells's particles
								while (KeyArr[*Counter1] == KeyGrid[OffstX, OffstY])
								{
									VerletParticle::SolveCollision(*Group.at(IndexArr[*Counter1] - 1), *Group.at(IndexArr[*Counter2] - 1));
									*Counter2++;
								}
								*Counter2 = 0;
							}
						}
					}
					*Counter1++;
				}
				*Counter1 = 0;
			}
		}
	}
}