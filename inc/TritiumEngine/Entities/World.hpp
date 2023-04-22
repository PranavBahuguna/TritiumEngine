#pragma once

#define MAX_ENTITIES 5000

#include <TritiumEngine/Entities/Entity.hpp>

#include <array>

namespace TritiumEngine::Entities
{
  typedef std::array<Entity *, MAX_ENTITIES> EntityList;

  class World {
  public:
    World();
    ~World();

    const EntityList GetEntities() const;
    size_t GetNumEntities() const;

    void AddEntity(Entity *entity);
    void ClearEntities();
    void Init();
    void Update();

  private:
    EntityList m_entities;
    size_t m_nEntities;
    bool m_isInitialised;
  };
} // namespace TritiumEngine::Entities