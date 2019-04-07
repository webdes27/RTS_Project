// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTIsInCover.h"
#include "../Public/Unit.h"
#include "../Public/UnitAIController.h"
#include "../Public/CoverPoint.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTIsInCover::ExecuteTask(UBehaviorTreeComponent & ownerComp, uint8 * nodeMemory)
{
	AUnitAIController* unitController = Cast<AUnitAIController>(ownerComp.GetOwner());
	AUnit* unit = unitController->unit;

	UBlackboardComponent* BB = ownerComp.GetBlackboardComponent();
	if (BB)
	{
		BB->SetValueAsBool(unitController->isInCover, true);
		unit->isInCover = true;
	}
	return EBTNodeResult::Failed;

}

void UBTTIsInCover::OnGameplayTaskActivated(UGameplayTask & Task)
{
}
