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

	for (AActor* actor : FoundActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Found target point %s"), *actor->GetName()));
	}

	SpawnDefaultController();

	GetTarget();

	bUseControllerRotationYaw = false; //Smooth rotation

	
}
// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

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

void AUnit::GetTarget()
{
	target = FoundActors[rand() % FoundActors.Num()];
}
// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUnit::NotifyActorBeginOverlap(AActor* Other)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("%s targets %s"),*GetName(), *Other->GetName()));
	atRangeTargets.insert(Other);
}

void AUnit::NotifyActorEndOverlap(AActor* Other)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("%s no longer targets %s"), *GetName(), *Other->GetName()));
	atRangeTargets.erase(Other);
}

void AUnit::Idle()
{
	if (atRangeTargets.size() > 0 && fireTimer <= 0.f)
	{
		AActor* t = *atRangeTargets.begin();
		fireTarget = t->GetActorLocation();
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Shoot to %s"), *t->GetName()));
		state = UnitState::AIMING;
	}
	else
	{
		GetTarget();
		AController* controller = GetController();
		unitAIController = Cast<AUnitAIController>(controller);
		if (unitAIController != nullptr)
		{
			unitAIController->MoveToActor(target);
			unitAIController->arrived = false;
			state = UnitState::MOVING;
		}
	}
}

void AUnit::Moving()
{
	if (unitAIController->arrived || Controller->GetPawn()->GetVelocity().Size() < 10)
	{
		state = UnitState::IDLE;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Arrived")));
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
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Ang %s"), *FString::SanitizeFloat(angDist)));
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