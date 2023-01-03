// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "PlayerAnimInstance.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance* animInstance;
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


};
