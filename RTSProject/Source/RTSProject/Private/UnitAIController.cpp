// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/UnitAIController.h"
#include "../Public/Unit.h"


void AUnitAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{	
	if (owner != nullptr && Result == EPathFollowingResult::Success)
	{
		owner->bArrived = true;
	}
}

AUnitAIController::AUnitAIController()
{
	bAllowStrafe = true;	
}