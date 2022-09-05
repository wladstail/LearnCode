// Shoot Game


#include "ShooterGameModeBase.h"
#include "Player/ShootBaseCharacter.h"			//�������� ���� �� ��������� 
#include "Player/ShootPlayerController.h"		//���� �� �����������
#include "UI/ShootGameHUD.h"				//������

AShooterGameModeBase::AShooterGameModeBase()
{

	DefaultPawnClass = AShootBaseCharacter::StaticClass();					//�� ��������� ����� - ����� ���������, � �� ������������ �� �����
	PlayerControllerClass = AShootPlayerController::StaticClass();			//�� ��������� ����� - �����������
	HUDClass = AShootGameHUD::StaticClass();								//��������� �����������

}
