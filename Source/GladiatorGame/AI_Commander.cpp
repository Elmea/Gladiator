// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Commander.h"

// Sets default values
AAI_Commander::AAI_Commander()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAI_Commander::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyActors);
	for(int i = 0; i<EnemyActors.Num();i++)
	{
		Enemies.Add(Cast<AEnemy>(EnemyActors[i]));
	}
	player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AAI_Commander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (Enemies.Num() != 0)
	{
		int attackingEnemy = 0;
		AttackingEnemyCount = 0;
	
		SetEnemiesIdleLocation();

		if (beginingTimer < beginingTime)
		{
			beginingTimer += DeltaTime;
			return;
		}

		if (Enemies.Num() > 1)
			attackingEnemy = FMath::RandRange(0, Enemies.Num() - 1);


		if (AttackingEnemyCount < maxSimultaneousAttacks && Enemies.Num()>0)
			Enemies[attackingEnemy]->isAttacking = true;
	}

}

void AAI_Commander::SetEnemiesIdleLocation()
{
	FVector targetLocation = player->GetTransform().GetLocation();
	FVector pointOnTheEdge;
	float angle = 0;

	deadEnemiesArray.Empty();

	for (int i = 0; i < Enemies.Num(); i++)
	{
		if (Enemies[i]->isAlive)
		{
			pointOnTheEdge = FVector(targetLocation.X, targetLocation.Y, targetLocation.Z);
			pointOnTheEdge.X += distance * cos(angle);
			pointOnTheEdge.Y += distance * sin(angle);

			Enemies[i]->IdleLocation = pointOnTheEdge;

			if (Enemies[i]->isAttacking)
				AttackingEnemyCount++;
			angle++;
		}		
		else { deadEnemiesArray.Push(Enemies[i]); }		
	}

	for (int i = 0; i < deadEnemiesArray.Num(); i++)
	{
		Enemies.Remove(deadEnemiesArray[i]);
	}

}

