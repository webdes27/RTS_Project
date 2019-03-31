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
		FVector targetPoint = unitController->enemy->GetActorLocation();
		targetPoint.Z = unit->laserPoint->GetComponentLocation().Z;
		FVector aimNoise = unitController->enemy->GetActorRightVector() * FMath::RandRange(-1.f, 1.f); // TODO: Add accuracy parameter to unit
		FRotator newRot = (targetPoint + aimNoise - unit->laserPoint->GetComponentLocation()).Rotation();

		FQuat q = FQuat::FastLerp(unit->laserPoint->GetComponentRotation().Quaternion(), newRot.Quaternion(), .3f);
		unit->SetActorRotation(q.Rotator());
		float angDist = q.AngularDistance(unit->GetActorRotation().Quaternion());
		if (angDist < .01f)
		{
			FHitResult OutHit;
			FVector ForwardVector = unit->GetActorForwardVector();
			/*unit->laserPoint->SetWorldRotation((unitController->enemy->GetActorLocation()
				+ aimNoise - unit->laserPoint->GetComponentLocation()).Rotation().Quaternion());*/
			FVector Start = unit->laserPoint->GetComponentLocation();
			//FVector End = ((unit->laserPoint->GetForwardVector() * 10000.f) + Start);
			FVector End = (ForwardVector * 10000.f) + Start;
			FCollisionQueryParams CollisionParams;
			//CollisionParams.AddIgnoredActor(unit);			
			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
			{
				//laserBeam->ActivateSystem(true);
				if (OutHit.bBlockingHit)
				{
					if (OutHit.Actor->IsA(AUnit::StaticClass())) //Shoot unit
					{
						AUnit* unitHit = (AUnit*)OutHit.Actor.Get();
						if (unitHit && unit->unitTeam != unitHit->unitTeam)
						{
							if (unitHit->TakeDamage(10))
							{
								BB->SetValueAsObject(unitController->target, nullptr);
								return EBTNodeResult::Failed;
							}
							unit->ShootEvent(OutHit.ImpactPoint);
						}	
						
					}
					else
					{
						--unit->shootCounter;
						if (unit->shootCounter <= 0)
						{
							BB->SetValueAsObject(unitController->target, nullptr);
							unit->shootCounter = unit->shootAttempts;
						}
						unit->ShootEvent(OutHit.ImpactPoint);
					}
					/*else if (OutHit.Actor->IsA(AHomeBase::StaticClass())) //Shoot to base
					{
						AHomeBase* enemyBase = (AHomeBase*)OutHit.Actor.Get();
						enemyBase->life -= damage;
					}
					*/					
					
				}				
			}
			else
			{
				BB->SetValueAsObject(unitController->target, nullptr);
				unit->laserBeam->SetBeamTargetPoint(0, End, 0);				
				unit->laserBeam->Activate(true);
			}
			
			return EBTNodeResult::Succeeded;
		}
		
		BB->SetValueAsFloat(unitController->angularDistance, angDist);
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}

void UBTTShoot::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

