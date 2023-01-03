// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/Widget.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */

UCLASS()
class GLADIATORGAME_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void SetProgressBar(UProgressBar* _ProgressBar) { ProgressBar = _ProgressBar; };

public:
	UPROPERTY(EditAnywhere)
	class UProgressBar* ProgressBar;
};
