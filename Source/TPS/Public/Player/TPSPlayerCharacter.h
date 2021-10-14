// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/TPSCharacter.h"
#include "TPSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class TPS_API ATPSPlayerCharacter : public ATPSCharacter
{
	GENERATED_BODY()
	
public:
	ATPSPlayerCharacter(const FObjectInitializer& ObjInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CameraCollisionComponent;

	virtual void OnDeath() override;
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool IsRunning() const override;

private:
	bool WantsToRun = false;
	bool IsMovingForward = false;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void OnStartRunning();
	void OnStopRunning();

	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCameraCollisionEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckCameraOverlap();
};
