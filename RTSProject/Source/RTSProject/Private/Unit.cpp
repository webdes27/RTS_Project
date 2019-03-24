// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Unit.h"
#include "../Public/HomeBase.h"

#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"

#include <assert.h>

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
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHomeBase::StaticClass(), FoundActors);
	
	SpawnDefaultController();

	bUseControllerRotationYaw = false; //Smooth rotation	
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceWeight = 0.5f;
		
	unitMesh->OnComponentHit.AddDynamic(this, &AUnit::OnCompHit);

}
// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

inline bool AUnit::IsUnderAttack() 
{
	if (attacker != nullptr)
	{
		enemy = attacker;
		state = UnitState::AIM;
		bUnderAttack = true;
	}
	return bUnderAttack;
}
inline void AUnit::GetDestination()
{
	targetDestination = FoundActors[rand() % FoundActors.Num()];
	unitAIController = Cast<AUnitAIController>(GetController()); //TODO: Performance issue?
	if (unitAIController != nullptr)
	{
		unitAIController->owner = this;
		unitAIController->MoveToActor(targetDestination);
		bArrived = false;
		state = UnitState::MOVE;
	}
}

inline bool AUnit::HasArrived() const
{
	return bArrived;
}

inline void AUnit::ResetShooting()
{
	state = UnitState::IDLE;
	bHasTarget = false;
	enemy = nullptr;
	attacker = nullptr;
	bUnderAttack = false;
}


void AUnit::Init(int team)
{
	unitTeam = team;

	if (unitTeam == RED_TEAM)
	{
		unitMesh->SetMaterial(0, redUnitMaterial);
		laserBeam->SetMaterial(0, redUnitMaterial);
	}
	else
	{
		unitMesh->SetMaterial(0, blueUnitMaterial);
		laserBeam->SetMaterial(0, blueUnitMaterial);
	}
	AActor* ownBase = nullptr;

	for (AActor* hbActor : FoundActors)
	{
		AHomeBase* hb = (AHomeBase*)hbActor;
		if (hb->team == unitTeam)
		{
			ownBase = hbActor;
		}
	}
	
	assert(ownBase != nullptr);

	FoundActors.Remove(ownBase);
}

void AUnit::Aiming(AActor* target)
{	
	if (target->GetName().Equals(FString("None")))
	{
		state = UnitState::IDLE;
		return;
	}
	FVector aimNoise = target->GetActorRightVector() * FMath::RandRange(-50.f, 50.f); // TODO: Add accuracy parameter to unit
	FRotator newRot = (target->GetActorLocation() + aimNoise - GetActorLocation()).Rotation();
	FQuat q = FQuat::FastLerp(GetActorRotation().Quaternion(), newRot.Quaternion(), .1f);
	newRot = q.Rotator();
	SetActorRotation(q.Rotator());
	//DrawDebugLine(GetWorld(), GetActorLocation(), target->GetActorLocation(), FColor::Green, false, 0.05f, 0, 1);
	float angDist = q.AngularDistance(GetActorRotation().Quaternion());
	if (angDist < .01f && fireTimer <= 0.f)
	{
		state = UnitState::SHOOT;
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

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		laserBeam->ActivateSystem(true);
		laserBeam->SetBeamSourcePoint(0, Start, 0);
		if (OutHit.bBlockingHit)
		{			
			laserBeam->SetBeamTargetPoint(0, OutHit.ImpactPoint, 0); 
			if (OutHit.Actor->IsA(AUnit::StaticClass())) //Shoot unit
			{				
				AUnit* unit = (AUnit*)OutHit.Actor.Get();
				if (unit && unit->unitTeam != unitTeam)
				{
					unit->life -= damage;
					if (enemy != nullptr)
						enemy->attacker = this;
				}
				else
				{
					ResetShooting();
					return;
				}
			}
			else if (OutHit.Actor->IsA(AHomeBase::StaticClass())) //Shoot to base
			{
				AHomeBase* enemyBase = (AHomeBase*)OutHit.Actor.Get();
				enemyBase->life -= damage;
			}
			else //Target out of sight
			{
				ResetShooting();
				return;
			}
			if (unitTeam == 0)
				DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 0.1f, 0, 1);
			else
				DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Blue, false, 0.1f, 0, 1);
		}
		else
		{
			laserBeam->SetBeamTargetPoint(0, End, 0);
			if (unitTeam == RED_TEAM)
				DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 1);
			else
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.1f, 0, 1);
			ResetShooting();
			return;
		}
		fireTimer = fireRate;
		laserTimer = laserDuration;
		if (enemy != nullptr)
		{
			if (enemy->state == UnitState::DEAD)
				state = UnitState::CHECK_ENEMIES;
			else
				state = UnitState::AIM;
		}
		else if (bAttackingBase)
		{
			state = UnitState::AIM_BASE;			
		}

	}
	else
	{		
		ResetShooting();
	}

}

void AUnit::CheckEnemies()
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
				FHitResult OutHit;
				FVector ForwardVector = GetActorForwardVector();
				FVector Start = laserPoint->GetComponentLocation();;
				FVector End = unit->GetActorLocation();
				FCollisionQueryParams CollisionParams;
				CollisionParams.AddIgnoredActor(this);
			
				if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
				{
					laserBeam->ActivateSystem(true);
					laserBeam->SetBeamSourcePoint(0, Start, 0);
					if (OutHit.bBlockingHit)
					{
						laserBeam->SetBeamTargetPoint(0, OutHit.ImpactPoint, 0);
						if (OutHit.Actor->IsA(AUnit::StaticClass()))
						{
							enemy = unit;
							bHasTarget = true;
							bAttackingBase = false;
						}
					}
				}
			}
			break;
		}
	}
}

void AUnit::CheckGoal()
{
	switch (goal)
	{
	case UnitGoal::ATTACK_BASE:
		if (FVector::Dist(GetActorLocation(), targetDestination->GetActorLocation()) < unitSphere->GetScaledSphereRadius()) //TODO: this harcoded value stinks!
		{
			FHitResult OutHit;
			FVector ForwardVector = GetActorForwardVector();
			FVector Start = laserPoint->GetComponentLocation();
			FVector Direction = targetDestination->GetActorLocation() - laserPoint->GetComponentLocation();
			Direction.Normalize();
			FVector End = ((Direction * 10000.f) + Start);
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);

			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
			{
				if (OutHit.bBlockingHit)
				{
					//DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Yellow, false, 1, 0, 1);
					laserBeam->SetBeamTargetPoint(0, OutHit.ImpactPoint, 0);
					if (OutHit.Actor->IsA(AHomeBase::StaticClass()))
					{
						AHomeBase* enemyBase = (AHomeBase*)OutHit.Actor.Get();
						if (enemyBase->team != unitTeam)
						{
							state = UnitState::AIM_BASE;
							bAttackingBase = true;
							enemy = nullptr;
						}
					}
				}
			}
		}
	}
}

void AUnit::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
		state = UnitState::IDLE;	
		SetActorLocation(GetActorLocation() + GetActorRightVector());
	}
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (life <= 0)
	{
		state = UnitState::DEAD;
	}

	if (fireTimer > 0.f)
	{
		fireTimer -= DeltaTime;
	}

	if (attacker != nullptr && attacker->state == UnitState::DEAD)
	{
		attacker = nullptr;
		bUnderAttack = false;
	}

	switch (state)
	{
	case UnitState::IDLE:
		stateText->SetText(FString("I - ") + FString::FromInt(life));
		if (!IsUnderAttack())
		{
			state = UnitState::CHECK_ENEMIES;
		}
		break;

	case UnitState::CHECK_ENEMIES:
		stateText->SetText(FString("C - ") + FString::FromInt(life));
		enemy = nullptr;
		CheckEnemies();
		if (enemy != nullptr)
		{
			state = UnitState::AIM;
		}
		else
		{
			GetDestination();
			state = UnitState::MOVE;
		}
		break;

	case UnitState::MOVE:
		stateText->SetText(FString("M - ") + FString::FromInt(life));
		//DrawDebugLine(GetWorld(), GetActorLocation(), targetDestination->GetActorLocation(), FColor::White, false, 0.05f, 0, 1);
		enemy = nullptr;
		CheckGoal();
		CheckEnemies();
		if (IsUnderAttack() || enemy != nullptr)
		{
			state = UnitState::AIM;
		}
		else
		{
			if (HasArrived() || FVector::Distance(GetActorLocation(), targetDestination->GetActorLocation()) < 10)
			{
				state = UnitState::IDLE;
			}
			else if (bAbortedPath)
			{
				DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0,0,1000), FColor::Emerald, false, 0.05f, 0, 1);
				unitAIController->MoveToActor(targetDestination);
				bAbortedPath = false;
			}
		}
		break;

	case UnitState::AIM:
		stateText->SetText(FString("A - ") + FString::FromInt(life));
		if (enemy != nullptr && enemy->state != UnitState::DEAD)
			Aiming(enemy);
		else
			state = UnitState::IDLE;
		break;

	case UnitState::AIM_BASE:
		stateText->SetText(FString("AB - ") + FString::FromInt(life));	
		CheckEnemies();
		if (enemy != nullptr)
		{
			state = UnitState::AIM;		
		}
		else
		{
			Aiming(targetDestination);
		}
		break;

	case UnitState::SHOOT:
		stateText->SetText(FString("S - ") + FString::FromInt(life));
		Shooting();
		break;

	case UnitState::DEAD:
		--framesToDestroy;
		if (framesToDestroy <= 0)
		{
			Destroy();
		}
		break;
	default:
		break;
	}
}
	
