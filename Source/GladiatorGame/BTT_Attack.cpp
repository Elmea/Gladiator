// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* enemyController = Cast<AAIController>(OwnerComp.GetOwner());
	
	if (enemyController)
	{
		AEnemy* enemy = Cast<AEnemy>(enemyController->GetInstigator());
		if(enemy)
		{
			enemy->Attack();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}