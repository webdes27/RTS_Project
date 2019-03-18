// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/HomeBase.h"


// Sets default values
AHomeBase::AHomeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>("baseMesh");
	RootComponent = baseMesh;
	static ConstructorHelpers::FObjectFinder<UBlueprint> unitBP(TEXT("Blueprint'/Game/Blueprints/BP_Unit.BP_Unit'"));
	if (unitBP.Object) {
		unit = (UClass*)unitBP.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AHomeBase::BeginPlay()
{
	Super::BeginPlay();		
}

// Called every frame
void AHomeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	spawnTimer -= DeltaTime;

	if (spawnTimer <= 0.f)
	{
		UWorld* const World = GetWorld(); // get a reference to the world
		if (World) {
			// if world exists
			AUnit* newUnit = World->SpawnActor<AUnit>(unit, GetActorLocation(), GetActorRotation());
			if (newUnit)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Spawn %s"), *teamName));
				if (teamName.Equals(TEXT("Red")))
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Spawn %s"), *teamName));
					newUnit->Init(0);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Spawn %s"), *teamName));
					newUnit->Init(1);
				}
			}
		}
		spawnTimer = timeBetweenSpawns;
	}
}

