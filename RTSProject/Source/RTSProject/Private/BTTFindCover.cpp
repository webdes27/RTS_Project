// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTFindCover.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "CoverPoint.h"
#include "../Public/Unit.h"
#include "../Public/UnitAIController.h"
#include "../Public/Homebase.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTFindCover::ExecuteTask(UBehaviorTreeComponent & ownerComp, uint8 * nodeMemory)
{
	AUnitAIController* unitController = Cast<AUnitAIController>(ownerComp.GetOwner());
	AUnit* unit = unitController->unit;
	TArray<AActor*> covers;	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoverPoint::StaticClass(), covers);
	ACoverPoint* bestCover = nullptr;
	if (unit->coverPoint)
	{
		unit->coverPoint->user = nullptr;
		unit->coverPoint = nullptr;
	}
	for (AActor* cover : covers)
	{
		ACoverPoint* cp = Cast<ACoverPoint>(cover);
		if (std::find(cp->targetsOnSight.begin(), cp->targetsOnSight.end(), unitController->enemy) != cp->targetsOnSight.end())
		{
			if (bestCover == nullptr && cp->user == nullptr)
			{
				if (cp != unit->lastCoverPoint)
				{
					bestCover = cp;
				}
			}
			else
			{
				if (bestCover == nullptr && cp != unit->lastCoverPoint)
				{
					bestCover = cp;
				}
				else
				{
					if (unit->unitTeam == 0)
					{
						if (cp->user == nullptr && cp != unit->lastCoverPoint)
						{
							if (bestCover->blueOnSight > cp->blueOnSight)
							{
								bestCover = cp;
							}
							else if (bestCover->blueOnSight == cp->blueOnSight
								&& cp != unit->lastCoverPoint
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
							if (bestCover->redOnSight > cp->redOnSight && cp != unit->lastCoverPoint)
							{
								bestCover = cp;
							}
							else if (bestCover->redOnSight == cp->redOnSight
								&& cp != unit->lastCoverPoint
								&& FVector::Dist(bestCover->GetActorLocation(), unit->GetActorLocation()) > FVector::Dist(cp->GetActorLocation(), unit->GetActorLocation()))
							{
								bestCover = cp;
							}
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
		bestCover->user = unit;
		unit->coverPoint = bestCover;
		unit->lastCoverPoint = bestCover;
	}
	
	return EBTNodeResult::Succeeded;
}

void UBTTFindCover::OnGameplayTaskActivated(UGameplayTask & Task)
{
}
