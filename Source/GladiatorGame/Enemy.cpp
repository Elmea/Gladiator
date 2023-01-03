// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemyController.h"
#include "EnemyHealthBar.h"
#include "GladiatorGameCharacter.h"
#include "BehaviorTree/BehaviorTree.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DwarfMesh = GetMesh();

	HammerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hammer"));
	HammerMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("WeaponPoint")));

	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	ShieldMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("DualWeaponPoint")));

	HammerCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HammerCollider"));
	HammerCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	HammerCollider->AttachToComponent(HammerMesh, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("HammerHead")));

	//Create Health Bar
	WidgetComp = CreateDefaultSubobject<UEnemyWidgetComp>(TEXT("HealthBar"));
	WidgetComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	enemyController = Cast<AEnemyController>(GetController());

	animInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	HammerCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::WarhammerHit);

	hp = maxHp;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	enemyController->IdleLocation = this->IdleLocation;
	enemyController->isAttacking = this->isAttacking;

	UpdateDamagesTimer();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::TakeDamage(int damage)
{
	if (canTakeDamages)
	{
		UE_LOG(LogTemp, Display, TEXT("Ouille"));

		hp -= damage;
		canTakeDamages = false;
		damagesTimer = InvincibilityTime;

		DwarfMesh->SetMaterial(0, HitMaterial);
		DwarfMesh->SetMaterial(1, EquipementHittedMaterial);
	}

	animInstance->StopAttack();
	isAttacking = false;

	if (WidgetComp)
	{
		if (WidgetComp->HealthBar)
		{
			UE_LOG(LogTemp, Display, TEXT("Set percent"));
			WidgetComp->HealthBar->ProgressBar->SetPercent(hp / maxHp);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Cast error"));
	}
		
}

void AEnemy::UpdateDamagesTimer()
{
	if (!canTakeDamages && isAlive)
	{
		damagesTimer -= FApp::GetDeltaTime();

		if (damagesTimer < 0.f)
		{
			canTakeDamages = true;

			DwarfMesh->SetMaterial(0, BaseMaterial);
			DwarfMesh->SetMaterial(1, EquipementMaterial);
		}

		if (hp <= 0)
			Death();
	}
}

void AEnemy::Death()
{
	UE_LOG(LogTemp, Display, TEXT("La vie quitte mon corps, oh tragedie et desespoir, je me meurs"));

	hp = 0;
	isAlive = false;

	DwarfMesh->SetMaterial(0, BaseMaterial);
	DwarfMesh->SetMaterial(1, EquipementMaterial);

	if (animInstance)
		animInstance->PlayDeath();

	isAttacking = false;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Cast<AEnemyController>(GetOwner())->Death();

	if (WidgetComp)
		WidgetComp->DestroyComponent();
}

void AEnemy::Attack()
{
	if (isAttacking)
	{
		if (animInstance)
			animInstance->PlayAttack();
	}
}

void AEnemy::PostAttack()
{
	if (animInstance)
		animInstance->StopAttack();

	isAttacking = false;
}

void AEnemy::ActivateDamageSpot()
{
	attackDamageSpot = true;
}

void AEnemy::DeactivateDamageSpot()
{
	attackDamageSpot = false;
}

void AEnemy::WarhammerHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (attackDamageSpot && isAlive)
	{
		AGladiatorGameCharacter* player = Cast<AGladiatorGameCharacter>(OtherActor);
		if (!player)
			return;
			
		if (player->IsShielding())
		{
			FVector2D forward { player->GetActorForwardVector().X, player->GetActorForwardVector().Y };

			// Using this strategy because sweep result seems to always be 0
			FVector normal = GetActorLocation() - player->GetActorLocation();
			FVector2D normal2 {normal.X, normal.Y};
			normal2.Normalize();

			if (FVector2D::DotProduct(forward, normal2) > 0.4f)
			{
				UE_LOG(LogTemp, Display, TEXT("Heee ! Le bouclier c'est de la triche !"));
				return;
			}
		}

		UE_LOG(LogTemp, Display, TEXT("Hehehe bonk, je touche mon pote"));
		player->GetHit();
	}
}

