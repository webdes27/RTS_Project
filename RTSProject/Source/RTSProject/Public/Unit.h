// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "UnitAIController.h"
#include <unordered_set>
#include "Unit.generated.h"

#define RED_TEAM 0
#define BLUE_TEAM 1

UCLASS()
class RTSPROJECT_API AUnit : public ACharacter
{
	GENERATED_BODY()

public:
	enum class UnitState
	{
		IDLE,
		MOVE,
		CHECK_ENEMIES,
		AIM,
		SHOOT,
		AIM_BASE,
		ATTACKING_BASE,
		DEAD
	};

	enum class UnitGoal
	{
		ATTACK_BASE
	};

	// Sets default values for this character's properties
	AUnit();

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

	TArray<AActor*>			FoundActors; //TODO: Remove this | Handle from base
	AUnitAIController*		unitAIController = nullptr;
	UnitState				state = UnitState::IDLE;
	UnitGoal				goal = UnitGoal::ATTACK_BASE;
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
	inline void ResetShooting();
	void CheckEnemies();	//TODO: Move this to homebase to don't perform all this checks per unit each frame
	void CheckGoal();	//TODO: Move this to homebase to don't perform all this checks per unit each frame
	void Aiming(AActor* target);
	void Shooting();

	AActor*				targetDestination = nullptr;
	AUnit*				enemy = nullptr;
	float				fireTimer = .0f;
	float				laserTimer = .0f;
	int					damage = 10;
	int					life = 30;
	int					framesToDestroy = 50;
	bool				bHasTarget = false;
	bool				bUnderAttack = false;
	bool				bAttackingBase = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
