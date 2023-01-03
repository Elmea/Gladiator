// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class GLADIATORGAME_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UpdateSpeed();

	UPlayerAnimInstance();

	void PlayAttack();
	void StopAttack();
	
	void PlayShield();
	void PlayDeath();
	void ReleaseShield();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool playAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool playShield;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool playDeath;
};