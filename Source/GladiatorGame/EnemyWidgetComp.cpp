// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWidgetComp.h"

void UEnemyWidgetComp::BeginPlay()
{
	Super::BeginPlay();

	HealthBar = Cast<UEnemyHealthBar>(GetWidget());
}