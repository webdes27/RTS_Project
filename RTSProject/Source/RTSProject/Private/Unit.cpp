// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Unit.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	laserBeam = CreateDefaultSubobject<UParticleSystemComponent>("LaserBeam");
	laserBeam->SetupAttachment(RootComponent);
	laserBeam->bAutoActivate = false;	
	
	unitMesh = CreateDefaultSubobject<UStaticMeshComponent>("UnitMesh");
	unitMesh->SetupAttachment(RootComponent);

	laserPoint = CreateDefaultSubobject<USceneComponent>("laserPoint");
	laserPoint->SetupAttachment(unitMesh);

	stateText = CreateDefaultSubobject<UTextRenderComponent>("stateText");
	stateText->SetupAttachment(unitMesh);

	unitSphere = CreateDefaultSubobject<USphereComponent>("UnitSphere");
	unitSphere->SetupAttachment(RootComponent);

	AIControllerClass = AUnitAIController::StaticClass();
	AController* controller = GetController();
	unitAIController = Cast<AUnitAIController>(controller);
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundActors);

	SpawnDefaultController();

	GetTarget();

	bUseControllerRotationYaw = false; //Smooth rotation	
	GetCharacterMovement()->bUseRVOAvoidance = true;

}
// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	if (life <= 0)
	{
		Destroy();
		state = UnitState::DEAD;
	}

	CheckTargets();
	if (!bHasTarget)
		CheckArrived();
	//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Yellow, FString::Printf(TEXT("DT %s"), *FString::SanitizeFloat(DeltaTime)));
	DrawDebugLine(GetWorld(), GetActorLocation(), target->GetActorLocation(), FColor::White, false, 0.01f, 0, 1);
	switch (state)
	{
	case UnitState::IDLE:
	{
		stateText->SetText(FString("I") + FString::FromInt(bHasTarget) + FString::FromInt(bArrived));
		Idle();
		break;
	}
	case UnitState::MOVING:
	{
		stateText->SetText(FString("M") + FString::FromInt(bHasTarget) + FString::FromInt(bArrived));
		Moving();
		break;
	}
	case UnitState::AIMING:
	{
		stateText->SetText(FString("A") + FString::FromInt(bHasTarget) + FString::FromInt(bArrived));
		Aiming();
		break;
	}
	case UnitState::SHOOTING:
	{
		stateText->SetText(FString("S") + FString::FromInt(bHasTarget) + FString::FromInt(bArrived));
		Shooting();
		break;
	}
	}	
	if (fireTimer > 0.f)
	{
		fireTimer -= DeltaTime;
	}

	if (laserTimer > 0.f)
	{
		laserTimer -= DeltaTime;
		laserBeam->SetBeamSourcePoint(0, GetActorLocation(), 0);
		laserBeam->SetBeamTargetPoint(0, enemy->GetActorLocation(), 0);
	}
	else
	{
		laserBeam->DeactivateSystem();
	}
	
}

void AUnit::Init(int team)
{
	unitTeam = team;

	if (unitTeam == 0)
	{
		unitMesh->SetMaterial(0, redUnitMaterial);
		laserBeam->SetMaterial(0, redUnitMaterial);
	}
	else
	{
		unitMesh->SetMaterial(0, blueUnitMaterial);
		laserBeam->SetMaterial(0, blueUnitMaterial);

	}	
	
}
void AUnit::GetTarget()
{
	target = FoundActors[rand() % FoundActors.Num()];
}
// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUnit::Idle()
{
	if (bArrived)
	{
		GetTarget(); //TODO:Rename this to avoid confusion with shoot target
		AController* controller = GetController();
		unitAIController = Cast<AUnitAIController>(controller);
		if (unitAIController != nullptr)
		{
			unitAIController->owner = this;
			unitAIController->MoveToActor(target);
			bArrived = false;
			state = UnitState::MOVING;
		}
	}	
}

void AUnit::Moving()
{	
	if (bHasTarget && fireTimer <= 0.f)
	{
		bArrived = true;
		state = UnitState::AIMING;
		if (unitAIController != nullptr)
		{
			unitAIController->StopMovement();
		}
	}	
}

void AUnit::CheckArrived()
{
	if (bArrived || FVector::Distance(GetActorLocation(), target->GetActorLocation()) < 10)
	{
		bArrived = true;
		state = UnitState::IDLE;
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 0, 1000), FColor::Black, false, 5.f, 0, 1);
	}
}

void AUnit::Aiming()
{	
	FRotator newRot = (enemy->GetActorLocation() - GetActorLocation()).Rotation();
	FQuat q = FQuat::FastLerp(GetActorRotation().Quaternion(), newRot.Quaternion(), .1f);
	newRot = q.Rotator();
	SetActorRotation(q.Rotator());
	DrawDebugLine(GetWorld(), GetActorLocation(), enemy->GetActorLocation(), FColor::Green, false, 0.05f, 0, 1);
	float angDist = q.AngularDistance(GetActorRotation().Quaternion());	
	if (angDist < .01f)
	{
		state = UnitState::SHOOTING;
	}
		
	if (unitAIController != nullptr)
	{
		unitAIController->StopMovement();
	}
}

void AUnit::Shooting()
{
	FHitResult OutHit;
	FVector ForwardVector = GetActorForwardVector();
	FVector Start = laserPoint->GetComponentLocation();;
	FVector End = ((ForwardVector * 10000.f) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if(GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) 
	{
		laserBeam->ActivateSystem(true);
		laserBeam->SetBeamSourcePoint(0, Start, 0);
		if(OutHit.bBlockingHit)
		{
			laserBeam->SetBeamTargetPoint(0, OutHit.ImpactPoint, 0);
			if (OutHit.Actor->IsA(AUnit::StaticClass()))
			{
				AUnit* unit = (AUnit*)OutHit.Actor.Get();				
				unit->life -= damage;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("%s : Target %s life %s"), *GetName(), *unit->GetName(), *FString::FromInt(unit->life)));
			}
			if (unitTeam == 0)
				DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 0.1f, 0, 1);
			else
				DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Blue, false, 0.1f, 0, 1);
		}
		else
		{
			laserBeam->SetBeamTargetPoint(0, End, 0);
			if (unitTeam == 0)
				DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 1);
			else
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.1f, 0, 1);
			state = UnitState::IDLE;
			bHasTarget = false;
		}
		fireTimer = fireRate;
		laserTimer = laserDuration;
		if (enemy->life <= 0)
			state = UnitState::IDLE;
		else
			state = UnitState::AIMING;

	}	
	
}

void AUnit::CheckTargets()
{
	FCollisionQueryParams query;
	query.AddIgnoredActor(this);
	auto sphere = FCollisionShape::MakeSphere(unitSphere->GetScaledSphereRadius());
	FVector start = GetActorLocation();
	auto traceType = EAsyncTraceType::Multi;
	FCollisionResponseParams params;
	TArray<FOverlapResult> overlaps;
	GetWorld()->OverlapMultiByChannel
	(
		overlaps,
		start,
		FQuat::Identity,
		ECollisionChannel::ECC_WorldDynamic,
		sphere,
		query,
		params
	);

	bHasTarget = false;

	for (FOverlapResult r : overlaps)
	{
		if (r.Actor != this && r.Actor->IsA(AUnit::StaticClass()))
		{
			AUnit* unit = (AUnit*)r.Actor.Get();
			if (unit && unit->unitTeam != unitTeam)
			{				
				enemy = unit;
				bHasTarget = true;
			}
			break;
		}
	}
}

