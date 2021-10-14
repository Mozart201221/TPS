// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/TPSBaseWidget.h"
#include "TPSCoreTypes.h"
#include "TPSMenuUserWidget.generated.h"

class UButton;
class UHorizontalBox;
class UTPSGameInstance;
class UTPSLevelItemWidget;
class USoundCue;

UCLASS()
class TPS_API UTPSMenuUserWidget : public UTPSBaseWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemsBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HideAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* StartGameSound;

	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	UPROPERTY()
	TArray<UTPSLevelItemWidget*> LevelItemWidgets;

	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();

	void InitLevelItems();
	void OnLevelSelected(const FLevelData& Data);
	UTPSGameInstance* GetTPSGameInstance() const;
};
