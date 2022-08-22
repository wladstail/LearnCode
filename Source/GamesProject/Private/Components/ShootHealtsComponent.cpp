
#include "Components/ShootHealtsComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"	//для камерашейк
#include "GameFramework/Controller.h" //для камерашейк
#include "Engine/World.h"
#include "TimerManager.h"	//таймер подключили на восполнения хп
#include "Camera/CameraShakeBase.h"	//для камерашейк


/*#include "DevContent/ShootFireDamageType.h"	//подключили огонь
#include "DevContent/ShootIceDamageType.h"	//лед
*/

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All)

UShootHealtsComponent::UShootHealtsComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UShootHealtsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//проверка что б на нуль не было деления
	check(MaxHealts > 0);

	//Присвоение хп в начале игры
	SetHealts(MaxHealts);
			//Healts = MaxHealts;		//при старте макс хп
			//OnHeatlsChanged.Broadcast(Healts);	//вызываем как присвоели жизни

	//Вызывает событие, делегат получение урона у владельца компонента
	AActor* ComponentOwner = GetOwner(); //GetOwner - возвращает указатель на владельца
	if (ComponentOwner)	//если не нулевой то подписываемся на владельца
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UShootHealtsComponent::OnTakeAnyDamageHandle);
	}
}

//вызывается при любом типе события
void UShootHealtsComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;//два условия если отрицательный урон по нам и мы мертвы выходим. третий если не мир то выходим
	
	//проверка хп на урон
	SetHealts(Healts - Damage);

	//Healts = FMath::Clamp(Healts - Damage, 0.0f, MaxHealts);//clamp 1 аргумент это значение которое от min до max, если не входит в интервал, то возвращается к ближайшей границе
	//OnHeatlsChanged.Broadcast(Healts);	//вызвали когда меняются жизни
	
	//прекращение автохила при дамаге
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);	//остановить таймер при попадании в нас

	//событие смерти персонажа и автохил персонажа
	if (IsDead())
	{
		OnDeath.Broadcast(); //этим оповещаем всех подписанных на делегат, что персонаж погиб
	}
	else if (AutoHill && GetWorld()) //если живы, то проверяем указатель на мир игры, потому что таймер в UWorld
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UShootHealtsComponent::HealUpdate, HealUpdateTime, true, HealDelay);//в таймере востановление хп(истина - цикл)
	}

	//тряска камеры при получении любого урона
	PlayCameraShake();
	//Healts -= Damage;


	// UE_LOG(BaseCharacterLog, Display, TEXT("Damage: %f"), Damage); 


/*
	if (DamageType)	//проверка что существует урон
	{
		//можно было по очереди делать каст на тип урона, т.е. приводить указатель дамага к интересующему типу дамага и если успешно, работать с ним
		if (DamageType->IsA<UShootFireDamageType>())	//IsA - возвращает истину если объект относится к этому типу
		{
			UE_LOG(BaseCharacterLog, Display, TEXT("Fire Damage "));
		}
		else if (DamageType->IsA<UShootIceDamageType>())	//проверка на леденой урон
		{
			UE_LOG(BaseCharacterLog, Display, TEXT("Ice Damage "));
		}
	}
	*/
}

//авто хил персонажа
void UShootHealtsComponent::HealUpdate()
{
	SetHealts(Healts + HealModifier);

	//Healts += FMath::Min(Healts + HealModifier, MaxHealts);// увеличение хп на велечину и не привысит макс велечину
	//	OnHeatlsChanged.Broadcast(Healts); //вызываем как присвоели жизни
	
	//сделали рефакторинг if (FMath::IsNearlyEqual(Healts, MaxHealts) && GetWorld()) //IsNearlyEqual проверяет на равенство 2 числа типа флоат, с некоторым эпсилом что будет записано третьим
	//if (Healts == MaxHealts && GetWorld())	//если хп полностью восстановилось вызывать восстановление нету смысла

	if (IsHealthFull() && GetWorld()) //IsNearlyEqual проверяет на равенство 2 числа типа флоат, с некоторым эпсилом что будет записано третьим
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);//обнулить таймер
	}
}

//Восстановление хп (сделали функцию из-за частого повторения в коде)
void UShootHealtsComponent::SetHealts(float NewHealts)
{
	//создаем локальную переменную для фикс бага виджетма мигания здоровья
	const auto NextHealth = FMath::Clamp(NewHealts, 0.0f, MaxHealts);	//ограничение хп максимальной величиной
	const auto HealthDelta = NextHealth - Healts;//разница между следующий значением здоровья и текущим

	//рефакторинг из-за фикс бага красного экрана в виджета
	//Healts = FMath::Clamp(NewHealts, 0.0f, MaxHealts);	//ограничение хп максимальной величиной
	Healts = NextHealth;
	//OnHeatlsChanged.Broadcast(Healts); //вызываем как присвоели жизни
	OnHeatlsChanged.Broadcast(Healts, HealthDelta); //добавили, второй аргумент, что бы фиксить баг вызывание красного экрана при автохиле, вызываем как присвоели жизни. HealthDelta - величина изменения здоровья
}

//возвращает истину, если успешно дабавлено хп Pickup
bool UShootHealtsComponent::TryToAddHealth(float HealthAmount)
{
	//проверка когда будет false
	if (IsDead() || IsHealthFull()) return false;	//либо персонаж мертв, либо полная шкала здоровья

	//восстанавление хп текущего + что в Pickup
	SetHealts(Healts + HealthAmount);

	return true;	//успешно добавлено хп
}

//полное ли хп
bool UShootHealtsComponent::IsHealthFull() const 
{
	//проверка на равенство
	return FMath::IsNearlyEqual(Healts, MaxHealts);
}

//проигрывание эффекта тряски камеры
void UShootHealtsComponent::PlayCameraShake()
{
	//проверка жив ли персонаж
	if (IsDead()) return;

	//получаем указатель на Pawn
	const auto Player = Cast<APawn>(GetOwner());	// Указатель на контроллер в APawn, поэтому нет смысла кастить к ACharacter

	//проверка указателя на нуль
	if (!Player) return;

	//получаем указатель на котроллер
	const auto Controller = Player->GetController<APlayerController>();

	//проверка указателя на нуль контроллера и камера менеджера
	if (!Controller ||!Controller->PlayerCameraManager) return;

	//вызываем камерашейк
	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}