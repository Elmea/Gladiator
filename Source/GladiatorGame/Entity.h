// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.generated.h"

UCLASS()
class GLADIATORGAME_API AEntity : public ACharacter
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Ragdoll")
	virtual void ActivateRagDoll();

public:
	// Sets default values for this character's properties
	AEntity();	

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PostAttack() {};

	UFUNCTION(BlueprintCallable)
	virtual void ActivateDamageSpot() {};

	UFUNCTION(BlueprintCallable)
	virtual void DeactivateDamageSpot() {};
};
