// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/Unit.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "AI/NavigationSystemHelpers.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	laserBeam = CreateDefaultSubobject<UParticleSystemComponent>("LaserBeam");
	laserBeam->SetupAttachment(RootComponent);
	laserBeam->bAutoActivate = false;	
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	//target = 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundActors);

	for (AActor* actor : FoundActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Found target point %s"), *actor->GetName()));		
	}

	GetTarget();

	navSystem = GetWorld()->GetNavigationSystem();

	bUseControllerRotationYaw = false; //Smooth rotation

	//laserBeam->SetTemplate(laserBeam_);
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Yellow, FString::Printf(TEXT("DT %s"), *FString::SanitizeFloat(DeltaTime)));

	if (Controller->GetPawn()->GetVelocity().Size() < 10)
		GetTarget();
	
	if (atRangeTargets.size() > 0 && fireTimer <= 0.f)
	{
		AActor* t = *atRangeTargets.begin();
		fireTarget = t->GetActorLocation();
		laserBeam->ActivateSystem();
		fireTimer = fireRate;
		laserTimer = laserDuration;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Shoot to %s"), *t->GetName()));
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
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Deactivate")));
		laserBeam->SetBeamSourcePoint(0, FVector(10000,10000,10000), 0);
		laserBeam->SetBeamTargetPoint(0, FVector(10000, 10000, 10000), 0);
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
	atRangeTargets.insert(Other);
}

void AUnit::NotifyActorEndOverlap(AActor* Other)
{
	atRangeTargets.erase(Other);
}

