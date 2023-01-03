// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyHealthBar.h"
#include "Components/WidgetComponent.h"
#include "EnemyWidgetComp.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UEnemyWidgetComp : public UWidgetComponent
{
	GENERATED_BODY()

public:
	class UEnemyHealthBar* HealthBar;

	void BeginPlay() override;
};
