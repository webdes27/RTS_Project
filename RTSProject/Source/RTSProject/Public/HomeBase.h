// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "../Public/Unit.h"
#include "HomeBase.generated.h"

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
	float timeBetweenSpawns = 5.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString teamName;


	TSubclassOf<AUnit>		unit;
	float					spawnTimer = 0.f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
