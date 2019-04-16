#include "Block.h"
#include <Classes/Components/StaticMeshComponent.h>
#include "Engine/World.h"
#include "Engine/Engine.h"
#include <chrono>
#include <thread>
#include "TimerManager.h"
#include <iostream> 
#include <list> 
#include <vector>
#include "Gameframework/Actor.h"

constexpr int amountOfDiscs = 3;
AActor* Discs[amountOfDiscs];

// Struct containing movement data, which disc has to go where
struct MovementData
{
	FVector towerLocation;
	int number;
};

// Vector containing the order of which discs have to move where
std::vector <MovementData> moves;

// Sets default values
ABlock::ABlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Spawns all discs and sets their scale and position
void ABlock::Spawn()
{
	canSpawn = false;

	if (counter < amountOfDiscs)
	{
		if (ToSpawn)
		{
			if (world)
			{
				if (counter > 0)
				{
					XScale = XScale + (counter * 0.3f);
					YScale = XScale;
				}
				else // First disc decreased scale
				{
					XScale = XScale / 1.1f;
					YScale = XScale;
				}

				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;

				FRotator rotator = FRotator(0, 0, 0);

				FVector spawnLocation = spawnLocation1;

				AActor* newBlock = world->SpawnActor<AActor>(ToSpawn, spawnLocation1 - FVector(0, 0, counter * 10), rotator, spawnParams);;
				Discs[counter] = newBlock;
				newBlock->SetActorScale3D(FVector(XScale, YScale, 0.3f));
				counter++;

				if (counter == 1) // Downscale scale again for the rest of the discs
				{
					XScale = XScale * 1.3;
					YScale = XScale;
				}
			}
		}
		GetWorldTimerManager().SetTimer(TimerHandler, this, &ABlock::SwitchBool, 1.0f, false);
	}
	else // Start moving the discs
	{
		if (!stoppedSpawning)
			TowerOfHanoi(amountOfDiscs - 1, spawnLocation1, spawnLocation3, spawnLocation2);
	}
}

/* Recursive method that will only work if the minimum number of rods is 3. Follows the 132 231 pattern, where it moves a disc from 1 to 3 to 2.
   The one after that will go 231, where it moves from 2 to 3 to 1. The one after that again follows 132, etc. It always takes the smallest disc of a stack, because math.
   source: https://www.geeksforgeeks.org/c-program-for-tower-of-hanoi/
*/
void ABlock::TowerOfHanoi(int n, FVector from_rod, FVector to_rod, FVector aux_rod)
{
	stoppedSpawning = true;
	if (n == 0)
	{
		moves.push_back({ to_rod, n });
		it = 0;
		ABlock::MoveBlocks();
		return;
	}

	TowerOfHanoi(n - 1, from_rod, aux_rod, to_rod);
	moves.push_back({ to_rod, n });
	TowerOfHanoi(n - 1, aux_rod, to_rod, from_rod);
}

// Loops a timer which will activate the move method each time with a small delay
void ABlock::MoveBlocks()
{
	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandler, this, &ABlock::ChangeDiscLocation, 1.0f, true);
}

// Switches a bool, which will activate a timer occasionally, which will spawn a new disc
void ABlock::SwitchBool()
{
	canSpawn = true;
	GetWorldTimerManager().ClearTimer(TimerHandler);
}

// Changes a disc's position based on the list of structs with the pattern
void ABlock::ChangeDiscLocation()
{
	if (it < moves.size()) { 
		UE_LOG(LogTemp, Error, TEXT("Discnumber: %d  to: %s"), moves.at(it).number, *moves.at(it).towerLocation.ToString());
		Discs[moves.at(it).number]->SetActorLocation(FVector(moves.at(it).towerLocation.X, Discs[moves.at(it).number]->GetActorLocation().Y,  Discs[moves.at(it).number]->GetActorLocation().Z));
	}
	else {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandler); //clear timer
		return;
	}
	it++;
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	counter = 0;
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (canSpawn)
		Spawn();
}