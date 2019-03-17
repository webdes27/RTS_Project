// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/UnitAIController.h"

void AUnitAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	arrived = true;
}
