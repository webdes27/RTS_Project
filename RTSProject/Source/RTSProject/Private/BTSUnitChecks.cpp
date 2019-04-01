// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSUnitChecks.h"
#include "../Public/Unit.h"
#include "../Public/UnitAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <algorithm>

void UBTSUnitChecks::TickNode(UBehaviorTreeComponent &tree, uint8 *nodeMemory, float deltaTime)
{
	AUnitAIController* unitController = Cast<AUnitAIController>(tree.GetOwner());
	AUnit* unit = unitController->unit;

	unitController->targets.Sort([unit](const APawn& a, const APawn& b) {
		return FVector::Dist(a.GetActorLocation(), unit->GetActorLocation())
			< FVector::Dist(b.GetActorLocation(), unit->GetActorLocation());
	});

	for (APawn* target : unitController->targets)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("%s -> %s"), *unit->GetName(), *target->GetName()));
		if (unitController->IsTargetInSight(unit->laserPoint->GetComponentLocation(), target))
		{
			tree.GetBlackboardComponent()->SetValueAsObject(unitController->target, target);
			unitController->enemy = target;
			return; //Return the closest enemy as target
		}
	}
	tree.GetBlackboardComponent()->SetValueAsObject(unitController->target, nullptr);
	unitController->enemy = nullptr;
}
