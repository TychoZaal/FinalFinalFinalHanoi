#include "Block.h"
#include <Classes/Components/StaticMeshComponent.h>
#include "Engine/World.h"
#include <chrono>
#include <thread>
#include "TimerManager.h"
#include "Gameframework/Actor.h"

constexpr int amountOfDiscs = 5;
AActor* Discs[amountOfDiscs];

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

// Sets default values
ABlock::ABlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
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
					YScale = YScale / (counter * 0.8f);
				}

				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;

				FRotator rotator = FRotator(0, 0, 0);

				FVector spawnLocation = spawnLocation1;

				AActor* newBlock = world->SpawnActor<AActor>(ToSpawn, spawnLocation1 + FVector(0, 0, 0), rotator, spawnParams);;
				Discs[counter] = newBlock;
				newBlock->SetActorScale3D(FVector(XScale, YScale, 1.0f));
				counter++;
			}
		}
	}
	GetWorldTimerManager().SetTimer(TimerHandler, this, &ABlock::SwitchBool, 1.0f, false);

	if (counter == amountOfDiscs)
		canMove = true;
}

void ABlock::Move(int count, FVector t1, FVector t2, FVector t3)
{
	canMove = false;
	if (count > 0)
	{
		if (Discs[count])
		{
			//Move(count - 1, t1, t2, t3);
			MoveBlocks(Discs[count], t3);
			//Move(count - 1, t2, t3, t1);
		}
	}
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

	//if (canMove)
		//Move(amountOfDiscs, spawnLocation1, spawnLocation2, spawnLocation3);
}