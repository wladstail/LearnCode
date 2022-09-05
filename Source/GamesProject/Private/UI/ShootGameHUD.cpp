// Shoot Game


#include "UI/ShootGameHUD.h"
#include "Engine/Canvas.h"	//����������� ������� 
#include "Blueprint/UserWidget.h"	//����� �������� �������

void AShootGameHUD::DrawHUD()
{

	Super::DrawHUD();

	//������ ����������� ������ �.�. ���� ��� �������
	//DrawCrossHair();
}

//� ��� ����� ����������� ��� PlayerHUDWidget
void AShootGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	//���������
	auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);	//��� ���������� ��������� - ��� �������. 1 ��������� �� ������ ��������� 2 ����� ������� ������� ����� �������

	//�������� ���� ������ ��������, �� ��� ����������� �� ������ ���
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();	//AddToViewport, Z-order �������� �� ������� ��������� �������
	}
}

//������
void AShootGameHUD::DrawCrossHair()
{
	//int32 SizeX = Canvas->SizeX;//Canvas - � ��� ���������� ���� ���������� � ���� ������� ���� X
	//int32 SizeY = Canvas->SizeY;// ������� ���� Y
	//const TInterval<float> Center(SizeX * 0.5f, SizeY * 0.5f); //TInterval - ������� , Center - ������

	//����� ����
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.0f;	//�������� �� �������� �����
	const float LineThickness = 2.0f;	//������� �����
	const FLinearColor LineColor = FLinearColor::Red;
		
	//������ ����� �� ������ �����, ����, �����, ������
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min+ HalfLineSize, Center.Max, LineColor, LineThickness);	//�������������� �����
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);	//������������ �����

}	