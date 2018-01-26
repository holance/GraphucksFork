#pragma once
#include "..\Core\GraphicsTypes.h"
#include <functional>
#include <vector>
#include <map>
#include <memory>

namespace Graphucks
{
    class VisualElement;

    typedef LONGLONG EventToken;

    
    template<typename TArgument>
    class DelegateBase
    {
    public:
        virtual void Invoke(TArgument argument) = 0;
    };

    template<typename TOwner, typename TArgument>
    class DelegateMemberFunction : public DelegateBase<TArgument>
    {
        typedef void (TOwner::*Callback)(TArgument);
    public:
        DelegateMemberFunction(const std::shared_ptr<TOwner>& object, Callback callback)
        {
            m_weakRef = object;
            m_callback = callback;
        }

        virtual void Invoke(TArgument argument) override
        {
            std::shared_ptr<TOwner> object(m_weakRef);
        
            (object.get()->*m_callback)(argument);
        }
    private:

        std::weak_ptr<TOwner> m_weakRef;
        Callback m_callback;
    };

    template<typename TArgument>
    class DelegateFunction : public DelegateBase<TArgument>
    {
    public:
        DelegateFunction(std::function<void(TArgument)> callback)
        {
            m_callback = callback;
        }

        virtual void Invoke(TArgument argument) override
        {
            m_callback(argument);
        }
    private:
       
        std::function<void(TArgument)> m_callback;
    };

    template<typename TOwner,typename TArgument> struct DelegateFunctionPointer { typedef void (TOwner::*Type)(TArgument); };

    template<typename TArgument, typename TOwner>
    static std::shared_ptr<DelegateBase<TArgument>>
        CreateDelegate(typename DelegateFunctionPointer<typename TOwner::element_type,TArgument>::Type callbackPtr, TOwner owner)
    {
        return std::make_shared<DelegateMemberFunction<typename TOwner::element_type, TArgument>>(owner, callbackPtr);
    }

    template<typename TArgument>
    static std::shared_ptr<DelegateBase<TArgument>>
        CreateDelegate(std::function<void(TArgument)> callback)
    {
        return std::make_shared<DelegateFunction<TArgument>>(callback);
    }

    template<typename TArgument>
    class EventBase
    {
    public:
        EventBase() : m_tokenCounter(0)
        {
        }

        ~EventBase()
        {
        }
    public:

        void Invoke(const TArgument& arg )
        {
            for( auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++ )
            {
                (*i).second->Invoke(arg);
            }
        }

        EventToken operator += (const std::shared_ptr<DelegateBase<TArgument>>& delegateBase ) const
        {
            auto token = InterlockedIncrement64(&m_tokenCounter);

            m_handlerMap[token] = delegateBase;
            return token;
        }

        EventBase<TArgument>& operator -= (const std::shared_ptr<DelegateBase<TArgument>>& delegateBase )
        {
            for( auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++ )
            {
                if ( (*i) == delegateBase )
                {
                    m_handlerMap.erase( i );
                    break;
                }
            }

            return *this;
        }

        EventToken operator -= ( EventToken token ) const
        {
            m_handlerMap.erase(token);
            return token;
        }
    private:
        mutable LONGLONG m_tokenCounter;
        mutable std::map<EventToken, std::shared_ptr<DelegateBase<TArgument>>> m_handlerMap;
    };

    class EventArgs
    {
    public:
        EventArgs(const std::shared_ptr<void>& sender)
        {
            m_sender = sender;
        }

        virtual ~EventArgs()
        {

        }

        std::shared_ptr<void> Sender()
        {
            return m_sender;
        }
    private:
        std::shared_ptr<void> m_sender;

    };

    template<typename T1>
    class MulticastDelegate
    {
    public:
        MulticastDelegate() : m_tokenCounter(0)
        {

        }
        typedef std::function<void (std::shared_ptr<VisualElement>, T1)> CallbackFunction;

    public:
        void Invoke(const std::shared_ptr<VisualElement>& sender, const T1& arg )
        {
            for( auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++ )
            {
                (*i).second(sender, arg );
            }
        }

        void operator()(const std::shared_ptr<VisualElement>& sender, const T1& arg)
        {
            Invoke( sender, arg );
        }

        EventToken operator += ( CallbackFunction f ) const
        {
            auto token = InterlockedIncrement64(&m_tokenCounter);

            m_handlerMap[token] = f;
            return token;
        }

        MulticastDelegate& operator -= ( CallbackFunction f )
        {
            for( auto i = m_handlerMap.begin(); i != m_handlerMap.end(); i++ )
            {
                if ( (*i).target<void (T1)>() == f.target<void (T1)>() )
                {
                    m_handlerMap.erase( i );
                    break;
                }
            }

            return *this;
        }

        EventToken operator -= ( EventToken token ) const
        {
            m_handlerMap.erase(token);
            return token;
        }

    private:
        mutable LONGLONG m_tokenCounter;
        mutable std::map<EventToken, CallbackFunction> m_handlerMap;
    };

    class PointerDownEvent : public MulticastDelegate<Graphucks::Point>
    {

    };
}


