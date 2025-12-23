export module model;

import <vector>;
import entity;
import shape;
import <memory>;

export class Model {
    std::vector<std::unique_ptr<Entity*>> entities;
    int height, width, start_x, start_y;
    bool solid_borders = true;
public:
    Model(int h = 22, int w = 80, int sx = 0, int sy = 0, bool sb = true)
        : height(h), width(w), start_x(sx), start_y(sy), solid_borders(sb) {}
    
    ~Model();
    
    void addEntity(std::unique_ptr<Entity*> entity);

    void removeEntity(Entity* entity);

    std::vector<std::unique_ptr<Entity*>>& getEntities();

    int getEntityCount() const;

    void updateEntities(int input);

    int getHeight() const;

    int getWidth() const;

    int getStartX() const;

    int getStartY() const;

    bool hasSolidBorders() const;

    void resetEntitiesAfterTick();
};
