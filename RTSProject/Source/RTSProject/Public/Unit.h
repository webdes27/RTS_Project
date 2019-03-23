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
		MOVE,
		CHECK_ENEMIES,
		AIM,
		SHOOT,
		DEAD
	};

	enum class Teams
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
		UTextRenderComponent* stateText;

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

	void Init(int team);

	TArray<AActor*>			FoundActors;
	AUnitAIController*		unitAIController = nullptr;
	UnitState				state = UnitState::IDLE;
	int						unitTeam = 0;
	bool					bArrived = true;
	bool					bAbortedPath = false;
	AUnit*					attacker = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit);

	inline bool IsUnderAttack();
	inline void GetDestination();
	inline bool HasArrived() const;
	void CheckEnemies();	
	void Aiming();
	void Shooting();

	AUnit*				enemy;
	float				fireTimer = .0f;
	float				laserTimer = .0f;
	int					damage = 10;
	int					life = 30;
	int					framesToDestroy = 50;
	bool				bHasTarget = false;
	bool				bUnderAttack = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
