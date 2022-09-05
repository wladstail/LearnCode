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
	virtual void DrawHUD() override;	//��� �������� ������ ���� ������, �����, �����. ��� �����������

protected:
	//��� ��������� ����� PlayerHUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	//� ��� ����� ����������� ��� PlayerHUDWidget
	virtual void BeginPlay() override;

private:
	//������
	void DrawCrossHair();
};
