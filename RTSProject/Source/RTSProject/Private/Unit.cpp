// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Unit.h"
#include "../Public/HomeBase.h"

#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include <assert.h>

// Sets default values
AUnit::AUnit()
{
	laserBeam = CreateDefaultSubobject<UParticleSystemComponent>("LaserBeam");
	laserBeam->SetupAttachment(RootComponent);
	laserBeam->bAutoActivate = false;	

	unitMesh = GetMesh();

	laserPoint = CreateDefaultSubobject<USceneComponent>("laserPoint");
	laserPoint->SetupAttachment(RootComponent);

	stateText = CreateDefaultSubobject<UTextRenderComponent>("stateText");
	stateText->SetupAttachment(RootComponent);

	AIControllerClass = AUnitAIController::StaticClass();
	AController* controller = GetController();
	unitAIController = Cast<AUnitAIController>(controller);
	
	sensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("sensingComponent");
	sensingComponent->SetPeripheralVisionAngle(90);
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDefaultController();

	bUseControllerRotationYaw = false; //Smooth rotation	
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceWeight = 0.5f;

	if (sensingComponent)
	{
		sensingComponent->OnSeePawn.AddDynamic(this, &AUnit::OnUnitSeen);
		sensingComponent->bOnlySensePlayers = false;
		sensingComponent->bEnableSensingUpdates = true;
		sensingComponent->SetComponentTickInterval(.5f);
		sensingComponent->SightRadius = 10000;
	}
		
}
void AUnit::OnUnitSeen(APawn* pawn)
{
	//DrawDebugLine(GetWorld(), GetActorLocation(), pawn->GetActorLocation(), FColor::Green, false, 0.2f, 0, 1);
	AUnit* unit = Cast<AUnit>(pawn);
	AController* controller = GetController();
	unitAIController = Cast<AUnitAIController>(controller);
	if (unit && unitAIController)
	{
		unitAIController->SetTarget(pawn);
	}
}
// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUnit::Init(int team, AHomeBase* base)
{
	homeBase = base;
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
	
}
bool AUnit::TakeDamage(int damage)
{
	life -= damage;
	if (life <= 0)
	{
		SetActorLocation(homeBase->GetActorLocation());
		unitAIController = Cast<AUnitAIController>(GetController());
		if (unitAIController)
		{
			UBlackboardComponent* BB = unitAIController->GetBlackboardComponent();
			if (BB)
			{
				BB->SetValueAsObject(unitAIController->target, nullptr);
			}
		}
		return true;
	}
	return false;
}

/*
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
		newRot = (target->GetActorLocation() - laserPoint->GetComponentLocation()).Rotation();
		laserPoint->SetWorldRotation(newRot);
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
	FVector Start = laserPoint->GetComponentLocation();
	FVector End = ((laserPoint->GetForwardVector() * 10000.f) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		//laserBeam->ActivateSystem(true);
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
				DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 0.3f, 0, 10);
			else
				DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Blue, false, 0.3f, 0, 10);
		}
		else
		{
			laserBeam->SetBeamTargetPoint(0, End, 0);
			if (unitTeam == RED_TEAM)
				DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.3f, 0, 10);
			else
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.3f, 0, 10);
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
	for (unsigned i = 0u; i < homeBase->unitsActive.size(); ++i)
	{
		query.AddIgnoredActor(homeBase->unitsActive.front());
		homeBase->unitsActive.push(homeBase->unitsActive.front());
		homeBase->unitsActive.pop();
	}
	auto sphere = FCollisionShape::MakeSphere(unitSphere->GetScaledSphereRadius());
	FVector start = GetActorLocation();
	auto traceType = EAsyncTraceType::Multi;
	FCollisionResponseParams response;
	TArray<FOverlapResult> overlaps;
	GetWorld()->OverlapMultiByChannel
	(
		overlaps,
		start,
		FQuat::Identity,
		ECollisionChannel::ECC_WorldDynamic,
		sphere,
		query,
		response
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
	//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
		state = UnitState::IDLE;	
		float rDist = FVector::Dist(GetActorLocation() + GetActorRightVector(), OtherActor->GetActorLocation());
		float lDist = FVector::Dist(GetActorLocation() - GetActorRightVector(), OtherActor->GetActorLocation());
		if (rDist < lDist)
			SetActorLocation(GetActorLocation() - GetActorRightVector() * 5);
		else
			SetActorLocation(GetActorLocation() + GetActorRightVector() * 5);

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
	*/
