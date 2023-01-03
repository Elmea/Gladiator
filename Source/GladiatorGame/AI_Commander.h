// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "AI_Commander.generated.h"

UCLASS()
class GLADIATORGAME_API AAI_Commander : public AActor
{
	GENERATED_BODY()

	TArray<AActor*> EnemyActors;
	APawn* player;
	TArray<AEnemy*> Enemies;
	TArray<AEnemy*> deadEnemiesArray;

	int AttackingEnemyCount = 0;

public:	
	// Sets default values for this actor's properties
	AAI_Commander();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float distance = 350;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int maxSimultaneousAttacks = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float beginingTime = 3.f;

	float beginingTimer = 0.f;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetEnemiesIdleLocation();

};	

