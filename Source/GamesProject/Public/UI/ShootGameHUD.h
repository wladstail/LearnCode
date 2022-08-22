// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAMESPROJECT_API AShootGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;	//она вызывает каждый кадр фигуры, текст, линии. ќна виртуальна€

protected:
	//тут хранитьс€ класс PlayerHUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	//в нем будет создаватьс€ наш PlayerHUDWidget
	virtual void BeginPlay() override;

private:
	//прицел
	void DrawCrossHair();
};
