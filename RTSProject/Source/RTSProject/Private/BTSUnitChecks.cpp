// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSUnitChecks.h"
#include "../Public/Unit.h"
#include "../Public/UnitAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTSUnitChecks::TickNode(UBehaviorTreeComponent &tree, uint8 *nodeMemory, float deltaTime)
{
	AUnitAIController* unitController = Cast<AUnitAIController>(tree.GetOwner());
	AUnit* unit = unitController->unit;

	if (!unitController->IsTargetInSight(unit->headPoint->GetComponentLocation()))
	{
		tree.GetBlackboardComponent()->SetValueAsObject(unitController->target, nullptr);
		unitController->enemy = nullptr;
	}
}
