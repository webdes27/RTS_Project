// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTFindCover.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UBTTFindCover : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	virtual void OnGameplayTaskActivated(UGameplayTask &Task) override;
};
