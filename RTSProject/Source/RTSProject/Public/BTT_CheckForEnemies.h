// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CheckForEnemies.generated.h"

/**
 * 
 */
UCLASS()
class RTSPROJECT_API UBTT_CheckForEnemies : public UBTTaskNode
{
	GENERATED_BODY()

	virtual void OnGameplayTaskActivated(UGameplayTask &Task) override;
};
