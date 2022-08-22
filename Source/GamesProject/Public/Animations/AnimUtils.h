#pragma once

//шаблонна€ функци€
class AnimUtils
{
public:
    template <typename T>
    //возвращает указатель на тип
    static T* FindNotifyByClass(UAnimSequenceBase* Animation)   //в UAnimSequenceBase определен массив animnotify. —делали статичной т.к. не зависит от конкретного экземпл€ра класса
    {
        //поиск нужного notify
        if (!Animation) return nullptr;

        const auto NotifyEvents = Animation->Notifies;
        for (auto NotifyEvent : NotifyEvents)
        {
            auto AnimNotify = Cast<T>(NotifyEvent.Notify);
           //если успешен каст, то возвращаем AnimNotify
            if (AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};