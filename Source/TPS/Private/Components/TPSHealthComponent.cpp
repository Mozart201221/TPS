// TPS. All Rights Reserved


#include "Components/TPSHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShake.h"
#include "TPSGameMode.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UTPSHealthComponent::UTPSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UTPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();  //TODO check
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UTPSHealthComponent::OnTakeAnyDamageHandle); 
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &UTPSHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UTPSHealthComponent::OnTakeRadialDamage);
	}
}

void UTPSHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
	UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, final damage: %f, bone: %s"), Damage, FinalDamage,*BoneName.ToString());
	ApplyDamage(FinalDamage, InstigatedBy);
}

void UTPSHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
	FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f"), Damage);
	ApplyDamage(Damage, InstigatedBy);
}


void UTPSHealthComponent::OnTakeAnyDamageHandle(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthComponent, Display, TEXT("On any damage: %f"), Damage);
}


void UTPSHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;
	SetHealth(CurHealth - Damage);
	OnHealthChanged.Broadcast(CurHealth, 0.0f);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UTPSHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

void UTPSHealthComponent::HealUpdate()
{
	SetHealth(CurHealth + HealModifier);

	if (IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UTPSHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - CurHealth;

	CurHealth = NextHealth;
	OnHealthChanged.Broadcast(CurHealth, HealthDelta);
}

bool UTPSHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull()) return false;

	SetHealth(CurHealth + HealthAmount);
	return true;
}

bool UTPSHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(CurHealth, MaxHealth);
}


void UTPSHealthComponent::PlayCameraShake()
{
	if(IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void UTPSHealthComponent::Killed(AController* KillerController)
{
	if (!GetWorld()) return;

	const auto GameMode = Cast<ATPSGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : NULL;

	GameMode->Killed(KillerController, VictimController);
}

float UTPSHealthComponent::GetPointDamageModifier(AActor* DamageActor, const FName& BoneName)
{
	const auto Character = Cast<ACharacter>(DamageActor);
	if (!Character ||
		!Character->GetMesh() ||
		!Character->GetMesh()->GetBodyInstance(BoneName)) return 1.0f;

	const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if (!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.0f;

	return DamageModifiers[PhysMaterial];
}

void UTPSHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;
	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		GetOwner(), 
		InstigatedBy->GetPawn(), 
		Damage, 
		InstigatedBy->GetPawn()->GetActorLocation(), 
		GetOwner()->GetActorLocation());
}
