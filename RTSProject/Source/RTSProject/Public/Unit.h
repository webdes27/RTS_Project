// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Unit.generated.h"

UCLASS()
class RTSPROJECT_API AUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnit();

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	AActor* target = nullptr;

	UFUNCTION(BlueprintCallable)
	void GetTarget();

	TArray<AActor*> FoundActors;

	UNavigationSystemBase* navSystem = nullptr;
	ANavigationData* navData = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
