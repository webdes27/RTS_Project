// Fill out your copyright notice in the Description page of Project Settings.

#include "CoverPoint.h"
#include "EngineUtils.h"
#include "Engine/Classes/Components/TextRenderComponent.h"
#include "Unit.h"

ACoverPoint::ACoverPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	stateText = CreateDefaultSubobject<UTextRenderComponent>("stateText");
	stateText->SetupAttachment(RootComponent);
}

void ACoverPoint::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACoverPoint::Tick(float DeltaTime)
{
	stateText->SetText(FString("R ") + FString::FromInt(redOnSight) + FString(" B ") + FString::FromInt(blueOnSight));
	if (user)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), user->GetActorLocation(), FColor::Yellow, false, 0.1f, 0, 1);
	}
	if (timer > 0.f)
	{
		timer -= DeltaTime;
	}
	else
	{
		timer = 1.5f;
		redOnSight = 0;
		blueOnSight = 0;
		TArray<AActor*> units;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnit::StaticClass(), units);
		for (AActor* unitActor : units)
		{
			AUnit* unit = Cast<AUnit>(unitActor);
			FHitResult OutHit;
			FVector ForwardVector = GetActorForwardVector();
			FVector Start = GetActorLocation();
			FVector End = unit->laserPoint->GetComponentLocation();
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
			{
				if (OutHit.bBlockingHit)
				{					
					if (OutHit.Actor->IsA(AUnit::StaticClass())) //Shoot unit
					{
						AUnit* unitHit = (AUnit*)OutHit.Actor.Get();

						if (unitHit->unitTeam == 0)
						{
							++redOnSight;
						}
						else
						{
							++blueOnSight;
						}
					}
				}
			}
		}
	}
}

