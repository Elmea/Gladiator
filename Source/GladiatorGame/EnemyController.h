// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GLADIATORGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()

	UBehaviorTreeComponent* BehaviorComp;
	UBlackboardComponent* BlackboardComp;
	
	//BlackboardKeys
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PlayerKey;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName IsAttackingKey;


	virtual void OnPossess(APawn* pawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;


public:
	APawn* player;
	FVector IdleLocation;

	bool isAttacking=false;
	float angle;
	AEnemyController();

	void Death();



	FORCEINLINE APawn* GetPlayer() const { return player; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComp; }

};
