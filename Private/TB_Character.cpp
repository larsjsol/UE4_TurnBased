// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Character.h"
#include "TB_GameState.h"
#include "TB_Weapon.h"
#include "TB_AnimInstance.h"
#include "TB_Name.h"
#include "TB_AimComponent.h"
#include "TB_AIController.h"

#include "AIController.h"
#include "AI/Navigation/NavigationPath.h"

#include <algorithm>


ATB_Character::ATB_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = ATB_AIController::StaticClass();
	NameClass = UTB_Name::StaticClass();

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
	FPSpringArm->TargetArmLength = 25;
	FPSpringArm->CameraLagSpeed = 10;
	FPSpringArm->CameraRotationLagSpeed = 10;

	AimComponent = ObjectInitializer.CreateAbstractDefaultSubobject<UTB_AimComponent>(this, TEXT("Aim Component"));
	AimComponent->AttachParent = FPSpringArm;
}

void ATB_Character::BeginPlay()
{
	ACharacter::BeginPlay();

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

	/* Set up the actor components that we use as cameras */
	OverheadCamera = World->SpawnActor<ACameraActor>(FActorSpawnParameters());
	OverheadCamera->AttachRootComponentTo(OverheadSpringArm);
	OverheadCamera->CameraComponent->bConstrainAspectRatio = false;
	FPCamera = World->SpawnActor<ACameraActor>(FActorSpawnParameters());
	FPCamera->AttachRootComponentTo(FPSpringArm);
	FPCamera->CameraComponent->bConstrainAspectRatio = false;
	FPCamera->CameraComponent->AddRelativeRotation(FRotator(0, 180, 0));

	AnimInstance = (UTB_AnimInstance*)GetMesh()->GetAnimInstance();

	/* Equip weapon */
	if (WeaponClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoCollisionFail = true;
		SpawnParams.Owner = this;
		Weapon = GetWorld()->SpawnActor<ATB_Weapon>(WeaponClass, SpawnParams);
		Weapon->Equip(this);
	}
}

void ATB_Character::PostInitProperties()
{
	Super::PostInitProperties();

	/* Use blueprint class name if no other is given */
	if (SpeciesName.IsEmpty())
	{
		SpeciesName = FText::FromString(StaticClass()->GetName());
	}

	if (NameClass)
	{
		CharacterName = ConstructObject<UTB_Name>(NameClass, this);
		CharacterName->Generate(Gender);
	}
	else
	{
		/* fall back to a simple default name (the human, the robot, etc.) */
		CharacterName = ConstructObject<UTB_Name>(UTB_Name::StaticClass(), this);
		CharacterName->FirstName = FString::Printf(TEXT("the %s"), *SpeciesName.ToString().ToLower());
	}
}

float ATB_Character::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// add an entry to the log
	auto *GameState = (ATB_GameState *)GetWorld()->GetGameState();

	HitPoints -= Damage;
	//play the hit animation if we are not dead (death animations are handled in the animgraph)
	if (HitPoints > 0)
	{
		float Time = AnimInstance->PlayAnimation(AnimInstance->GetHurtAnim());
	}

	// add an entry to the log
	GameState->GameLog->Log(ETB_LogCategory::VE_Violence, FString::Printf(TEXT("%s hits %s"),
		*((ATB_Character *)DamageCauser)->CharacterName->ToString(),
		*CharacterName->ToString()));
	// ...and another one if we are dead
	if (HitPoints <= 0)
	{
		GameState->GameLog->Log(ETB_LogCategory::VE_Violence, FString::Printf(TEXT("%s is killed!"), *CharacterName->ToString()));
	}

	return Damage;
}

void ATB_Character::OnBeginTurn_Implementation()
{
}

void ATB_Character::OnEndTurn_Implementation()
{
	ActionPoints = MaxActionPoints;
}

void ATB_Character::OnActivation_Implementation()
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

void ATB_Character::ClearBusy()
{
	Busy = false;

	// prod the character to do the next action
	PlayTurn();
}

void ATB_Character::Reload_Implementation()
{
	/* Do we have enough APs and a weapon? */
	if (ActionPoints <= 0 || Weapon == NULL)
	{
		return;
	}

	/* start the animations and figure out how long until they are done */
	float ReloadTime = AnimInstance->PlayAnimation(AnimInstance->GetReloadAnim());
	float WeaponReloadTime = Weapon->Reload();
	SetBusy(std::max(ReloadTime, WeaponReloadTime));
	
	ActionPoints--;
}

void ATB_Character::InitiateAttack_Implementation()
{
	if (Weapon && AimComponent->EnemyTarget && ActionPoints > 0)
	{
		float AttackTime = AnimInstance->PlayAnimation(AnimInstance->GetAttackAnim());
		float WeaponAttackTime = Weapon->Attack();
		SetBusy(std::max(AttackTime, WeaponAttackTime));
	}
}

void ATB_Character::Attack_Implementation()
{
	if (Weapon && AimComponent->EnemyTarget && ActionPoints > 0)
	{
		if (AimComponent->FinalHitChance > FGenericPlatformMath::Rand() % 100)
		{
			AimComponent->EnemyTarget->TakeDamage(AimComponent->FinalDamage, FDamageEvent(), TeamController, this);
		}
		ActionPoints--;
	}
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
	
	ClearBusy();
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

void ATB_Character::TargetEnemy_Implementation()
{
	AimComponent->UpdateVisibleEnemies();
	if (!AimComponent->EnemyTarget)
	{
		TargetNextEnemy();
	}
	else
	{
		AimComponent->UpdateValues();
	}
}

void ATB_Character::TargetNextEnemy_Implementation()
{
	AimComponent->TargetNextEnemy();
	if (AimComponent->EnemyTarget)
	{
		LookAt(AimComponent->EnemyTarget);
	}
	AimComponent->UpdateValues();
}

void ATB_Character::GetHitLocations_Implementation(TArray<FVector> &WorldLocations)
{
	WorldLocations.Empty();

	TArray<FName> SocketNames = Mesh->GetAllSocketNames();
	for (auto Name : SocketNames)
	{
		FVector Location;
		FRotator Rotation;
		
		Mesh->GetSocketWorldLocationAndRotation(Name, Location, Rotation);
		WorldLocations.Add(Location);
	}
}

void ATB_Character::PlayTurn_Implementation()
{
	ATB_AIController *Controller = (ATB_AIController *) GetController();

	if (ActionPoints > 0)
	{
		if (!HumanControlled)
		{
			Controller->DoSingleAction();
		}
	}
	else
	{
		TeamController->CharacterPlayTurn();
	}
}

