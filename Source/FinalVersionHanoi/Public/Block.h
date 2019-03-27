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
		void Move(int count, FVector t1, FVector t2, FVector t3);

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

	UPROPERTY()
		int counter;

	UPROPERTY(EditAnywhere)
		bool canMove;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};