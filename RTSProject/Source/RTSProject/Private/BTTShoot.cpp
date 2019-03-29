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
		FVector aimNoise = unitController->enemy->GetActorRightVector() * FMath::RandRange(-15.f, 15.f); // TODO: Add accuracy parameter to unit
		FRotator newRot = (unitController->enemy->GetActorLocation() + aimNoise - unit->GetActorLocation()).Rotation();

		FQuat q = FQuat::FastLerp(unit->GetActorRotation().Quaternion(), newRot.Quaternion(), .1f);
		unit->SetActorRotation(q.Rotator());
		float angDist = q.AngularDistance(unit->GetActorRotation().Quaternion());
		if (angDist < .01f)
		{
			FHitResult OutHit;
			FVector ForwardVector = unit->GetActorForwardVector();
			unit->laserPoint->SetWorldRotation((unitController->enemy->GetActorLocation()
				+ aimNoise - unit->laserPoint->GetComponentLocation()).Rotation().Quaternion());
			FVector Start = unit->laserPoint->GetComponentLocation();
			FVector End = ((unit->laserPoint->GetForwardVector() * 10000.f) + Start);
			FCollisionQueryParams CollisionParams;
			//CollisionParams.AddIgnoredActor(unit);

			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
			{
				//laserBeam->ActivateSystem(true);
				//laserBeam->SetBeamSourcePoint(0, Start, 0);
				if (OutHit.bBlockingHit)
				{
					//laserBeam->SetBeamTargetPoint(0, OutHit.ImpactPoint, 0);
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
						}						
					}
					else
					{
						BB->SetValueAsObject(unitController->target, nullptr);
					}
					/*else if (OutHit.Actor->IsA(AHomeBase::StaticClass())) //Shoot to base
					{
						AHomeBase* enemyBase = (AHomeBase*)OutHit.Actor.Get();
						enemyBase->life -= damage;
					}
					*/
					if (unit->unitTeam == 0)
						DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 0.3f, 0, 2);
					else
						DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Blue, false, 0.3f, 0, 2);
				}				
			}
			else
			{
				//laserBeam->SetBeamTargetPoint(0, End, 0);
				if (unit->unitTeam == 0)
					DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.3f, 0, 2);
				else
					DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.3f, 0, 2);

			}
			return EBTNodeResult::Failed;
		}
		
		BB->SetValueAsFloat(unitController->angularDistance, angDist);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

void UBTTShoot::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

