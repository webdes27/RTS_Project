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

	stateText = CreateDefaultSubobject<UTextRenderComponent>("stateText");
	stateText->SetupAttachment(baseMesh);
}

// Called when the game starts or when spawned
void AHomeBase::BeginPlay()
{
	Super::BeginPlay();	
	if (teamName.Equals(TEXT("Red")))
	{
		team = RED_TEAM;
	}
	else
	{
		team = BLUE_TEAM;
	}
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
					newUnit->Init(RED_TEAM);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Spawn %s"), *teamName));
					newUnit->Init(BLUE_TEAM);
				}
				unitsActive.push(newUnit);
			}
		}
		spawnTimer = timeBetweenSpawns;
	}

	for (unsigned i = 0u; i < CHECKS_PER_FRAME && unitsActive.size() > 0u; ++i)
	{
		AUnit* unit = unitsActive.front();
		unitsActive.pop();
		if (unit->state != AUnit::UnitState::DEAD)
		{
			unitsActive.push(unit);
		}
	}

	stateText->SetText(FString("Units - ") + FString::FromInt(unitsActive.size())
		+ FString("Life - ") + FString::FromInt(life));
}

