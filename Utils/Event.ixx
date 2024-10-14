/**
* @file Event.ixx
 * @brief 事件类
 * @version 1.0
 * @author Chaim
 * @date 2024/09/30
 */

export module CEngine.Event;
import std;
import CEngine.Base;

namespace CEngine {
    export template<typename Res, typename... ArgTypes>
    class Event;

    /**
    * @brief <b>事件类</b>
    * @todo 增加删除绑定操作(-=)
    */
    export template<typename Res, typename... ArgTypes>
    class Event<Res(ArgTypes...)> {
        using rRes = std::conditional_t<std::is_void_v<Res>, Res, std::optional<Res> >; // 如果Res为void，则rRes为void，否则为optional<Res>

    public:
        Event() = default;
        ~Event() = default;

        /**
        * <b>绑定普通函数</b>
        * @param func 函数指针
        */
        Event &operator+=(const std::function<Res(ArgTypes...)> func) {
            if constexpr (std::is_void_v<Res>) {
                /* 判断void，减少封装 */
                Functions.push_back(func);
            } else {
                Functions.push_back([func](ArgTypes... args) -> rRes {
                    return {std::invoke(func, args...)};
                });
            }
            return *this;
        }

        /**
        * <b>绑定类函数</b> \n
        * 建议使用公开基础CEngine::Object的类
        * @param t 类型为tuple<对象指针, 类函数指针>
        */
        template<class T>
        Event &operator+=(std::tuple<T *, Res(T::*)(ArgTypes...)> t) {
            /* 类函数必须封装，不需要判断void */
            Functions.push_back([t](ArgTypes... args) -> rRes {
                if constexpr (std::is_base_of_v<Object, T>) {
                    /* 如果是Object的子类，增加对象可用判断 */
                    if (const auto obj_ptr = static_cast<Object *>(std::get<0>(t)); obj_ptr != nullptr && obj_ptr->IsValid()) {
                        return {std::invoke(std::get<1>(t), std::get<0>(t), args...)};
                    }
                    return std::nullopt; // 对象不可用
                } else {
                    /* 不是Object的子类，直接执行 */
                    return {std::invoke(std::get<1>(t), std::get<0>(t), args...)};
                }
            });
            return *this;
        }

        /**
        * <b>执行绑定在事件中的所有函数</b>
        * @param args 与原函数参数相同
        * @return 返回一个vector<optional<原函数返回类型>>，存储全部返回值，成员可能为<u>nullopt</u>
        */
        [[nodiscard]] auto Invoke(ArgTypes... args) {
            if constexpr (std::is_void_v<Res>) {
                for (auto &f: Functions) {
                    std::invoke(f, args...);
                }
            } else {
                std::vector<std::optional<Res> > res;
                for (auto &f: Functions) {
                    res.push_back(std::invoke(f, args...));
                }
                return res;
            }
        }

    private:
        std::vector<std::function<rRes(ArgTypes...)> > Functions;
    };
}
