
#include "Puckups/ShootBasePickup.h"
#include "Components/SphereComponent.h"

//лоиг пикапа
DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);


AShootBasePickup::AShootBasePickup()
{
 	
	PrimaryActorTick.bCanEverTick = true;

    //создали сферу и колизию у снаряда
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(50.0f);                                         //радиус коллизии
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);              //физического столкновения не будет, только оповещения о столкновении коллизии
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);   //реагирование коллизии на другие объекты каналов . Вызывает событие соприкосновения
    SetRootComponent(CollisionComponent);

}


void AShootBasePickup::BeginPlay()
{
	Super::BeginPlay();

    //проверка что задали коллизию
    check(CollisionComponent);

    //вращение Pickup
    GenerateRotationYaw();
}

void AShootBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //изменяем угол вращения
    AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f)); //угол х, у, z
}

//переопределяем функцию соприкосновения
void AShootBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    //что бы не потерять логику родительской функции NotifyActorBeginOverlap пишем
    Super::NotifyActorBeginOverlap(OtherActor);

    //локальная переменная указателя на Pawn.
    const auto Pawn = Cast<APawn>(OtherActor);//делаем каст актора, который пришел в качестве параметра функции в классе Pawn

    //если удалось взять Pickup, вызываем PickupWasTaken - Делаем невидимым
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();   //Делаем невидимым
    }
    //UE_LOG(LogBasePickup, Display, TEXT("Pickup was taken"));
    
    //уничтожение pickup. Но сделали вместо удаления, что он будет просто невидемый
    //Destroy();
    //PickupWasTaken();
}

//Вызывается когда взяли Pickup. Делаем невидимым и привязываем таймер
void AShootBasePickup::PickupWasTaken()
{
    //отключаем все коллизии, что бы не срабатывал BeginOverlap
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //прекращение взаимодействия коллизии с окружающим миром

    //проверка 
    if (GetRootComponent())
    {
        //получаем указатель на корневой компонент актора и делаем его невидимым
        GetRootComponent()->SetVisibility(false, true); //SetVisibility (1 параметр рендерить элемент, 2 visibilty применено для всех дочерних компонентов). Т.е. прекращаем рендерить все компоненты актора
    }

    //запускаем таймер респавна. будет локальной. делать отдельной необходимости нет, т.к. срабатывать будет один раз и отключать его не будем
    FTimerHandle RespawnTimerHandle;

    //при срабатывании вызывается Respawn
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AShootBasePickup::Respawn, RespawnTime);

}

//срабатывает по срабатыванию таймера респавна
//возвращает коллизию и делает снова видимым
void AShootBasePickup::Respawn()
{
    //вариативность, угол будет каждый раз разный при взятии Pickup
    GenerateRotationYaw();

    //включаем все коллизии, что бы срабатывал BeginOverlap
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); // взаимодействие коллизии с окружающим миром

    //проверка 
    if (GetRootComponent())
    {
        //получаем указатель на корневой компонент актора и делаем его видимым
        GetRootComponent()->SetVisibility(true, true); //SetVisibility (1 параметр рендерить элемент, 2 visibilty применено для всех дочерних компонентов)
    }
}

//В базовом классе Pickup делаем ложь, потому что по логике его взять просто невозможно
bool AShootBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

//вычисляет угол вращения
void AShootBasePickup::GenerateRotationYaw()
{
    //направление вращения 1 или -1 
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;    //RandBool возвращает рандомно истину или ложь, если истина принимаем 1, иначе -1. 

    //угол вращения
    RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;//генерация случайных числе из отрезка(min, max). Не делаем отрезок где есть 0, что бы выражение было 0
}