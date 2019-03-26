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

UCLASS()
class RTSPROJECT_API AUnit : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	AUnit();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBehaviorTree*			BTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextRenderComponent*			stateText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent*				laserPoint;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
		UMaterial*						blueUnitMaterial;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
		UMaterial*						redUnitMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent*		laserBeam;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float							fireRate = 2.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float							laserDuration = .1f;

	void Init(int team, AHomeBase* base);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent* unitMesh;
	TArray<AActor*>			FoundActors; //TODO: Remove this | Handle from base
	AUnitAIController*		unitAIController = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnUnitSeen(APawn* pawn);

	AHomeBase*						homeBase = nullptr;
	int								unitTeam;
	class UPawnSensingComponent*	sensingComponent;




	
};
