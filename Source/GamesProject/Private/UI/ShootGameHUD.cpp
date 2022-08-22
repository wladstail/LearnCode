// Shoot Game


#include "UI/ShootGameHUD.h"
#include "Engine/Canvas.h"	//подключения канваса 
#include "Blueprint/UserWidget.h"	//класс базового виджета

void AShootGameHUD::DrawHUD()
{

	Super::DrawHUD();

	//прицел дебагерский убрали т.к. есть уже виджеты
	//DrawCrossHair();
}

//в нем будет создаваться наш PlayerHUDWidget
void AShootGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	//указатель
	auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);	//тип принимаего параметра - тип виджета. 1 указатель на объект владельца 2 класс виджета который хотим создать

	//проверка если виджет создался, то вкл отображение на экране его
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();	//AddToViewport, Z-order отвечает за порядок отрисовки виджета
	}
}

//прицел
void AShootGameHUD::DrawCrossHair()
{
	//int32 SizeX = Canvas->SizeX;//Canvas - в нем прорисовка всех примитивов и есть размеры окна X
	//int32 SizeY = Canvas->SizeY;// размеры окна Y
	//const TInterval<float> Center(SizeX * 0.5f, SizeY * 0.5f); //TInterval - отрезок , Center - объект

	//центр окна
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.0f;	//отвечает за половину линии
	const float LineThickness = 2.0f;	//толщина линий
	const FLinearColor LineColor = FLinearColor::Red;
		
	//рисуем линию от центра вверх, вниз, влево, вправо
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min+ HalfLineSize, Center.Max, LineColor, LineThickness);	//горизонтальная линия
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);	//вертикальная линия

}	