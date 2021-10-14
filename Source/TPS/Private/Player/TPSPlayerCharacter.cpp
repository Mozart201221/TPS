// TPS. All Rights Reserved


#include "Player/TPSPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ATPSPlayerCharacter::ATPSPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CameraCollisionComponent"));
	CameraCollisionComponent->SetupAttachment(FollowCamera);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ATPSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraCollisionComponent);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATPSPlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATPSPlayerCharacter::OnCameraCollisionEndOverlap);
}

void ATPSPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

void ATPSPlayerCharacter::OnCameraCollisionEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

void ATPSPlayerCharacter::CheckCameraOverlap()
{
	const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(HideMesh);

	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);

	for (auto MeshChild : MeshChildren)
	{
		const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
		if (MeshChildGeometry)
		{
			MeshChildGeometry->SetOwnerNoSee(HideMesh);
		}
	}

}

void ATPSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ATPSPlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ATPSPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATPSPlayerCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ATPSPlayerCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UTPSWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UTPSWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UTPSWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UTPSWeaponComponent::Reload);

	DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
	PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, WeaponComponent, &UTPSWeaponComponent::Zoom, true);
	PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, WeaponComponent, &UTPSWeaponComponent::Zoom, false);
}

void ATPSPlayerCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ATPSPlayerCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
}

void ATPSPlayerCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ATPSPlayerCharacter::OnStopRunning()
{
	WantsToRun = false;
}


bool ATPSPlayerCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

void ATPSPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

