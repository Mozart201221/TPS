// TPS. All Rights Reserved


#include "AI/TPSAICharacter.h"
#include "AI/TPSAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TPSAIWeaponComponent.h"
#include "BrainComponent.h"

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
