// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"

class AUnit;
class UBehaviorTreeComponent;
class UBlackboardComponent;

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

	FName			target = "Target";	

private:

	virtual void Possess(APawn* pawn);
};
