// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"

#include "GameFramework/Character.h"
#include "PlayerAnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "GladiatorGameCharacter.generated.h"

UCLASS(config=Game)
class AGladiatorGameCharacter : public AEntity
{
private:
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* HammerCollider;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* HammerMesh;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* HealthBar;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* ShieldMesh;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ShieldParticle;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* DashParticle;

	class USkeletalMeshComponent* DwarfMesh;


	bool isShielding = false;
	bool canMove = true;
	bool isAttacking = false;
	bool isAlive = true;
	bool attackDamageSpot = false; 
	bool shieldBuffer = false;

	UPlayerAnimInstance* animInstance;

	void Attack();
	void Shield();
	void StopShield();
	void Death();
	void Dash();

	UFUNCTION()
	void WarhammerHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	class UMaterial* HitMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterial* BaseMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* EquipementMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* EquipementHittedMaterial;

	bool canTakeDamages;
	float damagesTimer;

	void UpdateDamagesTimer();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InvincibilityTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShieldRescale = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DashStrenght = 2500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DashCooldown = 1.f;

	float DashTimer = 5.f;

	AGladiatorGameCharacter();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void EnableMove();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void DisableMove();

	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsShielding() { return isShielding; }

	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsAttacking() { return isAttacking; }

	void PostAttack() override;

	void ActivateDamageSpot() override;

	void DeactivateDamageSpot() override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void GetHit();

	void QuitGame();

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "State")
	FORCEINLINE bool IsAlive() { return isAlive; }

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
 
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void Tick(float DeltaSeconds) override;

protected:

	///** Resets HMD orientation in VR. */
	void OnResetVR();

	///** Called for forwards/backward input */
	void MoveForward(float Value);

	///** Called for side to side input */
	void MoveRight(float Value);

	///** 
	// * Called via input to turn at a given rate. 
	// * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	// */
	void TurnAtRate(float Rate);

	///**
	// * Called via input to turn look up/down at a given rate. 
	// * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	// */
	void LookUpAtRate(float Rate);

	///** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	///** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UPROPERTY(BlueprintReadOnly, Category = "Life")
	float life;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Life")
	float maxLife = 5;
};

