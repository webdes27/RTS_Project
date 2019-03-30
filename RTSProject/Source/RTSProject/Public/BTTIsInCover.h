// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTIsInCover.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UBTTIsInCover : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	virtual void OnGameplayTaskActivated(UGameplayTask &Task) override;
};
