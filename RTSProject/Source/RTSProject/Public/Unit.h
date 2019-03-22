// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "UnitAIController.h"
#include <unordered_set>
#include "Unit.generated.h"

UCLASS()
class RTSPROJECT_API AUnit : public ACharacter
{
	GENERATED_BODY()

	enum class UnitState
	{
		IDLE,
		MOVING,
		AIMING,
		SHOOTING,
		DEAD
	};

	enum Teams
	{
		RED,
		BLUE
	};

public:
	// Sets default values for this character's properties
	AUnit();

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	AActor* target = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* laserPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* unitSphere;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UMaterial* blueUnitMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	UMaterial* redUnitMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* unitMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* laserBeam;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float fireRate = 2.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float laserDuration = .1f;

	UFUNCTION(BlueprintCallable)
	void GetTarget();

	void Init(int team);

	TArray<AActor*>			FoundActors;
	AUnitAIController*		unitAIController = nullptr;
	UnitState				state = UnitState::IDLE;
	int						unitTeam = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void Idle();
	void Moving();
	void Aiming();
	void Shooting();
	void CheckTargets();

	FVector				fireTarget;
	float				fireTimer = .0f;
	float				laserTimer = .0f;
	int					damage = 10;
	int					life = 30;
	bool				bHasTarget = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
