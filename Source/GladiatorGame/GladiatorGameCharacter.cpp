// Copyright Epic Games, Inc. All Rights Reserved.

#include "GladiatorGameCharacter.h"
#include "PlayerAnimInstance.h"
#include "Enemy.h"

#include "Kismet/KismetSystemLibrary.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AGladiatorGameCharacter

AGladiatorGameCharacter::AGladiatorGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//Create Health Bar
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create hammer and Shield
	HammerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hammer"));
	HammerMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("WeaponPoint")));

	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	ShieldMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("DualWeaponPoint")));

	HammerCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HammerCollider"));
	HammerCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	HammerCollider->AttachToComponent(HammerMesh, FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("HammerHead")));

	ShieldParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShieldParticle"));
	ShieldParticle->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	DashParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DashParticle"));
	DashParticle->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void AGladiatorGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	animInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	HammerCollider->OnComponentBeginOverlap.AddDynamic(this, &AGladiatorGameCharacter::WarhammerHit);
	DwarfMesh = GetMesh();
	life = maxLife;
	ShieldParticle->Deactivate();
	DashParticle->Deactivate();
}	

////////////////////////////////////////////////////////////////////////////
//// Input

void AGladiatorGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGladiatorGameCharacter::Attack);
	PlayerInputComponent->BindAction("Shield", IE_Pressed, this, &AGladiatorGameCharacter::Shield);
	PlayerInputComponent->BindAction("Shield", IE_Released, this, &AGladiatorGameCharacter::StopShield);
	PlayerInputComponent->BindAction("HitPlayer", IE_Pressed, this, &AGladiatorGameCharacter::GetHit);
	PlayerInputComponent->BindAction("KillPlayer", IE_Pressed, this, &AGladiatorGameCharacter::Death);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AGladiatorGameCharacter::Dash);
	PlayerInputComponent->BindAction("QuitGame", IE_Pressed, this, &AGladiatorGameCharacter::QuitGame);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGladiatorGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGladiatorGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGladiatorGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGladiatorGameCharacter::LookUpAtRate);

	//// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGladiatorGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGladiatorGameCharacter::TouchStopped);

	//// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGladiatorGameCharacter::OnResetVR);
}

void AGladiatorGameCharacter::OnResetVR()
{
	// If GladiatorGame is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in GladiatorGame.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGladiatorGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AGladiatorGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AGladiatorGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGladiatorGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGladiatorGameCharacter::MoveForward(float Value)
{
	if (!isAlive)
		return;

	if ((Controller != nullptr) && (Value != 0.0f) && canMove)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGladiatorGameCharacter::MoveRight(float Value)
{
	if (!isAlive)
		return;

	if ( (Controller != nullptr) && (Value != 0.0f) && canMove)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGladiatorGameCharacter::Attack()
{
	if (!isAlive)
		return;

	if (!isShielding)
	{
		UE_LOG(LogTemp, Display, TEXT("Attack !"));
		DisableMove();

		// Jouer animation 
		if (animInstance)
		{
			isAttacking = true;
			animInstance->PlayAttack();
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Y'a pas !!"));
		}
	}
}

void AGladiatorGameCharacter::EnableMove()
{
	canMove = true;
}

void AGladiatorGameCharacter::DisableMove()
{
	canMove = false;
}

void AGladiatorGameCharacter::Shield()
{
	if (isAttacking)
	{
		shieldBuffer = true;
		return;
	}

	shieldBuffer = false;

	if (!isAlive)
		return;

	UE_LOG(LogTemp, Display, TEXT("Shield !"));
	isShielding = true;
	DisableMove();

	// Démarrer animation 
	ShieldParticle->Activate(true);
	ShieldMesh->SetRelativeScale3D(FVector(ShieldRescale, ShieldRescale, ShieldRescale));
	if (animInstance)
		animInstance->PlayShield();
	else
		UE_LOG(LogTemp, Display, TEXT("Y'a pas !!"));
}

void AGladiatorGameCharacter::StopShield()
{
	if (!isAlive)
		return;

	shieldBuffer = false;

	if (!isAttacking)
	{
		UE_LOG(LogTemp, Display, TEXT("Stop shield !"));
		isShielding = false;
		EnableMove();

		// Stopper animation 
		ShieldParticle->Deactivate();	
		ShieldMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
		if (animInstance)
			animInstance->ReleaseShield();
		else
			UE_LOG(LogTemp, Display, TEXT("Y'a pas !!"));
	}
}

void AGladiatorGameCharacter::WarhammerHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (attackDamageSpot && isAlive)
	{
		AEnemy* enemy = Cast<AEnemy>(OtherActor);
		if (!enemy)
			return;

		enemy->TakeDamage(1);
	}
}

void AGladiatorGameCharacter::PostAttack()
{
	if (!isAlive)
		return;

	if (!isShielding)
	{
		EnableMove();
	}

	if (animInstance)
		animInstance->StopAttack();

	attackDamageSpot = false;
}

void AGladiatorGameCharacter::GetHit()
{
	if (canTakeDamages)
	{
		if (!isAlive)
			return;

		life--;

		DwarfMesh->SetMaterial(0, HitMaterial);
		DwarfMesh->SetMaterial(1, EquipementHittedMaterial);
		canTakeDamages = false;
		damagesTimer = InvincibilityTime;

		if (life <= 0)
			Death();
	}
}

void AGladiatorGameCharacter::Death()
{
	if (!isAlive)
		return;

	life = 0; 
	isAlive = false;
	DisableMove();

	DwarfMesh->SetMaterial(0, BaseMaterial);
	DwarfMesh->SetMaterial(1, EquipementMaterial);

	if (animInstance)
		animInstance->PlayDeath();

	
}

void AGladiatorGameCharacter::ActivateDamageSpot()
{
	attackDamageSpot = true;
}

void AGladiatorGameCharacter::DeactivateDamageSpot()
{
	attackDamageSpot = false;

	// do it now to reduce attack lag
	isAttacking = false;
}

void AGladiatorGameCharacter::UpdateDamagesTimer()
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
	}

	if (life <= 0)
		Death();
}

void AGladiatorGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateDamagesTimer();

	if (shieldBuffer)
		Shield();

	if (DashTimer <= DashCooldown)
		DashTimer += DeltaSeconds;
}

void AGladiatorGameCharacter::Dash()
{
	if (DashTimer > DashCooldown && canMove)
	{
		UE_LOG(LogTemp, Display, TEXT("Daaaaaaaashu !!"));
		FVector forwardV = GetActorForwardVector();
		LaunchCharacter(forwardV * DashStrenght, true, false);
		DashParticle->Activate(true);
		DashTimer = 0;
	}
}

void AGladiatorGameCharacter::QuitGame()
{
	Cast<APlayerController>(GetController())->ConsoleCommand("Quit");
}