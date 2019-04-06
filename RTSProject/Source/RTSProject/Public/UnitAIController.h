// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"

class AUnit;
class UBehaviorTreeComponent;
class UBlackboardComponent;

#define AI_PATIENCE 15 //TODO: Make this a blueprint editable variable

UCLASS()
class RTSPROJECT_API AUnitAIController : public AAIController
{
	GENERATED_BODY()

public:

	AUnitAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBlackboardComponent*			BBComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBehaviorTreeComponent*			BTreeComp;
	

	void SetTarget(APawn* targetPawn);
	virtual void Tick(float DeltaTime) override;

	bool IsTargetInSight(FVector Start, APawn* target);
	
	//BlackBoard Keys

	FName							target = "Target";	
	FName							angularDistance = "AngularDistance";
	FName							destination = "Destination";
	FName							underAttack = "UnderAttack";
	FName							isInCover = "IsInCover";
	FName							isDead = "IsDead";

	//////////////////////////////////////////////////////////////////

	AUnit*							unit = nullptr;
	AActor*							enemy = nullptr;

	int								seekAttempts = AI_PATIENCE;

	TSet<APawn*>					targets;

private:

	void OnPossess(APawn* pawn) override;
};
