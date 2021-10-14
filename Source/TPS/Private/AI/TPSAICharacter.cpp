// TPS. All Rights Reserved


#include "AI/TPSAICharacter.h"
#include "AI/TPSAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TPSAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/TPSHealthBarWidget.h"
#include "Components/TPSHealthComponent.h"

ATPSAICharacter::ATPSAICharacter(const FObjectInitializer& ObjInit) 
	: Super(ObjInit.SetDefaultSubobjectClass<UTPSAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ATPSAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ATPSAICharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthWidgetComponent);
}

void ATPSAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHealthWidgetVisibility();
}

void ATPSAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto TPSController = Cast<AAIController>(Controller);
	if (TPSController && TPSController->BrainComponent)
	{
		TPSController->BrainComponent->Cleanup();
	}
}

void ATPSAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
	Super::OnHealthChanged(Health, HealthDelta);

	const auto HealthBarWidget = Cast<UTPSHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (!HealthBarWidget) return;
	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ATPSAICharacter::UpdateHealthWidgetVisibility()
{
	if (!GetWorld() || //
		!GetWorld()->GetFirstPlayerController() || //
		!GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
		return;

	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
	HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}
