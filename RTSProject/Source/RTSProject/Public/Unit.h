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

class AHomeBase;
class UPawnSensingComponent;
class ACoverPoint;

UCLASS()
class RTSPROJECT_API AUnit : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	AUnit();

	UFUNCTION(BlueprintImplementableEvent, category = "UnitEvents")
		void ShootEvent(FVector hitPoint);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBehaviorTree*					BTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent*				laserPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent*				headPoint;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
		UMaterial*						blueUnitMaterial;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
		UMaterial*						redUnitMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
		UParticleSystem*				blueLaser;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
		UParticleSystem*				redLaser;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent*		laserBeam;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int								unitTeam;


	void Init(int team, AHomeBase* base);
	bool TakeDamage(int damage); //Returns true if dead

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent*			unitMesh;
	AUnitAIController*				unitAIController = nullptr;
	int								life = 50;
	AHomeBase*						homeBase = nullptr;
	ACoverPoint*					coverPoint = nullptr;
	ACoverPoint*					lastCoverPoint = nullptr;
	int								shootAttempts = 2;
	int								shootCounter = shootAttempts;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnUnitSeen(APawn* pawn);

	class UPawnSensingComponent*	sensingComponent;	
	
};
