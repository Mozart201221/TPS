// TPS. All Rights Reserved

#include "Player/TPSCharacter.h"
#include "Components/TPSCharacterMovementComponent.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ATPSCharacter::ATPSCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UTPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	HealthComponent = CreateDefaultSubobject<UTPSHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UTPSWeaponComponent>("WeaponComponent");
}

void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(GetCharacterMovement());


	OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
	HealthComponent->OnDeath.AddUObject(this, &ATPSCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATPSCharacter::OnHealthChanged);
}


void ATPSCharacter::OnHealthChanged(float Health, float HealthDelta)
{
	
}

void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

bool ATPSCharacter::IsRunning() const
{
	return false;
}

void ATPSCharacter::SetPlayerColor(const FLinearColor& Color)
{
	const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	if (!MaterialInst) return;

	MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}

void ATPSCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

	// PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ATPSCharacter::TurnOff()
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);
	Super::TurnOff();
}

void ATPSCharacter::Reset()
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);
	Super::Reset();
}
