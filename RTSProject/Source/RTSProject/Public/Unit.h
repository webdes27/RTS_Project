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
		SHOOTING
	};

public:
	// Sets default values for this character's properties
	AUnit();

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	AActor* target = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* unitSphere;

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

	TArray<AActor*> FoundActors;
	
	AUnitAIController* unitAIController = nullptr;

	UnitState state = UnitState::IDLE;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual void NotifyActorEndOverlap(AActor* Other) override;

	void Idle();
	void Moving();
	void Aiming();
	void Shooting();

	std::unordered_set<AActor*> atRangeTargets;
	FVector fireTarget;
	float fireTimer = .0f;
	float laserTimer = .0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
