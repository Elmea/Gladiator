// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetTargetPosition.h"

EBTNodeResult::Type UBTT_SetTargetPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	
		FVector idlePos = AIController->IdleLocation;

		BlackboardComp->SetValueAsVector("IdleLocation", idlePos);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;

}