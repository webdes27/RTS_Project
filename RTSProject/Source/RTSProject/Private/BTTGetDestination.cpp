// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTGetDestination.h"

#include "DrawDebugHelpers.h"
#include "../Public/Unit.h"
#include "../Public/UnitAIController.h"
#include "../Public/HomeBase.h"
#include "../Public/CoverPoint.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTGetDestination::ExecuteTask(UBehaviorTreeComponent &ownerComp, uint8* nodeMemory)
{

	AUnitAIController* unitController = Cast<AUnitAIController>(ownerComp.GetOwner());
	AUnit* unit = unitController->unit;
	if (unit->coverPoint)
	{
		unit->coverPoint->user = nullptr;
		unit->coverPoint = nullptr;
	}
	UBlackboardComponent* BB = ownerComp.GetBlackboardComponent();
	if (unit->homeBase)
	{
		BB->SetValueAsObject(unitController->destination, unit->homeBase->enemyBase);
	}

	return EBTNodeResult::Failed;
}

void UBTTGetDestination::OnGameplayTaskActivated(UGameplayTask& Task)
{

}
