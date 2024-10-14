/**
 * @file Node.ixx
 * @brief 节点类
 * @version 1.0
 * @author Chaim
 * @date 2024/10/06
 */

module;

export module CEngine.Node:Node;
import std;
import CEngine.Base;
import CEngine.Utils;
import CEngine.Logger;

namespace CEngine {
    /**
     * @brief 节点类
     * @remark 仅实现父子关系
     * @todo 实现引擎相关处理流程
     */
    export class Node : public Object {
    public:
        const static char *TAG;

        static Node *Create() {
            return new Node();
        }

        Node(const Node &other) = delete;
        Node &operator=(const Node &other) = delete;
        Node(Node &&other) noexcept = delete;
        Node &operator=(Node &&other) = delete;

        ~Node() override {
            for (const auto child: Children | std::views::values) {
                delete child;
            }
        }

        /**
         * 获取子级数量
         * @return 子级数量
         */
        std::size_t GetChildCount() const {
            return Children.size();
        }

        /**
         * 获取全部子级指针
         * @return 全部子级指针的vector容器
         */
        std::vector<Node *> GetChildren() {
            return std::ranges::to<std::vector<Node *> >(Children | std::views::values);
        }

        /**
         * 添加子级
         * @remark 注意：若节点已有父级，则会从原父级弹出！
         * @param node 子级
         */
        void AddChild(Node *node) {
            // 判断节点是否有原父级
            if (const auto raw_parent = node->Parent; raw_parent != nullptr) {
                raw_parent->PopChild(node->Name);
            }
            // 判断Name在新父级中是否唯一
            for (int i = 0; Children.contains(node->Name); ++i) {
                node->Name += std::format(".{}", i);
            }
            node->Parent = this;
            Children.emplace(node->Name, node);
        }

        /**
         * 删除子级
         * @param name 目标名称
         */
        void RemoveChild(const std::string &name) {
            delete PopChild(name);
        }

        /**
         * 弹出子级
         * @param name 目标名称
         * @return 目标对象的optional<Node>容器
         */
        Node *PopChild(const std::string &name) {
            const auto t = Children.extract(name);
            if (t.empty()) return nullptr;
            const auto n = t.mapped();
            n->Parent = nullptr;
            return n;
        }

        /**
         * 获得指定子级指针
         * @param name 目标名称
         * @return 目标子级指针
         */
        Node *GetChild(const std::string &name) {
            if (!HasChild(name)) return nullptr;
            return Children[name];
        }

        /// @link GetChild \endlink
        Node *operator[](const std::string &name) {
            return GetChild(name);
        }

        /// @link GetChild \endlink
        template<class T>
        T *GetChild(const std::string &name) {
            static_assert(std::is_base_of_v<Node, T>, "目标类型必须是Node的派生类!");
            if (!HasChild(name)) return nullptr;
            return dynamic_cast<T *>(Children[name]);
        }

        /**
         * 判断是否存在对应子级
         * @param name 子级名称
         * @return 是否存在对应子级
         */
        bool HasChild(const std::string &name) const {
            return Children.contains(name);
        }

        void PrintChildrenTree(Logger::LogLevel ll = Logger::LogLevel::D);

        /// @property Name
        std::string getName() const {
            return Name;
        }

        /// @property Name
        void setName(const std::string &name) {
            const std::string _name = name.empty() ? Utils::GenerateUUID() : name;
            if (Parent != nullptr) {
                // 判断所在层级中Name是否唯一
                std::string new_name = _name;
                for (int i = 0; Parent->HasChild(new_name); ++i) {
                    new_name = _name + std::format(".{}", i);
                }
                auto s = Parent->Children.extract(Name); // 弹出改key
                s.key() = new_name;
                Parent->Children.insert(std::move(s));
                Name = new_name;
            } else {
                Name = _name;
            }
        }

        /// @property Parent
        Node *getParent() const {
            return Parent;
        }

    protected:
        Node() {
            Name = Utils::GenerateUUID();
        }

        /// @brief Node的名称（在同层级中唯一）
        std::string Name;
        /// @brief 父级
        Node *Parent = nullptr;
        /// @brief 所有子级
        std::unordered_map<std::string, Node *> Children;
    };

    const char *Node::TAG = "Node";
}
