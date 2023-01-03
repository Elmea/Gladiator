// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "EnemyController.h"
#include "EnemyWidgetComp.h"
#include "PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy.generated.h"

UCLASS(Blueprintable)
class GLADIATORGAME_API AEnemy : public AEntity
{
	GENERATED_BODY()

private:


	UPROPERTY(EditAnywhere)
	class UEnemyWidgetComp* WidgetComp;

	UPROPERTY(EditAnywhere, Category = "Shield")
	int ShieldingAngle = 130;

	bool canTakeDamages;
	float damagesTimer;
	bool attackDamageSpot;

	void UpdateDamagesTimer();
	void Death();
	UPlayerAnimInstance* animInstance;

	class USkeletalMeshComponent* DwarfMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* HammerCollider;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* HammerMesh;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* ShieldMesh;

	UPROPERTY(EditAnywhere)
	class UMaterial* HitMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* BaseMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* EquipementMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* EquipementHittedMaterial;

	UFUNCTION()
	void WarhammerHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Delegate signature
	//DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHealthChangedSignature, int32, NewHP)
	// Function signature
//	void OnHealthChanged(int32 NewHP);


public:
	/*
	UPROPERTY(CPF_BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChangedDelegate;
	*/

	UPROPERTY(EditAnywhere, Category = "Life")
		float maxHp = 3;

	float hp = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InvincibilityTime;
	
	void ActivateDamageSpot() override;

	void DeactivateDamageSpot() override;

	AEnemyController* enemyController;

	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	bool isAlive = true;

	FVector IdleLocation;
	bool isAttacking = false;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Life")
	void TakeDamage(int damage);

	UFUNCTION(BlueprintCallable)
	void Attack();

	void PostAttack() override;


	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviourTree;

};
