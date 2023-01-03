// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "BTT_SetTargetPosition.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GLADIATORGAME_API UBTT_SetTargetPosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
