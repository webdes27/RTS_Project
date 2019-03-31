// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTSUnitChecks.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UBTSUnitChecks : public UBTService
{
	GENERATED_BODY()

		virtual void TickNode(UBehaviorTreeComponent &tree, uint8* nodeMemory, float deltaTime) override;
};
