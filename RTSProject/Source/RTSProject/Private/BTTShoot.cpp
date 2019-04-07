// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTShoot.h"
#include "DrawDebugHelpers.h"
#include "../Public/Unit.h"
#include "../Public/UnitAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTShoot::ExecuteTask(UBehaviorTreeComponent &ownerComp, uint8* nodeMemory)
{

	AUnitAIController* unitController = Cast<AUnitAIController>(ownerComp.GetOwner());
	AUnit* unit = unitController->unit;
	UBlackboardComponent* BB = ownerComp.GetBlackboardComponent();
	if (unitController->enemy && unit)
	{
		AUnit* enemy = (AUnit*)unitController->enemy;
		FVector targetPoint = enemy->headPoint->GetComponentLocation();
		FVector aimNoise = FVector(0,0, FMath::RandRange(-30.f, 30.f)); // TODO: Add accuracy parameter to unit
		FRotator newRot = (targetPoint + aimNoise - unit->laserPoint->GetComponentLocation()).Rotation();

		FQuat q = FQuat::FastLerp(unit->laserPoint->GetComponentRotation().Quaternion(), newRot.Quaternion(), .3f);
		unit->SetActorRotation(q.Rotator());
		float angDist = q.AngularDistance(unit->GetActorRotation().Quaternion());
		if (angDist < .01f)
		{
			FHitResult OutHit;
			FVector ForwardVector = unit->GetActorForwardVector();
			FVector Start = unit->laserPoint->GetComponentLocation();
		
			FVector End = (ForwardVector * 10000.f) + Start;
			FCollisionQueryParams CollisionParams;
		
			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
			{
				if (OutHit.bBlockingHit)
				{
					if (OutHit.Actor->IsA(AUnit::StaticClass())) //Shoot unit
					{
						AUnit* unitHit = (AUnit*)OutHit.Actor.Get();
						if (unitHit && unit->unitTeam != unitHit->unitTeam)
						{
							if (unitHit->TakeDamage(10))
							{
								unit->ShootEvent(OutHit.ImpactPoint);
								BB->SetValueAsObject(unitController->target, nullptr);
								((AUnit*)unitController->enemy)->DeadEvent(true);
								return EBTNodeResult::Failed;
							}
						}	
						
					}
					else
					{
						--unit->shootCounter;
						if (unit->shootCounter <= 0)
						{
							unit->ShootEvent(OutHit.ImpactPoint);
							BB->SetValueAsObject(unitController->target, nullptr);
							unit->shootCounter = unit->shootAttempts;
							return EBTNodeResult::Failed;
						}
						
					}
					/*else if (OutHit.Actor->IsA(AHomeBase::StaticClass())) //Shoot to base
					{
						AHomeBase* enemyBase = (AHomeBase*)OutHit.Actor.Get();
						enemyBase->life -= damage;
					}
					*/					
					
				}				
			}								
		}						
	}
	
	return EBTNodeResult::Succeeded;
}

void UBTTShoot::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

