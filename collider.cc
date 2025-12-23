export module collider;

import entity;
import abstract_collider;


export class Collider : public AbstractCollider {
    bool hasCollided = false;
    Entity& entity;
public:
    Collider(Entity& ent) : entity(ent) {}

    bool getCollisionStatus() const {
        return hasCollided;
    }

    void setCollisionStatus(bool status) {
        hasCollided = status;
    }

    void doAction() {
        entity.hasCollided();
    }
};
