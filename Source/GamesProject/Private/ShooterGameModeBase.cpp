// Shoot Game


#include "ShooterGameModeBase.h"
#include "Player/ShootBaseCharacter.h"			//добавили путь до персонажа 
#include "Player/ShootPlayerController.h"		//путь до контроллера
#include "UI/ShootGameHUD.h"				//прицел

AShooterGameModeBase::AShooterGameModeBase()
{

	DefaultPawnClass = AShootBaseCharacter::StaticClass();					//по умолчанию класс - класс персонажа, а он наследовался от павна
	PlayerControllerClass = AShootPlayerController::StaticClass();			//по умолчанию класс - контроллера
	HUDClass = AShootGameHUD::StaticClass();								//интерфейс поумолчанию

}
