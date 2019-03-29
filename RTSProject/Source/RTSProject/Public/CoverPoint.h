// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "CoverPoint.generated.h"

class UTextRenderComponent;

UCLASS()
class RTSPROJECT_API ACoverPoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	ACoverPoint();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextRenderComponent*			stateText;

	int									redOnSight = 0;
	int									blueOnSight = 0;
	AActor*								user = nullptr;

private:

	float								timer = 0.f;
};
