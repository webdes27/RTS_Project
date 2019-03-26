// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/UnitAIController.h"
#include "../Public/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"


AUnitAIController::AUnitAIController()
{
	BBComp = CreateDefaultSubobject<UBlackboardComponent>("BBComp");
	BTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BTreeComp");
}

void AUnitAIController::SetTarget(APawn* targetPawn)
{
	BBComp->SetValueAsObject(target, targetPawn);
}

void AUnitAIController::Possess(APawn* pawn)
{
	Super::Possess(pawn);

	AUnit* unit = Cast<AUnit>(pawn);	

	if (unit)
	{
		if (unit->BTree->BlackboardAsset)
		{
			BBComp->InitializeBlackboard(*unit->BTree->BlackboardAsset);

			BTreeComp->StartTree(*unit->BTree);
		}

	}
}
