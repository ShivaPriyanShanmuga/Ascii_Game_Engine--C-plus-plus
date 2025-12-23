export module abstract_collider;

export class AbstractCollider {
public:
    AbstractCollider() = default;
    virtual ~AbstractCollider() = default;
    virtual bool getCollisionStatus() const = 0;
    virtual void setCollisionStatus(bool status) = 0;
    virtual void doAction() = 0;
};
