#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UCLASS()
class FINALVERSIONHANOI_API ABlock : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlock();

	UPROPERTY(EditAnywhere)
		float XScale = 1;

	UPROPERTY(EditAnywhere)
		float YScale = 1;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> ToSpawn;

	UFUNCTION()
		void Spawn();

	UFUNCTION()
		void TowerOfHanoi(int n, FVector from_rod, FVector to_rod, FVector aux_rod);

	UFUNCTION()
		void MoveBlocks(AActor * toMove, FVector newPosition);

	UFUNCTION()
		void SwitchBool();

private:

	UPROPERTY(EditAnywhere)
		FVector spawnLocation1;

	UPROPERTY(EditAnywhere)
		FVector spawnLocation2;

	UPROPERTY(EditAnywhere)
		FVector spawnLocation3;

	UPROPERTY()
		FTimerHandle TimerHandler;

	UPROPERTY(EditAnywhere)
		bool canSpawn;

	UPROPERTY()
		UWorld * world;

	UPROPERTY(EditAnywhere)
		int counter;

	UPROPERTY(EditAnywhere)
		bool canMove;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};