// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "Enemy.h"
#include "GladiatorGameCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AEnemyController::AEnemyController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT_Enemy"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB_Enemy"));

	PlayerKey = "Player";
	IsAttackingKey = "IsAttacking";
}

void AEnemyController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	AEnemy* Enemy = Cast<AEnemy>(pawn);
	
	if (Enemy)
	{
		if (Enemy->BehaviourTree)
		{
			if (Enemy->BehaviourTree->BlackboardAsset)
			{
				BlackboardComp->InitializeBlackboard(*(Enemy->BehaviourTree->BlackboardAsset));
			}
		}
		BehaviorComp->StartTree(*Enemy->BehaviourTree);
	}
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	BlackboardComp->SetValueAsBool("IsAlive", true);
	player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	BlackboardComp->SetValueAsObject("Player", player);

}

void AEnemyController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	BlackboardComp->SetValueAsBool("IsAttacking", isAttacking);

	FVector location = GetInstigator()->GetActorLocation();
	float distanceToPlayer = (player->GetTransform().GetLocation() - location).Size();
	if (distanceToPlayer < 0)
		distanceToPlayer *= -1;
}

void AEnemyController::Death()
{
	BlackboardComp->SetValueAsBool("IsAlive", false);
}
