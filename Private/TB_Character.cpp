// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Character.h"
#include "TB_GameState.h"
#include "TB_Weapon.h"

#include "AIController.h"
#include "AI/Navigation/NavigationPath.h"

#include <algorithm>

ATB_Character::ATB_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = AAIController::StaticClass();
	WeaponHandlingAnimations.AddZeroed((int32)ETB_WeaponAnimType::VE_Length);

	/* set up the overhead spring arm, the camera is spawned in BeginPlay()  */
	OverheadSpringArm = ObjectInitializer.CreateAbstractDefaultSubobject<USpringArmComponent>(this, TEXT("OverheadSpringArm"));
	OverheadSpringArm->AttachParent = Mesh;
	/* Somewhat sane defaults */
	OverheadSpringArm->RelativeLocation.Z = 140;
	OverheadSpringArm->RelativeRotation.Yaw = 90;
	OverheadSpringArm->RelativeRotation.Pitch = -30;
	OverheadSpringArm->TargetArmLength = 1000;
	OverheadSpringArm->CameraLagSpeed = 10;
	OverheadSpringArm->CameraRotationLagSpeed = 10;

	/* det up the first person spring arm, the camera is spawned in BeginPlay() */
	FPSpringArm = ObjectInitializer.CreateAbstractDefaultSubobject<USpringArmComponent>(this, TEXT("FPSpringArm"));
	FPSpringArm->AttachParent = Mesh;
	/* Somewhat sane defaults */
	FPSpringArm->RelativeLocation.Z = 170;
	FPSpringArm->RelativeRotation.Yaw = -90;
	FPSpringArm->TargetArmLength = 75;
	FPSpringArm->CameraLagSpeed = 10;
	FPSpringArm->CameraRotationLagSpeed = 10;
}

void ATB_Character::BeginPlay()
{
	ACharacter::BeginPlay();

	/* Use the actor name if no other is given */
	if (CharacterName.IsEmpty())
	{
		CharacterName = FText::FromString(GetName());
	}
	/* Use blueprint class name if no other is given */
	if (SpeciesName.IsEmpty())
	{
		SpeciesName = FText::FromString(StaticClass()->GetName());
	}

	/* Join a team */
	auto *World = GetWorld();
	auto *GameState = (ATB_GameState*) World->GetGameState();
	TeamController = GameState->GetTeamController(TeamName);
	if (!TeamController)
	{
		/* Create team controller if this is the first actor on this team team*/
		TeamController = World->SpawnActor<ATB_TeamController>(FActorSpawnParameters());
		TeamController->TeamName = TeamName;
		GameState->RegisterTeamController(TeamController);
	}
	TeamController->RegisterCharacter(this);


	/* Equip weapon */
	if (WeaponClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoCollisionFail = true;
		SpawnParams.Owner = this;
		Weapon = GetWorld()->SpawnActor<ATB_Weapon>(WeaponClass, SpawnParams);
		Weapon->Equip(this);
	}

	/* Set up the actor components that we use as cameras */
	OverheadCamera = World->SpawnActor<ACameraActor>(FActorSpawnParameters());
	OverheadCamera->AttachRootComponentTo(OverheadSpringArm);
	OverheadCamera->CameraComponent->bConstrainAspectRatio = false;
	FPCamera = World->SpawnActor<ACameraActor>(FActorSpawnParameters());
	FPCamera->AttachRootComponentTo(FPSpringArm);
	FPCamera->CameraComponent->bConstrainAspectRatio = false;
	FPCamera->CameraComponent->AddRelativeRotation(FRotator(0, 180, 0));
}


void ATB_Character::PrepareForNextTurn_Implementation()
{
	ActionPoints = MaxActionPoints;
}

void ATB_Character::PrepareForThisTurn_Implementation()
{
	;
}

bool ATB_Character::IsBusy_Implementation()
{
	auto Vel = GetVelocity();
	return Busy || Vel.Size() > 0;
}

void ATB_Character::SetBusy_Implementation(float BusyDuration)
{
	Busy = true;
	GetWorldTimerManager().SetTimer(this, &ATB_Character::ClearBusy, BusyDuration, false);
}

void ATB_Character::ClearBusy_Implementation()
{
	Busy = false;
}

void ATB_Character::Reload_Implementation()
{
	/* Do we have enough APs and a weapon? */
	if (ActionPoints <= 0 || Weapon == NULL)
	{
		return;
	}

	/* figure out how much time we will spend on this */
	float ReloadTime = PlayAnimation(WeaponHandlingAnimations[(int32) Weapon->AnimType].Reload);
	float WeaponReloadTime = Weapon->Reload();
	SetBusy(std::max(ReloadTime, WeaponReloadTime));
	
	ActionPoints--;
}

float ATB_Character::PlayAnimation(UAnimSequence *Animation) {
	if (Mesh && Animation)
	{
		UAnimInstance *AnimInstance = Mesh->GetAnimInstance();
		AnimInstance->PlaySlotAnimation(Animation, FName("DefaultSlot"));

		return AnimInstance->GetAnimAssetPlayerLength(Animation);
	}
	return 0;
}

bool ATB_Character::CanMoveTo_Implementation(FVector Destination)
{
	auto *World = GetWorld();
	auto *NavigationSystem = World->GetNavigationSystem();
	auto *Path = NavigationSystem->FindPathToLocationSynchronously(World, GetActorLocation(), Destination);
	auto Length = Path->GetPathLength();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("Path length: ") += FString::SanitizeFloat(Length / 100) += FString("m"));

	return Length >= 0 && Length <= Movement;
}

void ATB_Character::MoveTo_Implementation(FVector Destination)
{
	if (ActionPoints <= 0)
	{
		return;
	}
	ActionPoints--;

	auto *aic = (AAIController*) Controller;
	aic->MoveToLocation(Destination, 0.05, false);
}

void ATB_Character::LookAt_Implementation(AActor *Target)
{
	auto TargetLoc = Target->GetTransform().GetLocation();
	auto MyLoc = GetTransform().GetLocation();

	/* 
	FIXME
	This should be replaced by a mechanism that gradually turns the actor 
	*/
	auto Rotator = (TargetLoc - MyLoc).Rotation();
	auto Transform = GetTransform();
	Transform.SetRotation(Rotator.Quaternion());
	SetActorTransform(Transform);
}

void ATB_Character::TargetNextEnemy_Implementation()
{
	TArray<ATB_Character*> KnownEnemies;
	int32 Idx;
	TeamController->GetKnownEnemies(KnownEnemies);
	KnownEnemies.Find(EnemyTarget, Idx);
	
	if (KnownEnemies.Num())
	{
		Idx++;
		Idx = Idx % KnownEnemies.Num();
		EnemyTarget = KnownEnemies[Idx];

		if (EnemyTarget)
		{
			LookAt(EnemyTarget);
		}
	}
}

int32 ATB_Character::HitChance_Implementation()
{
	int32 ToHit = 0;

	if (Weapon && EnemyTarget)
	{
		float Range = GetDistanceTo(EnemyTarget);
		
		ToHit = RangedAttackSkill;
		ToHit += Weapon->HitModifier(Range);
	}

	return ToHit;
}

int32 ATB_Character::Damage_Implementation()
{
	int32 Damage = 0;

	if (Weapon && EnemyTarget)
	{
		float Range = GetDistanceTo(EnemyTarget);

		Damage = Weapon->BaseDamage;
		Damage += Weapon->DamageModifier(Range);
	}

	return Damage;
}