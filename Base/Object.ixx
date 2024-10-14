/**
* @file Object.ixx
 * @brief 基类
 * @version 1.0
 * @author Chaim
 * @date 2024/10/01
 */

export module CEngine.Base:Object;
import std;

namespace CEngine {
    /**
        * @class Object
        * @brief 基类
        * @remark 建议成员添加静态字符串成员TAG\n
        * @note 已实现判断对象指针是否可用\n
        */
    export class Object {
    public:
        const static char *TAG;

        Object() {
            _addr_.insert(this);
        }

        virtual ~Object() {
            _addr_.erase(this);
        }

        /**
        * 判断是否可用
        * @return 可以返回<code>true</code>，不可用返回<code>false</code>
        */
        bool IsValid() {
            return _addr_.contains(this);
        }

    private:
        /// @brief 用于存储实例化的对象地址
        static std::unordered_set<Object *> _addr_;
    };

    const char *Object::TAG = "Object";
    std::unordered_set<Object *> Object::_addr_;
}
