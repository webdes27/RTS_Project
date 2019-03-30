// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTFindCover.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "CoverPoint.h"
#include "../Public/Unit.h"
#include "../Public/UnitAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTFindCover::ExecuteTask(UBehaviorTreeComponent & ownerComp, uint8 * nodeMemory)
{
	AUnitAIController* unitController = Cast<AUnitAIController>(ownerComp.GetOwner());
	AUnit* unit = unitController->unit;
	TArray<AActor*> covers;	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoverPoint::StaticClass(), covers);
	ACoverPoint* bestCover = nullptr;
	for (AActor* cover : covers)
	{
		ACoverPoint* cp = Cast<ACoverPoint>(cover);
		if (bestCover == nullptr && cp->user == nullptr)
		{
			bestCover = cp;
		}
		else
		{
			if (bestCover == nullptr)
			{
				bestCover = cp;
			}
			else
			{
				if (unit->unitTeam == 0)
				{
					if (cp->user == nullptr)
					{
						if (bestCover->blueOnSight > cp->blueOnSight)
						{
							bestCover = cp;
						}
						else if (bestCover->blueOnSight == cp->blueOnSight
							&& FVector::Dist(bestCover->GetActorLocation(), unit->GetActorLocation()) > FVector::Dist(cp->GetActorLocation(), unit->GetActorLocation()))
						{
							bestCover = cp;
						}
					}
				}
				else
				{
					if (cp->user == nullptr)
					{
						if (bestCover->redOnSight > cp->redOnSight)
						{
							bestCover = cp;
						}
						else if (bestCover->redOnSight == cp->redOnSight
							&& FVector::Dist(bestCover->GetActorLocation(), unit->GetActorLocation()) > FVector::Dist(cp->GetActorLocation(), unit->GetActorLocation()))
						{
							bestCover = cp;
						}
					}
				}
			}
		}
	}
	UBlackboardComponent* BB = ownerComp.GetBlackboardComponent();
	if (bestCover)
	{
		BB->SetValueAsObject(unitController->destination, bestCover);
		BB->SetValueAsBool(unitController->underAttack, false);
	}
	bestCover->user = unit;
	unit->coverPoint = bestCover;
	return EBTNodeResult::Succeeded;
}

void UBTTFindCover::OnGameplayTaskActivated(UGameplayTask & Task)
{
}
