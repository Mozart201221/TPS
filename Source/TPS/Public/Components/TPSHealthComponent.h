// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSCoreTypes.h"
#include "TPSHealthComponent.generated.h"

class UPhysicalMaterial;
class UCameraShakeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UTPSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSHealthComponent();

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return FMath::IsNearlyZero(CurHealth); }


	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const { return CurHealth / MaxHealth; }

	float GetHealth() const { return CurHealth; }

	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealModifier = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		TMap<UPhysicalMaterial*, float> DamageModifiers;

private:
	float CurHealth = 0.0f;
	FTimerHandle HealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamageHandle(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
		void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, 
			class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	
	UFUNCTION()
		void OnTakeRadialDamage(
			AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
			FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser );

	void HealUpdate();
	void SetHealth(float NewHealth);
	void PlayCameraShake();

	void Killed(AController* KillerController);
	void ApplyDamage(float Damage, AController* InstigatedBy);
	float GetPointDamageModifier(AActor* DamageActor, const FName& BoneName);

	void ReportDamageEvent(float Damage, AController* InstigatedBy);
};