// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API AUnitAIController : public AAIController
{
	GENERATED_BODY()

	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;
	
public:

	bool arrived = false;
	
};
