// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/UnitAIController.h"
#include "../Public/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"


AUnitAIController::AUnitAIController()
{
	BBComp = CreateDefaultSubobject<UBlackboardComponent>("BBComp");
	BTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BTreeComp");
	PrimaryActorTick.bCanEverTick = true;
}

void AUnitAIController::SetTarget(APawn* targetPawn)
{
	AUnit* seenUnit = Cast<AUnit>(targetPawn);
	if (seenUnit && seenUnit->unitTeam != unit->unitTeam)
	{
		/*BBComp->SetValueAsObject(target, targetPawn);
		BBComp->SetValueAsFloat(angularDistance, 1.f);
		enemy = targetPawn;
		*/
		targets.Add(targetPawn);
	}
}

void AUnitAIController::Tick(float DeltaTime)
{
	/*if (enemy)
	{
		DrawDebugLine(GetWorld(), unit->GetActorLocation(), enemy->GetActorLocation(), FColor::Green, false, 0.2f, 0, 1);
		DrawDebugLine(GetWorld(), enemy->GetActorLocation(), enemy->GetActorLocation() + FVector(0, 0, 1000), FColor::Green, false, 0.2f, 0, 1);
	}*/
}

bool AUnitAIController::IsTargetInSight(FVector Start, APawn* target)
{
	if (target)
	{
		FHitResult OutHit;
		FVector End = ((AUnit*)target)->headPoint->GetComponentLocation();
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(unit);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			if (OutHit.bBlockingHit)
			{
				AActor* hitActor = OutHit.Actor.Get();
				if (hitActor == target)
				{
					seekAttempts = AI_PATIENCE;
					return true;
				}
				else
				{
					--seekAttempts;
					if (seekAttempts <= 0)
					{
						seekAttempts = AI_PATIENCE;
						return false;
					}
				}
			}
		}
	}
	return false;
}

void AUnitAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	unit = Cast<AUnit>(pawn);	

	if (unit)
	{
		if (unit->BTree->BlackboardAsset)
		{
			BBComp->InitializeBlackboard(*unit->BTree->BlackboardAsset);

			BTreeComp->StartTree(*unit->BTree);
		}

	}
}
