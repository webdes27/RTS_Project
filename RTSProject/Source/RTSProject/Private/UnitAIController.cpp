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
		BBComp->SetValueAsObject(target, targetPawn);
		BBComp->SetValueAsFloat(angularDistance, 1.f);
		enemy = targetPawn;
	}
}

void AUnitAIController::Tick(float DeltaTime)
{

}

bool AUnitAIController::IsTargetInSight(FVector Start)
{
	if (enemy)
	{
		FHitResult OutHit;
		FVector End = ((AUnit*)enemy)->headPoint->GetComponentLocation();
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(unit);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			if (OutHit.bBlockingHit)
			{
				AActor* hitActor = OutHit.Actor.Get();
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Hit %s Enemy %s Result %s"), *hitActor->GetName(), *enemy->GetName(), *FString::FromInt(hitActor == enemy)));
				if (hitActor == enemy)
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

void AUnitAIController::Possess(APawn* pawn)
{
	Super::Possess(pawn);

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
