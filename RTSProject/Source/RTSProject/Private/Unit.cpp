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

	unitSphere = CreateDefaultSubobject<USphereComponent>("UnitSphere");
	unitSphere->SetupAttachment(RootComponent);

	AIControllerClass = AUnitAIController::StaticClass();
	AController* controller = GetController();
	unitAIController = Cast<AUnitAIController>(controller);	
	int x = 0;
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundActors);

	SpawnDefaultController();

	GetTarget();

	bUseControllerRotationYaw = false; //Smooth rotation		
		
	//overlapDelegate.BindRaw(&AUnit::OnOverlapCompleted);
}
// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	CheckTargets();
	//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Yellow, FString::Printf(TEXT("DT %s"), *FString::SanitizeFloat(DeltaTime)));

	switch (state)
	{
	case UnitState::IDLE:
	{
		Idle();
		break;
	}
	case UnitState::MOVING:
	{
		Moving();
		break;
	}
	case UnitState::AIMING:
	{
		Aiming();
		break;
	}
	case UnitState::SHOOTING:
	{
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
		laserBeam->SetBeamTargetPoint(0, fireTarget, 0);
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
		unitMesh->SetMaterial(0, redUnitMaterial);
	else
		unitMesh->SetMaterial(0, blueUnitMaterial);
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
	if (bHasTarget && fireTimer <= 0.f)
	{
		state = UnitState::AIMING;
	}
	else
	{
		if (!bHasTarget)
		{
			GetTarget(); //TODO:Rename this to avoid confusion with shoot target
			AController* controller = GetController();
			unitAIController = Cast<AUnitAIController>(controller);
			if (unitAIController != nullptr)
			{
				unitAIController->MoveToActor(target);
				unitAIController->arrived = false;
				state = UnitState::MOVING;
			}
		}
		else
		{
			if (unitAIController != nullptr)
			{
				unitAIController->StopMovement();
			}
		}
	}
}

void AUnit::Moving()
{
	if (unitAIController->arrived || Controller->GetPawn()->GetVelocity().Size() < 10)
	{
		state = UnitState::IDLE;		
	}
	Idle();
}

void AUnit::Aiming()
{	
	FRotator newRot = (fireTarget - GetActorLocation()).Rotation();
	FQuat q = FQuat::FastLerp(GetActorRotation().Quaternion(), newRot.Quaternion(), .1f);
	newRot = q.Rotator();
	SetActorRotation(q.Rotator());

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
	laserBeam->ActivateSystem(true);
	laserBeam->SetBeamSourcePoint(0, GetActorLocation(), 0);
	laserBeam->SetBeamTargetPoint(0, fireTarget, 0);
	fireTimer = fireRate;
	laserTimer = laserDuration;
	state = UnitState::IDLE;
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
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Target %s"), *unit->GetName()));
				fireTarget = unit->GetActorLocation();
				bHasTarget = true;
			}
			break;
		}
	}
}

