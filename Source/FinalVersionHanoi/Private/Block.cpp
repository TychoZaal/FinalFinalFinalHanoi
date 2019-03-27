#include "Block.h"
#include <Classes/Components/StaticMeshComponent.h>
#include "Engine/World.h"
#include <chrono>
#include <thread>
#include "TimerManager.h"
#include "Gameframework/Actor.h"

constexpr int amountOfDiscs = 3;
AActor* Discs[amountOfDiscs];

// Sets default values
ABlock::ABlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

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
					XScale = XScale / (counter * 0.8f);
					YScale = XScale;
				}
				else // First disc set scale higher
				{
					XScale = XScale * 1.5f;
					YScale = XScale;
				}

				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;

				FRotator rotator = FRotator(0, 0, 0);

				FVector spawnLocation = spawnLocation1;

				AActor* newBlock = world->SpawnActor<AActor>(ToSpawn, spawnLocation1 + FVector(0, 0, 0), rotator, spawnParams);;
				Discs[counter] = newBlock;
				newBlock->SetActorScale3D(FVector(XScale, YScale, 0.2f));
				counter++;

				if (counter == 1) // Downscale scale again for the rest of the discs
				{
					XScale = XScale / 1.5f;
					YScale = XScale;
				}
			}
		}
	}
	else
	{
		TowerOfHanoi(amountOfDiscs - 1, spawnLocation1, spawnLocation3, spawnLocation2);
	}

	GetWorldTimerManager().SetTimer(TimerHandler, this, &ABlock::SwitchBool, 1.0f, false);
}

void ABlock::TowerOfHanoi(int n, FVector from_rod, FVector to_rod, FVector aux_rod)
{
	canMove = false;

	if (n == 0)
	{
		MoveBlocks(Discs[n], to_rod);
		return;
	}

	TowerOfHanoi(n - 1, from_rod, aux_rod, to_rod);
	MoveBlocks(Discs[n], to_rod);
	TowerOfHanoi(n - 1, aux_rod, to_rod, from_rod);
	UE_LOG(LogTemp, Error, TEXT("Move function called"));
}

void ABlock::MoveBlocks(AActor * toMove, FVector newPosition)
{
	toMove->SetActorLocation(newPosition);
}

void ABlock::SwitchBool()
{
	canSpawn = true;
	GetWorldTimerManager().ClearTimer(TimerHandler);
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