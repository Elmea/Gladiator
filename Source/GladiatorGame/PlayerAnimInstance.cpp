 // Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateSpeed()
{
	APawn* OwningPawn = TryGetPawnOwner();
	if (!OwningPawn)
		return;

	speed = OwningPawn->GetVelocity().Size();
}

UPlayerAnimInstance::UPlayerAnimInstance()
{
	speed = 0;
	playAttack = false;
	playShield = false;
	playDeath = false;
}

void UPlayerAnimInstance::PlayAttack()
{
	playAttack = true;
}

void UPlayerAnimInstance::PlayShield()
{
	playShield = true;
}

void UPlayerAnimInstance::ReleaseShield()
{
	playShield = false;
}

void UPlayerAnimInstance::PlayDeath()
{
	playDeath = true;
}

void UPlayerAnimInstance::StopAttack()
{
	playAttack = false;
}

