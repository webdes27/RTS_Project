// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include <list>
#include "CoverPoint.generated.h"

class UTextRenderComponent;

UCLASS()
class RTSPROJECT_API ACoverPoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	ACoverPoint();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent*				shootPoint;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	int									redOnSight = 0;
	int									blueOnSight = 0;
	AActor*								user = nullptr;
	std::list<AActor*>					targetsOnSight;

private:

	float								timer = 0.f;

};
