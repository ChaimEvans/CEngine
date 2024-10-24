/**
 * @file Behaviour.ixx
 * @brief
 * @version 1.0
 * @author Chaim
 * @date 2024/10/20
 */

export module CEngine.Node:Behaviour;
import CEngine.Base;

namespace CEngine {
    export class Node;

    export class Behaviour : public Object {
    public:
        Behaviour() = default;
        ~Behaviour() override = default;

        void Process(const double DeltaTime) {
            if (!ReadyCalled) Ready();
            Update(DeltaTime);
        }

        virtual void Ready() = 0;

        virtual void Update(double DeltaTime) = 0;

        virtual void Release() {
        }

        void SetParentNode(Node *node) { ParentNode = node; }
        Node *GetParentNode() const { return ParentNode; }

    protected:
        Node *ParentNode;

    private:
        bool ReadyCalled = false;
    };
}
