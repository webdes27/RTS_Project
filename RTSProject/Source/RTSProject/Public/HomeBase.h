// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "../Public/Unit.h"
#include <queue>

#include "HomeBase.generated.h"

#define CHECKS_PER_FRAME 5

UCLASS()
class RTSPROJECT_API AHomeBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AHomeBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* baseMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float timeBetweenSpawns = 10.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString teamName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextRenderComponent* stateText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* spawnPoint;

	TSubclassOf<AUnit>		unit;
	float					spawnTimer = 0.f;
	int						team;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int						life = 5000;
	std::queue<AUnit*>		unitsActive;
		

};
